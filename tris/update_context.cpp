
#include "update_context.hpp"


#include <algorithm>


namespace tris
{


update_context::update_context(engine* parent_engine)
: parent_engine(parent_engine)
{}




void update_context::update()
{
    this->perform_removals();
    for (auto iter = this->entities.begin(); iter != this->entities.end(); iter++)
        (*iter)->update(this->parent_engine);
    this->perform_additions();
}

void update_context::perform_additions()
{
    for (auto iter = this->entities_to_add.begin(), iter_end = this->entities_to_add.end(); iter != iter_end; iter++)
        this->entities.push_back(*iter);
    for (auto iter = this->entities_to_add.begin(), iter_end = this->entities_to_add.end(); iter != iter_end; iter++)
        (*iter)->on_added(this->parent_engine);

    this->entities_to_add.clear();
}
void update_context::perform_removals()
{
    for (auto removal_iter = this->entities_to_remove.begin(), iter_end = this->entities_to_remove.end(); removal_iter != iter_end; removal_iter++)
    {
        auto it = std::find(this->entities.begin(), this->entities.end(), *removal_iter);

        if (it != this->entities.end())
        {
            std::swap(*it, this->entities.back());
            this->entities.pop_back();
        }
    }

    for (auto removal_iter = this->entities_to_remove.begin(), iter_end = this->entities_to_remove.end(); removal_iter != iter_end; removal_iter++)
        (*removal_iter)->on_removed(this->parent_engine);

//    for (auto removal_iter = this->entities_to_remove.begin(), iter_end = this->entities_to_remove.end(); removal_iter != iter_end; removal_iter++)
//        delete *removal_iter;

    this->entities_to_remove.clear();
}

void update_context::add_entity(entity* ent)
{
    this->entities_to_add.push_back(ent);
}
void update_context::remove_entity(entity* ent)
{
    this->entities_to_remove.push_back(ent);
}


void update_context::add_service(const string& service_name, service_entity* ent)
{
    this->services[service_name] = ent;
}
void update_context::remove_service(const string& service_name, service_entity* ent)
{
    this->services.erase(service_name);
}


}


