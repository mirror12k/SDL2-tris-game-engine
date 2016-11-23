
#pragma once

#include <vector>
using std::vector;

#include <algorithm>

#include "exception.hpp"
#include "service_entity.hpp"

namespace tris
{

template<class entity_type>
class subscription_service_entity : public service_entity
{
public:
    vector<entity_type*> subscribed_entities;

    subscription_service_entity(const string& service_name);

    void subscribe(entity_type* ent);
    void unsubscribe(entity_type* ent);
};


template<class entity_type>
subscription_service_entity<entity_type>::subscription_service_entity(const string& service_name)
: service_entity(service_name)
{}

template<class entity_type>
void subscription_service_entity<entity_type>::subscribe(entity_type* ent)
{
    this->subscribed_entities.push_back(ent);
}

template<class entity_type>
void subscription_service_entity<entity_type>::unsubscribe(entity_type* ent)
{
    auto it = std::find(this->subscribed_entities.begin(), this->subscribed_entities.end(), ent);

    if (it != this->subscribed_entities.end())
    {
        std::swap(*it, this->subscribed_entities.back());
        this->subscribed_entities.pop_back();
    }
    else
    {
        throw generic_exception("subscribed entity not found");
    }
}



}


