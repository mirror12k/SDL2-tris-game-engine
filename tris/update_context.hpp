
#pragma once


#include <map>
using std::map;

#include <vector>
using std::vector;

#include "exception.hpp"
#include "entity.hpp"
#include "service_entity.hpp"


namespace tris
{



class update_context
{
public:
    vector<entity*> entities;
    vector<entity*> entities_to_add;
    vector<entity*> entities_to_remove;

    map<string, service_entity*> services;

    engine* parent_engine;

    update_context(engine* parent_engine);

    void update();
    void perform_additions();
    void perform_removals();

    void add_entity(entity* ent);
    void remove_entity(entity* ent);

    template<class service_type>
    service_type* get_service(const string& service_name);
    void add_service(const string& service_name, service_entity* ent);
    void remove_service(const string& service_name, service_entity* ent);
};




template<class service_type>
service_type* update_context::get_service(const string& service_name)
{
    auto it = this->services.find(service_name);
    if (it == this->services.end())
    {
        string service_list = "";
        for (auto it = this->services.begin(); it != this->services.end(); it++)
            service_list += it->first + ",";
        throw generic_exception("request for undefined service '" + service_name + "', available services are: " + service_list);
    }
    else
        return (service_type*)it->second;
}


}


