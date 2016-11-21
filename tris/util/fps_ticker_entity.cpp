
#include "fps_ticker_entity.hpp"




namespace tris
{
namespace util
{


void fps_ticker_entity::update(tris::engine* eng)
{
    if (this->tick == time(nullptr))
        this->frame++;
    else
    {
        printf("fps: %d\n", this->frame);
        this->tick = time(nullptr);
        this->frame = 0;
    }
}

}
}
