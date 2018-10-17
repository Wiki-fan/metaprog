#pragma once
#include <iostream>
#include "B.h"

class IA {

};

class A : public IA {
 public:
    void DoSomethingWithB(IB& b);

    void Update(IB& b);

    void OrderLanding(C<Role::Mediator>* atc, int number);
};
