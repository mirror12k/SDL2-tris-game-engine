
#include "box.hpp"


namespace tris
{


color::color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}


box::box(int w, int h, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
: rgba(r,g,b,a)
{
    this->rect.w = w;
    this->rect.h = h;
    this->rect.x = x;
    this->rect.y = y;
}

box::box(int w, int h, int x, int y, color rgba)
: rgba(rgba)
{
    this->rect.w = w;
    this->rect.h = h;
    this->rect.x = x;
    this->rect.y = y;
}

box::~box()
{
    if (this->tex != nullptr)
        SDL_DestroyTexture(this->tex);
}



}


