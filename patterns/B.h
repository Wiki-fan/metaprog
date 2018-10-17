#pragma once
#include <iostream>
#include "BaseC.h"

class IB
{
 public:
    virtual void Do() = 0;
};

class B : public IB
{
 public:
    B( int num_, C<Role::Mediator>* atc_ );

    void Do() override;

    bool RequestLanding();

    bool ReportLandedSuccessfully();

 private:
    int number; // Бортовой номер самолёта.
    C<Role::Mediator>* atc;
};
