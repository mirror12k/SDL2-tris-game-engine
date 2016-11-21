
#include "entity.hpp"



namespace tris
{




void entity::update(engine* eng)
{}
void entity::on_added(engine* eng)
{}
void entity::on_removed(engine* eng)
{
    delete this;
}



}


