
#include "graphics_context.hpp"


#include <algorithm>


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



void graphics_context::prepare_boxes()
{
    for (vector<box*>::iterator iter = this->boxes.begin(), iter_end = this->boxes.end(); iter != iter_end; iter++)
        if ((*iter)->tex == nullptr || (*iter)->changed)
            this->load_box(*iter);
}

void graphics_context::render()
{
    for (vector<box*>::iterator iter = this->boxes.begin(), iter_end = this->boxes.end(); iter != iter_end; iter++)
        this->render_box(*iter);
}

void graphics_context::load_box(box* p_box)
{
    if (p_box->changed)
    {
        p_box->changed = false;
        if (p_box->tex != nullptr)
        {
            SDL_DestroyTexture(p_box->tex);
            p_box->tex = nullptr;
        }
    }
    p_box->render(this->renderer);
}

void graphics_context::render_box(box* p_box)
{
    SDL_RenderCopyEx(this->renderer, p_box->tex, nullptr, &p_box->rect, p_box->angle, nullptr, SDL_FLIP_NONE);
}


void graphics_context::add_box(box* p_box)
{
    this->boxes.push_back(p_box);
}

void graphics_context::remove_box(box* p_box)
{
    auto it = std::find(this->boxes.begin(), this->boxes.end(), p_box);

    if (it != this->boxes.end())
    {
        std::swap(*it, this->boxes.back());
        this->boxes.pop_back();
    }
    else
        throw graphics_exception("invalid box pointer given to graphics_context::remove_box");
}


}

