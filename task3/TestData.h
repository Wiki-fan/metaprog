#pragma once
#include <cstring>
#include <type_traits>
#include <list>


struct Foo
{
    int a;
    char c;

    Foo( int a_, char c_ ) : a( a_ ), c( c_ )
    {
    }

    friend std::istream& operator>>( std::istream& istream, Foo& foo );
};

std::istream& operator>>( std::istream& istream, Foo& foo )
{
    istream >> foo.a >> foo.c;
    return istream;
}

// Разжимает данные из in в количестве inSz байт и кладёт результат по указателю out.
void ImitateCleverDecompressionAlgorithm( void* dstMem, void* srcMem, size_t inSz )
{
    memcpy( dstMem, srcMem, inSz );
}

// Разжимает данные из in в количестве inSz байт и кладёт результат по указателю out.
void ImitateCleverPostDecompressionAlgorithm( void* dstMem, void* srcMem, size_t inSz )
{
    memcpy( dstMem, srcMem, inSz );
}

struct CompressedFooSelfDecompressable
{

    CompressedFooSelfDecompressable( char* srcMem_, char* dstMem_ ) : srcMem( srcMem_ ), dstMem( dstMem_ )
    {
    }

    char* dstMem;
    char* srcMem;

    static size_t SizeOf()
    {
        return sizeof( Foo );
    }

    void decompress()
    {
        ImitateCleverDecompressionAlgorithm( dstMem, srcMem, SizeOf() );
    }
};

struct CompressedFoo
{

    explicit CompressedFoo( char* srcMem_ ) : srcMem( srcMem_ )
    {
    }

    char* srcMem;

    static size_t SizeOf()
    {
        return sizeof( Foo );
    }

    void* GetPtr()
    {
        return srcMem;
    }
};

template<typename TCompressedType>
struct UniversalDecompressor
{
    UniversalDecompressor( TCompressedType& compressedObject_, char* dstMem_ )
        : compressedObject( compressedObject_ ), dstMem( dstMem_ )
    {
    }

    using CompressedType = TCompressedType;
    TCompressedType& compressedObject;
    char* dstMem;

    void decompress()
    {
        //void* srcMem = reinterpret_cast<void*>(&compressedObject);
        void* srcMem = compressedObject.GetPtr();
        ImitateCleverDecompressionAlgorithm( dstMem, srcMem, sizeof( TCompressedType ) );
    }
};

struct NoDecompressor
{
};
struct NoneType
{
};

using Reader1TList1 = TypeList<int, char, Foo>;

using Reader2TList1 = TypeList<Foo, Foo>;
using Reader2TList2 = TypeList<NoneType, CompressedFooSelfDecompressable>;
using Reader2TList3 = TypeList<UniversalDecompressor<CompressedFoo>, NoDecompressor>;

template<typename CallableType, CallableType ct, typename... Args>
class Functor
{
 public:
    using FunType = CallableType;
    using t = CallableType;
    using RetType = typename std::result_of<CallableType( Args... )>::type;

    auto operator()( Args... args )->RetType
    {
        return ( *f )( args... );
    }

 private:
    const FunType f = ct;
};

template<typename CallableType, typename... Args>
class NiceFunctor
{
 public:
    using FunType = CallableType;
    using t = CallableType;
    using RetType = typename std::result_of<CallableType( Args... )>::type;

    NiceFunctor( const FunType& f_ ) : f( f_ )
    {

    }

    auto operator()( Args... args )->RetType
    {
        return ( *f )( args... );
    }

 private:
    const FunType& f;
};

template<typename CallableEntity, typename... Args>
NiceFunctor<CallableEntity, Args...> make_functor( CallableEntity ce, Args... args )
{
    return NiceFunctor<CallableEntity, Args...>( ce );
}

NiceFunctor<decltype( &ImitateCleverPostDecompressionAlgorithm ), void*, void*, size_t>
    f( ImitateCleverPostDecompressionAlgorithm );
//NiceFunctor<decltype(&ImitateCleverPostDecompressionAlgorithm)> f2(ImitateCleverPostDecompressionAlgorithm);
//auto f2 = make_functor<decltype(&ImitateCleverPostDecompressionAlgorithm)>(ImitateCleverPostDecompressionAlgorithm);
Functor<decltype( &ImitateCleverPostDecompressionAlgorithm ),
        ImitateCleverPostDecompressionAlgorithm,
        void*,
        void*,
        size_t> f3;

using Reader3TList1 = TypeList<Foo, Foo, Foo, Foo>;
using Reader3TList2 = TypeList<NoneType, CompressedFooSelfDecompressable, CompressedFooSelfDecompressable, NoneType>;
//using Reader3TList3 = TypeList<Functor<void, void*, void*, size_t, (&ImitateCleverPostDecompressionAlgorithm)>, NoDecompressor>;
using Reader3TList3 = TypeList<
    Functor<decltype( &ImitateCleverPostDecompressionAlgorithm ),
            ImitateCleverPostDecompressionAlgorithm,
            void*,
            void*,
            size_t>,
    NoDecompressor>;

template<typename RetType_, typename... Args>
class SimpleFunctor
{
 public:
    using RetType = RetType_;
    using FunType = RetType ( * )( Args... );

    SimpleFunctor( FunType f_ ) : f( f_ )
    {
    }

    RetType operator()( Args... args )
    {
        return ( *f )( args... );
    }

 private:
    const FunType f;
};

using DecompressorFunctionPtr = void(*)(void*, void*, size_t);
using DecompressorFunctionPtrArr = DecompressorFunctionPtr[];
//using DecompressorFunctionPtrArr = DecompressorFunctionPtr[];

DecompressorFunctionPtrArr Reader3Arr3 = {
    &ImitateCleverPostDecompressionAlgorithm,
    &ImitateCleverPostDecompressionAlgorithm,
    nullptr,
    nullptr
};

//ListOfDecompressorFunctors Reader3List3 = {
//    SimpleFunctor<void, void*, void*, size_t>( ImitateCleverPostDecompressionAlgorithm ),
//    SimpleFunctor<void, void*, void*, size_t>( &ImitateCleverPostDecompressionAlgorithm ),
//    nullptr
//};

