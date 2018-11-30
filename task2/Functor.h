#pragma once

#include <cassert>
#include <memory>
#include "typelists-m.h"


namespace m
{
//
//template<typename ResultType_, class TList_=NullType>
//class Functor
//{
// public:
//    typedef ResultType_ ResultType;
//    typedef TList_ ArgumentsTypeList;
//
//    Functor() : spImpl_( 0 )
//    {
//    }
//
//    Functor( const Functor& rhs ) : spImpl_( Impl::Clone( rhs.spImpl_.get() ) )
//    {
//    }
//
//    Functor( std::auto_ptr <Impl> spImpl ) : spImpl_( spImpl )
//    {
//    }
//
//    template<typename Fun>
//    Functor( Fun fun )
//        : spImpl_( new FunctorHandler<Functor, Fun>( fun ) )
//    {
//    }
//
//    template<class PtrObj, typename MemFn>
//    Functor( const PtrObj& p, MemFn memFn )
//        : spImpl_( new MemFunHandler<Functor, PtrObj, MemFn>( p, memFn ) )
//    {
//    }
//
//    typedef Impl* (std::auto_ptr<Impl>::*unspecified_bool_type)() const;
//
//    operator unspecified_bool_type() const
//    {
//        return spImpl_.get() ? &std::auto_ptr<Impl>::get : 0;
//    }
//
//    Functor& operator=( const Functor& rhs )
//    {
//        Functor copy( rhs );
//        // swap auto_ptrs by hand
//        Impl* p = spImpl_.release();
//        spImpl_.reset( copy.spImpl_.release() );
//        copy.spImpl_.reset( p );
//        return *this;
//    }
//
//    ResultType operator()() const
//    {
//        return ( *spImpl_ )();
//    }
//
//    ResultType operator()( Parm1 p1 ) const
//    {
//        return ( *spImpl_ )( p1 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2 ) const
//    {
//        return ( *spImpl_ )( p1, p2 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3, p4 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3, p4, p5 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
//                           Parm6 p6 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3, p4, p5, p6 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
//                           Parm6 p6, Parm7 p7 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3, p4, p5, p6, p7 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
//                           Parm6 p6, Parm7 p7, Parm8 p8 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3, p4, p5, p6, p7, p8 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
//                           Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3, p4, p5, p6, p7, p8, p9 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
//                           Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
//                           Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
//                           Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
//                           Parm12 p12 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
//                             p12 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
//                           Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
//                           Parm12 p12, Parm13 p13 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
//                             p12, p13 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
//                           Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
//                           Parm12 p12, Parm13 p13, Parm14 p14 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
//                             p12, p13, p14 );
//    }
//
//    ResultType operator()( Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
//                           Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
//                           Parm12 p12, Parm13 p13, Parm14 p14, Parm15 p15 ) const
//    {
//        return ( *spImpl_ )( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
//                             p12, p13, p14, p15 );
//    }
//
// private:
//    std::auto_ptr <Impl> spImpl_;
//};
//
//namespace Private
//{
//template<class Fctor>
//struct BinderFirstTraits;
//
//template<typename R, class TList, template<class> class ThreadingModel>
//struct BinderFirstTraits<Functor<R, TList, ThreadingModel>>
//{
//    typedef typename TL::Erase<TList,
//                               typename TL::TypeAt<TList, 0>::Result>::Result
//        ParmList;
//    typedef Functor<R, ParmList, ThreadingModel> BoundFunctorType;
//    typedef typename BoundFunctorType::Impl Impl;
//};
//}

template<typename ResultType_, typename... ArgsTypes_>
struct Functor
{
    struct FunctorWrapperBase
    {
        virtual ResultType_ operator()( ArgsTypes_... args ) = 0;

        virtual FunctorWrapperBase* Clone(const FunctorWrapperBase* other) = 0;
    };

    template<typename Fun_>
    struct FunctorWrapper : public FunctorWrapperBase
    {
        Fun_ fun;

        FunctorWrapper( Fun_ f ) : fun( f )
        {
        };

        ResultType_ operator()( ArgsTypes_... args )
        {
            return fun( args... );
        }

        FunctorWrapper* Clone(const FunctorWrapperBase* other) {
            Fun_ f = static_cast<const FunctorWrapper*>(other)->fun;
            auto tmp = new FunctorWrapper(f);
            return tmp;
        }
    };

    using ResultType=ResultType_;
    using ArgsTypeList = TypeList<ArgsTypes_...>;
    using FuncPtrType = ResultType( * )( ArgsTypes_... );

    std::unique_ptr<FunctorWrapperBase> wrapper;

    Functor( ) = default;

    template<typename Fun>
    explicit Functor( Fun fun_ ) : wrapper( new FunctorWrapper<Fun>( fun_ ) )
    {
    }

    explicit Functor( FuncPtrType ptr) : wrapper( new FunctorWrapper<FuncPtrType>( ptr ) )
    {
    }

    Functor(const Functor& other) : wrapper(wrapper->Clone(other.wrapper.get())) {}

    Functor& operator=(const Functor& rhs)
    {
        Functor copy(rhs);
        // swap auto_ptrs by hand
        FunctorWrapperBase* p = wrapper.release();
        wrapper.reset(copy.wrapper.release());
        copy.wrapper.reset(p);
        return *this;
    }

    ResultType Call( ArgsTypes_... args )
    {
        return ( *wrapper )( args... );
    }

    ResultType operator()( ArgsTypes_... args )
    {
        return Call( args... );
    }
};

int ExampleFunc( int a, char b )
{
    return 1;
}

using ExampleFuncType = int ( * )( int, char );

void TestFunctor()
{
    ExampleFuncType t = ExampleFunc;
    Functor<int, int, char> f( ExampleFunc );
    assert( f.Call( 1, 'c' ) == 1 );
}

}
