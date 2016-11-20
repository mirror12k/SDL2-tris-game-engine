
#include "box.hpp"


namespace tris
{


box::box(int w, int h, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    this->rect.w = w;
    this->rect.h = h;
    this->rect.x = x;
    this->rect.y = y;
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

box::~box()
{
    if (this->tex != nullptr)
        SDL_DestroyTexture(this->tex);
}



}


