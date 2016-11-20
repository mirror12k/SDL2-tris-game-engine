
#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace tris
{

class box
{
public:
    SDL_Rect rect;
    union {
        uint32_t rgba;
        uint8_t a,b,g,r;
    };
    double angle = 0;

    SDL_Texture* tex = nullptr;

    box(int w, int h, int x, int y, uint32_t rgba);
    ~box();
};

}

