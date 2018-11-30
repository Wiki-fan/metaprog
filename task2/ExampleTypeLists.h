#pragma once

#include "typelists-m.h"


namespace m
{
template<int v>
struct Int2Type
{
    enum
    {
        value = v
    };
};

using TL8 = TypeList
    <
        Int2Type<0>,
        Int2Type<1>,
        Int2Type<2>,
        Int2Type<3>,
        Int2Type<4>,
        Int2Type<5>,
        Int2Type<6>,
        Int2Type<7>
    >;

using TL3 = TypeList
    <
        Int2Type<0>,
        Int2Type<1>,
        Int2Type<2>
    >;
}

