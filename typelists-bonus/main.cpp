#include <iostream>
#include <cassert>
#include <vector>
#include "typelists-m.h"
#include "hierarchy-m.h"

using namespace m;

using CharList = TypeList<char, signed char, unsigned char>;
using NumList = TypeList<int, double>;
using AllList = Append<CharList, NumList>::Result;
using AllWithoutInt = Erase<AllList, int>::Result;

void test_typelists() {

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
using WidgetInfo = GenScatterHierarchy<TypeList<int, std::string>, Holder>;

template<typename T, template<typename, typename...> typename Unit, typename TL>
Holder<T>& GetSpec(GenScatterHierarchy<TL, Unit>& obj) {
    return static_cast<Unit<T>&>(obj);
}

void test_hierarchy() {
    H::LeftBase lb;
    H vec;
    StructVectorHierarchy swh;
    auto& a = static_cast<std::vector<Int>&>(swh);
    a.push_back(Int());
    a[0].intValue = 4;

    WidgetInfo obj;
    static_cast<Holder<std::string>&>(obj).value = "hello";

    std::string name = static_cast<Holder<std::string>&>(obj).value;
    assert(name == "hello");

    //GetSpec3<std::string, Holder, TypeList<int, std::string>>(obj);
    assert(GetSpec<std::string>(obj).value == "hello");

}

int main() {
    test_typelists();
    test_hierarchy();

    return 0;
}
