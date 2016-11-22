
#pragma once


#include <string>
using std::string;

#include "entity.hpp"

namespace tris
{

class service_entity : public entity
{
public:
    const string service_name;

    service_entity(const string& service_name);
    virtual void on_added(engine* eng);
    virtual void on_removed(engine* eng);
};



}


