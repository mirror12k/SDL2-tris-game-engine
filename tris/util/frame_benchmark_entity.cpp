
#include "frame_benchmark_entity.hpp"




namespace tris
{
namespace util
{

frame_benchmark_entity::frame_benchmark_entity(uint required_frames)
: required_frames(required_frames)
{}

void frame_benchmark_entity::update(tris::engine* eng)
{
    this->frame++;
    if (this->frame == this->required_frames)
        eng->set_running(false);
}

}
}
