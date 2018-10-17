#include "C.h"

C<Role::Proxy>::C( int num_, C<Role::Mediator>* atc_ ) : b(new B(num_, atc_))
{
}

void C<Role::Proxy>::Do()
{
    std::cout << "(proxy)";
    b->Do();
}

bool C<Role::Mediator>::OrderLanding( int number )
{
    return planes[number]->RequestLanding();
}
bool C<Role::Mediator>::AcceptLandingRequest()
{
    if (runwayClear) {
        runwayClear = false;
        return true;
    } else {
        return false;
    }
}

bool C<Role::Mediator>::AcceptLandingReport()
{
    runwayClear = true;
    return true;
}

void C<Role::Mediator>::addPlane( B* b, int index )
{
    planes[index] = b;
}

void C<Role::Observer>::attachAtoB( A* a, B* b )
{
    auto pair = pairs.find( b );
    if ( pair != pairs.end()) {
        pair->second.insert( a );
    } else {
        pairs[b] = std::set<A*>( {a} );
    }
}

void C<Role::Observer>::detachAfromB( A* a, B* b )
{
    pairs[b].erase( a );
}

void C<Role::Observer>::notify( B* b )
{
    for ( auto& a: pairs[b] ) {
        a->Update( *b );
    }
}
