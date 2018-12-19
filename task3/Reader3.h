#pragma once

template<typename TList1, typename TList2, DecompressorFunctionPtr*, int i>
struct Reader3Helper;

// Using first and maybe second
template<typename Head1, typename... Tail1, typename Head2, typename... Tail2, DecompressorFunctionPtr* lst, int i>
struct Reader3Helper<TypeList<Head1, Tail1...>, TypeList<Head2, Tail2...>, lst, i>
{
    static void Read( std::ifstream& ifs, char* mem )
    {
        char* srcMem = reinterpret_cast<char*>(operator new( Head2::SizeOf() ));
        char* tempMem = reinterpret_cast<char*>(operator new( sizeof( Head1 ) ));
        ifs.read( srcMem, Head2::SizeOf() );
        Head2 compressed( srcMem, tempMem );

        compressed.decompress();

        DecompressorFunctionPtr funcPtr = lst[i];
        if( funcPtr != nullptr ) {
            funcPtr( mem, tempMem, sizeof( Head1 ) );
        } else {
            memmove(mem, tempMem, sizeof( Head1 ));
        }

        delete srcMem;
        delete tempMem;

        Reader3Helper<TypeList<Tail1...>, TypeList<Tail2...>,
                      lst, i+1>::Read( ifs, mem + sizeof( Head1 ) );
    }
};

// Not using first and maybe second
template<typename Head1, typename... Tail1, typename... Tail2, DecompressorFunctionPtr* lst, int i>
struct Reader3Helper<TypeList<Head1, Tail1...>, TypeList<NoneType, Tail2...>, lst, i>
{
    static void Read( std::ifstream& ifs, char* mem )
    {
        char* srcMem = reinterpret_cast<char*>(operator new( sizeof( Head1 ) ));
        ifs.read( srcMem, sizeof( Head1 ) );

        DecompressorFunctionPtr funcPtr = lst[i];
        if( funcPtr != nullptr ) {
            funcPtr( mem, srcMem, sizeof( Head1 ) );
        } else {
            memmove(mem, srcMem, sizeof( Head1 ));
        }

        delete srcMem;

        Reader3Helper<TypeList<Tail1...>, TypeList<Tail2...>,
            lst, i >::Read( ifs, mem + sizeof( Head1 ) );
    }
};

template<DecompressorFunctionPtr* lst, int i>
struct Reader3Helper<EmptyList, EmptyList, lst, i>
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
        Reader3Helper<ElemTList, CompressedElemTList, lst, 0>::Read( ifs, mem );
        return mem;
    }
};
