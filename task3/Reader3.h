#pragma once

template<typename TList1, typename TList2, DecompressorFunctionPtr*>
struct Reader3Helper;

// Using first and maybe second
template<typename Head1, typename... Tail1, typename Head2, typename... Tail2, DecompressorFunctionPtr* lst>
struct Reader3Helper<TypeList<Head1, Tail1...>, TypeList<Head2, Tail2...>, lst>
{
    static void Read( std::ifstream& ifs, char* mem )
    {
        char* srcMem = reinterpret_cast<char*>(operator new( Head2::SizeOf() ));
        char* tempMem = reinterpret_cast<char*>(operator new( sizeof( Head1 ) ));
        ifs.read( srcMem, Head2::SizeOf() );
        Head2 compressed( srcMem, tempMem );

        compressed.decompress();

        if( lst[0] != nullptr ) {
            lst[0]( mem, tempMem, sizeof( Head1 ) );
        }

        Reader3Helper<TypeList<Tail1...>, TypeList<Tail2...>,
                      lst + 1>::Read( ifs, mem + sizeof( Head1 ) );
    }
};

// Not using first and maybe second
template<typename Head1, typename... Tail1, typename... Tail2, DecompressorFunctionPtr* lst>
struct Reader3Helper<TypeList<Head1, Tail1...>, TypeList<NoneType, Tail2...>, lst>
{
    static void Read( std::ifstream& ifs, char* mem )
    {
        char* srcMem = reinterpret_cast<char*>(operator new( sizeof( Head1 ) ));
        ifs.read( srcMem, sizeof( Head1 ) );

        if( lst[0] != nullptr ) {
            lst[0]( mem, srcMem, sizeof( Head1 ) );
        }

        Reader3Helper<TypeList<Tail1...>, TypeList<Tail2...>,
            (DecompressorFunctionPtr*)lst + 1 >::Read( ifs, mem + sizeof( Head1 ) );
    }
};

template<DecompressorFunctionPtrArr lst>
struct Reader3Helper<EmptyList, EmptyList, lst>
{
    static void Read( std::ifstream& ifs, char* mem )
    {
    }
};

template<typename ElemTList, typename CompressedElemTList, DecompressorFunctionPtr* lst>
class Reader3
{
    std::ifstream ifs;
 public:
    Reader3( const std::string& filename ) : ifs( filename )
    {
        if( ifs.fail() ) {
            throw std::exception();
        }
    }

    void* ReadNextLine()
    {
        char* mem = reinterpret_cast<char*>(operator new( GetInMemSize<ElemTList>::value ));
        Reader3Helper<ElemTList, CompressedElemTList, (DecompressorFunctionPtr*)lst>::Read( ifs, mem );
        return mem;
    }
};
