
#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace tris
{

class box
{
public:
    SDL_Rect rect;
    uint8_t r,g,b,a;
    double angle = 0;

    SDL_Texture* tex = nullptr;

    box(int w, int h, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    ~box();
};

}


