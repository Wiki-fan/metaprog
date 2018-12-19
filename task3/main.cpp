#include <iostream>
#include <fstream>
#include "typelists-m.h"
#include "TestData.h"
#include "Reader1.h"
#include "Reader2.h"
#include "Reader3.h"


void TestInMemSize()
{
    static_assert( GetInMemSize<EmptyList>::value == 0 );
    static_assert( GetInMemSize<TypeList<int>>::value == 4 );
    static_assert( GetInMemSize<TypeList<int, char>>::value == 5 );
    static_assert( GetInMemSize<TypeList<int, char, Foo>>::value == 13 );
}

void TestReader1()
{
    Reader1<Reader1TList1> reader( "test1.txt" );
    char* mem = static_cast<char*>(reader.ReadNextLine());
    int a = *reinterpret_cast<int*>(mem);
    assert( a == 13 );
    char c = *reinterpret_cast<char*>(mem + 4);
    assert( c == 'g' );
    Foo foo = *reinterpret_cast<Foo*>(mem + 5);
    assert( foo.a == 12 );
    assert( foo.c == 'a' );

    delete mem;
}

void TestReader2()
{
    static_assert( sizeof( CompressedFooSelfDecompressable ) == sizeof( Foo ) + sizeof( void* ) );

    std::ofstream ofs( "test2.bin" );
    Foo foo( 13, 'a' );
    ofs.write( reinterpret_cast<char*>(&foo), sizeof( Foo ) );
    ofs.write( reinterpret_cast<char*>(&foo), sizeof( Foo ) );
    ofs.close();

    Reader2<Reader2TList1, Reader2TList2, Reader2TList3> reader( "test2.bin" );
    char* mem = static_cast<char*>(reader.ReadNextLine());
    Foo foo1 = *reinterpret_cast<Foo*>(mem);
    Foo foo2 = *reinterpret_cast<Foo*>(mem + sizeof( Foo ));
    assert( foo1.a == foo.a );
    assert( foo1.c == foo.c );
    assert( foo2.a == foo.a );
    assert( foo2.c == foo.c );

    delete mem;
}

void TestReader3()
{
    DecompressorFunctionPtr* a = Reader3Arr3 + 4;
    std::ofstream ofs( "test3.bin" );
    Foo foo( 13, 'a' );
    for( int i = 0; i < 4; ++i ) {
        ofs.write( reinterpret_cast<char*>(&foo), sizeof( Foo ) );
    }
    ofs.close();

    Reader3<Reader3TList1, Reader3TList2, Reader3Arr3> reader( "test3.bin" );
    char* mem = static_cast<char*>(reader.ReadNextLine());

    for( int i = 0; i < 4; ++i ) {
        Foo foo2 = *reinterpret_cast<Foo*>(mem + sizeof( Foo ) * i);
        assert( foo2.a == foo.a );
        assert( foo2.c == foo.c );
    }

    delete mem;
}

int main()
{
    std::cout << "Hello, World!" << std::endl;

    TestInMemSize();
    TestReader1();
    TestReader2();
    TestReader3();

    return 0;
}
