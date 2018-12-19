#pragma once

#include <fstream>
#include "typelists-m.h"


template<typename TList1, typename TList2, typename TList3>
struct Reader2Helper;

// Using compressed
template<typename Head1, typename... Tail1, typename Head2, typename... Tail2, typename... Tail3>
struct Reader2Helper<TypeList<Head1, Tail1...>, TypeList<Head2, Tail2...>, TypeList<NoDecompressor, Tail3...>>
{
    static void Read( std::ifstream& ifs, char* mem )
    {
        char* srcMem = reinterpret_cast<char*>(operator new(Head2::SizeOf()));
        ifs.read(srcMem, Head2::SizeOf());
        Head2 compressed(srcMem, mem);

        compressed.decompress();

        Reader2Helper<TypeList<Tail1...>, TypeList<Tail2...>, TypeList<Tail3...>>::Read( ifs, mem + Head2::SizeOf() );
    }
};

// Using decompressor
template<typename Head1, typename... Tail1, typename... Tail2, typename Head3, typename... Tail3>
struct Reader2Helper<TypeList<Head1, Tail1...>, TypeList<NoneType, Tail2...>, TypeList<Head3, Tail3...>>
{
    static void Read( std::ifstream& ifs, char* mem )
    {
        char* srcMem = reinterpret_cast<char*>(operator new(Head3::CompressedType::SizeOf()));
        ifs.read(srcMem, Head3::CompressedType::SizeOf());
        typename Head3::CompressedType compressed(srcMem);

        Head3 decompressor(compressed, mem);

        decompressor.decompress();

        Reader2Helper<TypeList<Tail1...>, TypeList<Tail2...>, TypeList<Tail3...>>::Read( ifs, mem + Head3::CompressedType::SizeOf() );
    }
};

template<>
struct Reader2Helper<EmptyList, EmptyList, EmptyList>
{
    static void Read( std::ifstream& ifs, char* mem )
    {
    }
};

template<typename ElemTList, typename CompressedElemTList, typename DecompressorsTList>
class Reader2
{
    std::ifstream ifs;
 public:
    Reader2( const std::string& filename ) : ifs( filename )
    {
        if( ifs.fail() ) {
            throw std::exception();
        }
    }

    void* ReadNextLine()
    {
        char* mem = reinterpret_cast<char*>(operator new( GetInMemSize<ElemTList>::value ));
        Reader2Helper<ElemTList, CompressedElemTList, DecompressorsTList>::Read( ifs, mem );
        return mem;
    }
};
