#pragma once

#include <stdexcept>
#include "TypeInfo.h"
#include <map>
#include <iostream>
#include "Functor.h"
#include "ExampleTypeLists.h"


namespace m {

template <class To, class From>
struct StaticCaster
{
    static To& Cast(From& obj)
    {
        return static_cast<To&>(obj);
    }
};

template <class To, class From>
struct DynamicCaster
{
    static To& Cast(From& obj)
    {
        return dynamic_cast<To&>(obj);
    }
};

template<typename... Args>
using AssocVector = std::map<Args...>;






template
    <
        class BaseLhs,
        class BaseRhs = BaseLhs,
        typename ResultType = void,
        typename CallbackType = ResultType (*)(BaseLhs&, BaseRhs&)
    >
class BasicDispatcher
{
    typedef std::pair<TypeInfo,TypeInfo> KeyType;
    typedef CallbackType MappedType;
    typedef AssocVector<KeyType, MappedType> MapType;
    MapType callbackMap_;

    void DoAdd(TypeInfo lhs, TypeInfo rhs, CallbackType fun);
    bool DoRemove(TypeInfo lhs, TypeInfo rhs);

 public:
    template <class SomeLhs, class SomeRhs>
    void Add(CallbackType fun)
    {
        DoAdd(typeid(SomeLhs), typeid(SomeRhs), fun);
    }

    template <class SomeLhs, class SomeRhs>
    bool Remove()
    {
        return DoRemove(typeid(SomeLhs), typeid(SomeRhs));
    }

    ResultType Go(BaseLhs& lhs, BaseRhs& rhs);
};

// Non-inline to reduce compile time overhead...
template <class BaseLhs, class BaseRhs,
    typename ResultType, typename CallbackType>
void BasicDispatcher<BaseLhs,BaseRhs,ResultType,CallbackType>
::DoAdd(TypeInfo lhs, TypeInfo rhs, CallbackType fun)
{
    callbackMap_[KeyType(lhs, rhs)] = fun;
}

template <class BaseLhs, class BaseRhs,
    typename ResultType, typename CallbackType>
bool BasicDispatcher<BaseLhs,BaseRhs,ResultType,CallbackType>
::DoRemove(TypeInfo lhs, TypeInfo rhs)
{
    return callbackMap_.erase(KeyType(lhs, rhs)) == 1;
}

template <class BaseLhs, class BaseRhs,
    typename ResultType, typename CallbackType>
ResultType BasicDispatcher<BaseLhs,BaseRhs,ResultType,CallbackType>
::Go(BaseLhs& lhs, BaseRhs& rhs)
{
    typename MapType::key_type k(typeid(lhs),typeid(rhs));
    typename MapType::iterator i = callbackMap_.find(k);
    if (i == callbackMap_.end())
    {
        throw std::runtime_error("Function not found");
    }
    return (i->second)(lhs, rhs);
}

////////////////////////////////////////////////////////////////////////////////
// class template FunctorDispatcherAdaptor
// permits use of FunctorDispatcher under gcc.2.95.2/3
///////////////////////////////////////////////////////////////////////////////

namespace Private
{
template <class BaseLhs, class BaseRhs,
    class SomeLhs, class SomeRhs,
    typename ResultType,
    class CastLhs, class CastRhs,
    class Fun, bool SwapArgs>
class FunctorDispatcherHelper
{
    Fun fun_;
    ResultType Fire(BaseLhs& lhs, BaseRhs& rhs,Int2Type<false>)
    {
        return fun_(CastLhs::Cast(lhs), CastRhs::Cast(rhs));
    }
    ResultType Fire(BaseLhs& rhs, BaseRhs& lhs,Int2Type<true>)
    {
        return fun_(CastLhs::Cast(lhs), CastRhs::Cast(rhs));
    }
 public:
    FunctorDispatcherHelper(const Fun& fun) : fun_(fun) {}

    ResultType operator()(BaseLhs& lhs, BaseRhs& rhs)
    {
        return Fire(lhs,rhs,Int2Type<SwapArgs>());
    }
};
}

////////////////////////////////////////////////////////////////////////////////
// class template FunctorDispatcher
// Implements a logarithmic double dispatcher for functors
// Features automated casting
////////////////////////////////////////////////////////////////////////////////

template <class BaseLhs, class BaseRhs = BaseLhs,
    typename ResultType = void,
    template <class, class> class CastingPolicy = DynamicCaster,
    template <class, class, class, class>
    class DispatcherBackend = BasicDispatcher>
class FunctorDispatcher
{
    typedef TypeList<BaseLhs&, BaseRhs&> ArgsList;
    typedef Functor<ResultType, BaseLhs&, BaseRhs&> FunctorType;

    DispatcherBackend<BaseLhs, BaseRhs, ResultType, FunctorType> backEnd_;

 public:
    template <class SomeLhs, class SomeRhs, class Fun>
    void Add(const Fun& fun)
    {
        typedef Private::FunctorDispatcherHelper<
            BaseLhs, BaseRhs,
            SomeLhs, SomeRhs,
            ResultType,
            CastingPolicy<SomeLhs, BaseLhs>,
            CastingPolicy<SomeRhs, BaseRhs>,
            Fun, false> Adapter;

        backEnd_.template Add<SomeLhs, SomeRhs>(FunctorType(Adapter(fun)));
    }
    template <class SomeLhs, class SomeRhs, bool symmetric, class Fun>
    void Add(const Fun& fun)
    {
        Add<SomeLhs,SomeRhs>(fun);

        if (symmetric)
        {
            // Note: symmetry only makes sense where BaseLhs==BaseRhs
            typedef Private::FunctorDispatcherHelper<
                BaseLhs, BaseLhs,
                SomeLhs, SomeRhs,
                ResultType,
                CastingPolicy<SomeLhs, BaseLhs>,
                CastingPolicy<SomeRhs, BaseLhs>,
                Fun, true> AdapterR;

            backEnd_.template Add<SomeRhs, SomeLhs>(FunctorType(AdapterR(fun)));
        }
    }

    template <class SomeLhs, class SomeRhs>
    void Remove()
    {
        backEnd_.template Remove<SomeLhs, SomeRhs>();
    }

    ResultType Go(BaseLhs& lhs, BaseRhs& rhs)
    {
        return backEnd_.Go(lhs, rhs);
    }
};

struct Shape {
    virtual ~Shape() = default;
    int a;
};

struct Square : public Shape {
    Square() {}
    ~Square() = default;
};

struct Circle : public Shape {
    Circle() {}
    ~Circle() = default;
};

struct Triangle : public Shape {
    Triangle() {}
    ~Triangle() = default;
};

void DoSquareCircle(Square& a, Circle& b) {
    std::cout << "square circle" << std::endl;
}
void DoSquareCirclePtr(Square* a, Circle* b) {
    std::cout << "square circle" << std::endl;
}
void Do(Triangle& a, Circle& b) {
    std::cout << "triangle circle" << std::endl;
}

struct DoSquareCircleHatch {
    void operator()(Square& a, Circle& b) {
        std::cout << "square circle" << std::endl;
    }
};

void TestFunctorDispatcher() {
    Functor<int, int, char> f(ExampleFunc);
    Square* square = new Square;
    Circle* circle = new Circle;

    /*FunctorDispatcher<Shape&> disp;
    disp.Add<Square&, Circle&>(DoSquareCircle);
    disp.Go(*square, *circle);
     */
    /*FunctorDispatcher<Shape*> disp;
    disp.Add<Square*, Circle*>(DoSquareCirclePtr);
    disp.Go(square, circle);*/
    FunctorDispatcher<Shape> disp;
    disp.Add<Square, Circle>(DoSquareCircleHatch());
    disp.Go(*square, *circle);
}

}
