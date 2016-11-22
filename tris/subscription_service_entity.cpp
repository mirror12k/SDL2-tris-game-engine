
#include "subscription_service_entity.hpp"

#include <algorithm>

namespace tris
{


subscription_service_entity::subscription_service_entity(const string& service_name)
: service_entity(service_name)
{}

void subscription_service_entity::subscribe(entity* ent)
{
    this->subscribed_entities.push_back(ent);
}
void subscription_service_entity::unsubscribe(entity* ent)
{
    auto it = std::find(this->subscribed_entities.begin(), this->subscribed_entities.end(), ent);

    if (it != this->subscribed_entities.end())
    {
        std::swap(*it, this->subscribed_entities.back());
        this->subscribed_entities.pop_back();
    }
}

}
