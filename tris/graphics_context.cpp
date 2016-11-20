
#include "graphics_context.hpp"


namespace tris
{



graphics_exception::graphics_exception(const string& reason)
: generic_exception(reason)
{}





graphics_context::graphics_context(const string& window_title, int window_width, int window_height)
: window_title(window_title), window_width(window_width), window_height(window_height)
{}

graphics_context::~graphics_context()
{
    if (this->window != nullptr)
        this->end_graphics();
}


void graphics_context::start_graphics()
{
    this->window = SDL_CreateWindow(this->window_title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        this->window_width, this->window_height,
        SDL_WINDOW_OPENGL);


    if (this->window == nullptr)
    {
        throw graphics_exception("failed to create window: "+string(SDL_GetError()));
    }

    this->screen = SDL_GetWindowSurface(this->window);
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
//    SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);


    if (TTF_Init())
    {
        throw graphics_exception("failed to initialize SDL2 TTF: "+string(TTF_GetError()));
    }
}

void graphics_context::end_graphics()
{
    TTF_Quit();

    SDL_DestroyWindow(this->window);
    this->window = nullptr;
    this->screen = nullptr;
    this->renderer = nullptr;
}


void graphics_context::clear()
{
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(this->renderer, NULL);
}

void graphics_context::present()
{
    SDL_RenderPresent(this->renderer);
}



void graphics_context::render()
{
    this->my_box.rect.x += 2;
    this->my_box.rect.y += 1;
    this->my_box.angle += 15;
    this->render_box(&this->my_box);
}


void graphics_context::render_box(box* p_box)
{
    if (p_box->tex == nullptr)
    {
        p_box->tex = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, p_box->rect.w, p_box->rect.h);
        SDL_SetRenderTarget(this->renderer, p_box->tex);
        SDL_SetRenderDrawColor(this->renderer, p_box->r, p_box->g, p_box->b, p_box->a);
        SDL_RenderFillRect(this->renderer, nullptr);

        SDL_SetRenderTarget(this->renderer, nullptr);
    }

    SDL_RenderCopyEx(this->renderer, p_box->tex, nullptr, &p_box->rect, p_box->angle, nullptr, SDL_FLIP_NONE);
}


}

