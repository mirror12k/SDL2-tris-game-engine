
#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace tris
{

struct color
{
    uint8_t r,g,b,a;
    color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
};

class box
{
public:
    bool changed = false;

    SDL_Rect rect;
    color rgba;
    double angle = 0;

    SDL_Texture* tex = nullptr;

    box(int w, int h, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    box(int w, int h, int x, int y, color rgba);
    ~box();

    virtual void render(SDL_Renderer* renderer);
};

}


