#include "A.h"
#include "C.h"

void A::DoSomethingWithB( IB& b )
{
    std::cout << "A does ";
    b.Do();
}

void A::Update( IB& b )
{
    std::cout << "A observed B and noticed change" <<std::endl;
}

void A::OrderLanding( C<Role::Mediator>* atc, int number )
{
    atc->OrderLanding(number);
}
