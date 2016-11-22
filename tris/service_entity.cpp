
#include "service_entity.hpp"


#include "engine.hpp"

namespace tris
{



service_entity::service_entity(const string& service_name)
: service_name(service_name)
{}

void service_entity::on_added(engine* eng)
{
    eng->update_ctx.add_service(service_name, this);
}

void service_entity::on_removed(engine* eng)
{
    eng->update_ctx.remove_service(service_name, this);
}


}
