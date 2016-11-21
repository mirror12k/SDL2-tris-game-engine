
#pragma once


#include <vector>
using std::vector;

#include "entity.hpp"


namespace tris
{



class update_context
{
public:
    vector<entity*> entities;
    vector<entity*> entities_to_add;
    vector<entity*> entities_to_remove;

    engine* parent_engine;

    update_context(engine* parent_engine);

    void update();
    void perform_additions();
    void perform_removals();

    void add_entity(entity* ent);
    void remove_entity(entity* ent);
};



}


