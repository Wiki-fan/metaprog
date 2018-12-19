#pragma once

#include "typelists-m.h"

template<typename TList_>
struct Reader1Helper;

template<typename Head, typename... Tail>
struct Reader1Helper<TypeList<Head, Tail...>>
{
static void Read( std::ifstream& ifs, char* mem )
{
    int size = sizeof( Head );
    ifs >> *reinterpret_cast<Head*>(mem);
    Reader1Helper<TypeList<Tail...>>::Read( ifs, mem + size );
}
};

template<>
struct Reader1Helper<EmptyList>
{
    static void Read( std::ifstream& ifs, char* mem )
    {
    }
};

template<typename TList>
class Reader1
{

    std::ifstream ifs;
 public:
    Reader1( const std::string& filename ) : ifs( filename )
    {
        if( ifs.fail() ) {
            throw std::exception();
        }
    }

    void* ReadNextLine()
    {
        char* mem = reinterpret_cast<char*>(operator new( GetInMemSize<TList>::value ));
        Reader1Helper<TList>::Read( ifs, mem );
        return mem;
    }
};
