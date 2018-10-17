#pragma once

enum class Role
{
    Proxy, Mediator, Observer
};

template<Role>
class C
{

};
