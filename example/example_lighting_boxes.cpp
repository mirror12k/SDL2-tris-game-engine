
#include "example_lighting_boxes.hpp"



class sparkle_scale_box : public tris::entity
{
public:
    tris::box my_box;

    int step, step_mod;

    sparkle_scale_box(int x, int y, int step, int step_mod)
    : my_box(32,32,x,y, tris::color(0,0,0,255)), step(step), step_mod(step_mod)
    {}

    void update(tris::engine* eng)
    {
        this->step = (this->step + 1) % 360;
        if (this->step % this->step_mod == 0)
        {
            int val = 128 + 127 * std::cos(pi * this->step/360.0);
            this->my_box.rgba.r = val;
            this->my_box.rgba.g = val;
            this->my_box.rgba.b = val;
            this->my_box.changed = true;
        }
    }

    void on_added(tris::engine* eng)
    {
        eng->graphics.add_box(&this->my_box);
    }

    void on_removed(tris::engine* eng)
    {
        eng->graphics.remove_box(&this->my_box);
        tris::entity::on_removed(eng);
    }
};



class lighting_box : public tris::box
{
public:
    int direction = 1;
    int offset = 0;

    lighting_box(int w, int h)
    : tris::box(w, h, 0, 0, tris::color(0,0,0,255))
    {}

    void render(SDL_Renderer* renderer)
    {
        tris::box::render(renderer);
        SDL_SetRenderTarget(renderer, this->tex);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
        SDL_SetRenderDrawColor(renderer, 128, 128, 64, 255);
        SDL_Rect dest;
        dest.w = 200;
        dest.h = 200;
        dest.x = 100 + this->offset;
        dest.y = 100;
        SDL_RenderFillRect(renderer, &dest);
        SDL_SetRenderDrawColor(renderer, 255, 255, 128, 255);
        dest.w = 160;
        dest.h = 160;
        dest.x = 120 + this->offset;
        dest.y = 120;
        SDL_RenderFillRect(renderer, &dest);
        SDL_SetRenderDrawColor(renderer, 32, 64, 128, 255);
        dest.w = 200;
        dest.h = 200;
        dest.x = 600 - this->offset;
        dest.y = 200;
        SDL_RenderFillRect(renderer, &dest);
        SDL_SetRenderDrawColor(renderer, 64, 128, 255, 255);
        dest.w = 160;
        dest.h = 160;
        dest.x = 620 - this->offset;
        dest.y = 220;
        SDL_RenderFillRect(renderer, &dest);
        SDL_SetTextureBlendMode(this->tex, SDL_BLENDMODE_MOD);
        SDL_SetRenderTarget(renderer, nullptr);
    }
};


class lighting_service : public tris::service_entity
{
public:
    lighting_box my_box;

    lighting_service(int w, int h)
    : tris::service_entity("lighting_service"), my_box(w,h)
    {}

    void update(tris::engine* eng)
    {
        this->my_box.offset += this->my_box.direction;
        if (this->my_box.offset >= 500 || this->my_box.offset <= 0)
            this->my_box.direction = -this->my_box.direction;

        this->my_box.changed = true;
    }

    void on_added(tris::engine* eng)
    {
        eng->graphics.add_box(&this->my_box);
    }

    void on_removed(tris::engine* eng)
    {
        eng->graphics.remove_box(&this->my_box);
        tris::service_entity::on_removed(eng);
    }
};




void example_lighting_boxes()
{
    srand(time(nullptr));
    tris::engine my_game("lighting boxes example", 800, 640, 60);
    my_game.update_ctx.add_entity(new tris::util::fps_ticker_entity());
    for (int x = 0; x < 800; x += 32)
        for (int y = 0; y < 640; y += 32)
//            my_game.update_ctx.add_entity(new sparkle_scale_box(x, y, rand() % 360, 4));
            my_game.update_ctx.add_entity(new sparkle_scale_box(x, y, x / 32 + y / 32 + std::abs(90 * ((x / 32 - y / 32) % 4)), 64));
    my_game.update_ctx.add_entity(new lighting_service(800, 640));
    my_game.run();
}
