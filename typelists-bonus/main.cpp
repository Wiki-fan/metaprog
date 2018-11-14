#include <iostream>
#include <cassert>
#include <vector>
#include "typelists-m.h"
#include "hierarchy-m.h"
#include "abstract_factory_gen-m.h"

using namespace m;

using CharList = TypeList<char, signed char, unsigned char>;
using NumList = TypeList<int, double>;
using AllList = Append<CharList, NumList>::Result;
using AllWithoutInt = Erase<AllList, int>::Result;

void test_typelists() {

    assert(typeid(CharList::tail) == typeid(TypeList<signed char, unsigned char>));
    assert(typeid(CharList::tail::tail) == typeid(TypeList<unsigned char>));
    assert(typeid(CharList::tail::tail::head) == typeid(unsigned char));

    // Задание 1: пофиксить длину EmptyList. Просто специфицируем шаблон.
    static_assert(Length<CharList>::value == 3);
    static_assert(Length<TypeList<int>>::value == 1);
    static_assert(Length<EmptyList>::value == 0);

    // Остальные штуки и тесты.
    static_assert(IndexOf<EmptyList, signed char>::value == -1);
    static_assert(IndexOf<CharList, signed char>::value == 1);
    static_assert(IndexOf<CharList, char>::value == 0);

    TypeAt<CharList, 0>::Result c;
    TypeAt<CharList, 1>::Result sc;

    static_assert(Length<AllList>::value == 5);
    static_assert(IndexOf<AllList, double>::value == 4);

    static_assert( Length<Append<EmptyList, NumList>::Result>::value == 2);
    static_assert( Length<Append<NumList, EmptyList>::Result>::value == 2);
    static_assert( Length<Append<NumList, float>::Result>::value == 3);

    // Задание 2: удаление.
    static_assert(Length<AllWithoutInt>::value == 4);
    static_assert(IndexOf<AllWithoutInt, int>::value == -1);

    static_assert(IndexOf<AllWithoutInt, char>::value == 0);
    static_assert(IndexOf<AllWithoutInt, signed char>::value == 1);
    static_assert(IndexOf<AllWithoutInt, unsigned char>::value == 2);
    static_assert(IndexOf<AllWithoutInt, double>::value == 3);

    static_assert(Length<Erase<AllList, char>::Result>::value == 4);
    static_assert(Length<Erase<AllList, double>::Result>::value == 4);
    static_assert(Length<Erase<AllList, bool>::Result>::value == 5);
}

struct Int {
    int intValue;
};

struct Char {
    char value;
};

using StructList = TypeList<Int, Char>;
using StructVectorHierarchy = GenScatterHierarchy<StructList, std::vector>;
typedef GenScatterHierarchy<CharList, std::vector> H;

template <class T>
struct Holder {
    T value;
};
using WidgetInfoScatter = GenScatterHierarchy<TypeList<int, std::string, double>, Holder>;

WidgetInfoScatter::LeftBase a;

void test_scatter_hierarchy() {
    H::LeftBase lb;
    H vec;
    StructVectorHierarchy swh;
    auto& a = static_cast<std::vector<Int>&>(swh);
    a.push_back(Int());
    a[0].intValue = 4;

    assert(typeid(WidgetInfoScatter::LeftBase) == typeid(TypeList<int>));

    WidgetInfoScatter obj;
    static_cast<Holder<std::string>&>(obj).value = "hello";

    std::string name = static_cast<Holder<std::string>&>(obj).value;
    assert(name == "hello");

    //GetSpec<std::string, Holder, TypeList<int, std::string>>(obj);
    assert(GetSpec<std::string>(obj).value == "hello");

}

template <class T, class Base>
struct HolderLinear {
    T value;
};
using WidgetInfoLinear = GenLinearHierarchy<TypeList<int, double>, HolderLinear, NullType>;

/*void test_linear_hierarchy() {
    assert(typeid(WidgetInfoLinear::current) == typeid(int));
    assert(typeid(WidgetInfoLinear) == typeid(int));
    WidgetInfoLinear* obj;
    //static_cast<HolderLinear<double, NullType>*>(obj).value = 6.0;


    //static_cast<HolderLinear<std::string, GenLinearHierarchy<TypeList<double>, HolderLinear, NullType>&>>(obj).value = "hello";

    std::string name = static_cast<Holder<std::string>&>(obj).value;
    assert(name == "hello");
    assert(GetSpec<std::string>(obj).value == "hello");
}
*/
int main() {
    test_typelists();
    test_scatter_hierarchy();
    //test_linear_hierarchy();

    //m::test_factory();
    simple::test_factory();

    return 0;
}
