
#include "entity.hpp"



namespace tris
{


entity::~entity()
{}


void entity::update(engine* eng)
{}
void entity::on_added(engine* eng)
{}
void entity::on_removed(engine* eng)
{
    delete this;
}



}


