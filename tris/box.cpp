
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

void box::render(SDL_Renderer* renderer)
{
    this->tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->rect.w, this->rect.h);
    SDL_SetRenderTarget(renderer, this->tex);
    SDL_SetRenderDrawColor(renderer, this->rgba.r, this->rgba.g, this->rgba.b, this->rgba.a);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_RenderFillRect(renderer, nullptr);

    if (this->rgba.a == 255)
        SDL_SetTextureBlendMode(this->tex, SDL_BLENDMODE_NONE);
    else
        SDL_SetTextureBlendMode(this->tex, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(renderer, nullptr);
}



}


