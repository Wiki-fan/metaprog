#include "B.h"
#include "C.h"


void B::Do()
{
    std::cout << "Do" << std::endl;
}

B::B( int num_, C<Role::Mediator>* atc_ ) : number( num_ ), atc( atc_ )
{
    atc->addPlane( this, num_ );
}

bool B::RequestLanding()
{
    if ( !atc->AcceptLandingRequest()) {
        std::cout << "Runway busy" << std::endl;
        return false;
    }
    std::cout << "Landing" << std::endl;
    return true;
}

bool B::ReportLandedSuccessfully()
{
    if ( atc->AcceptLandingReport()) {
        std::cout << "Landed successfully" << std::endl;
        return true;
    }
    std::cout << "Landed unsuccessfully" << std::endl;
    return false;
}
