
#pragma once

#include <vector>
using std::vector;


#include "service_entity.hpp"

namespace tris
{

class subscription_service_entity : public service_entity
{
public:
    vector<entity*> subscribed_entities;

    subscription_service_entity(const string& service_name);

    void subscribe(entity* ent);
    void unsubscribe(entity* ent);
};



}


