
#include "box.hpp"


namespace tris
{


box::box(int w, int h, int x, int y, uint32_t rgba)
{
    this->rect.w = w;
    this->rect.h = h;
    this->rect.x = x;
    this->rect.y = y;
    this->rgba = rgba;
}


}


