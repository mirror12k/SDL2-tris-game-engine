
#include "example_striped_boxes.hpp"


class striped_box : public tris::box
{
public:
    int stripe_width;
    tris::color stripe_rgba;

    // more arguments for the stripes
    striped_box(int w, int h, int x, int y, tris::color rgba, int stripe_width, tris::color stripe_rgba)
    : box(w,h,x,y,rgba), stripe_width(stripe_width), stripe_rgba(stripe_rgba)
    {}

    // override the render function of tris::box so that we can add stripes
    void render(SDL_Renderer* renderer)
    {
        SDL_Texture* stripe = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->rect.w * 2, this->stripe_width);
        SDL_SetRenderTarget(renderer, stripe);
        SDL_SetRenderDrawColor(renderer, this->stripe_rgba.r, this->stripe_rgba.g, this->stripe_rgba.b, this->stripe_rgba.a);
        SDL_RenderFillRect(renderer, nullptr);
        SDL_SetTextureBlendMode(stripe, SDL_BLENDMODE_NONE);

        this->tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->rect.w, this->rect.h);
        SDL_SetRenderTarget(renderer, this->tex);
        SDL_SetRenderDrawColor(renderer, this->rgba.r, this->rgba.g, this->rgba.b, this->rgba.a);
        SDL_RenderFillRect(renderer, nullptr);

        SDL_Rect dest;
        dest.w = this->rect.w * 2;
        dest.h = this->stripe_width;
        for (int offset = 0; offset < this->rect.h; offset += this->stripe_width * 1.414)
        {
            dest.x = -offset;
            dest.y = offset;
            SDL_RenderCopyEx(renderer, stripe, nullptr, &dest, 45, nullptr, SDL_FLIP_NONE);
        }

        if (this->rgba.a == 255 && this->stripe_rgba.a == 255)
            SDL_SetTextureBlendMode(this->tex, SDL_BLENDMODE_NONE);
        else
            SDL_SetTextureBlendMode(this->tex, SDL_BLENDMODE_BLEND);

        SDL_SetRenderTarget(renderer, nullptr);
    }
};





class striped_entity : public tris::entity
{
public:
    striped_box my_box;

    int sx, sy;

    striped_entity(int x, int y, int sx, int sy, tris::color rgba, int striped_width, tris::color strip_rgba)
    : my_box(200, 200, x-25, y-25, rgba, striped_width, strip_rgba),
    sx(sx), sy(sy)
    {}

    virtual void update (tris::engine* eng)
    {
        this->my_box.rect.x += this->sx;
        if (this->my_box.rect.x + 25 >= eng->graphics.window_width || this->my_box.rect.x + 25 < 0)
            this->sx = -this->sx;
        this->my_box.rect.y += this->sy;
        if (this->my_box.rect.y + 25 >= eng->graphics.window_height || this->my_box.rect.y + 25 < 0)
            this->sy = -this->sy;
        this->my_box.angle += 0.5;
    }

    virtual void on_added(tris::engine* eng)
    {
        eng->graphics.add_box(&this->my_box);
    }
};


void example_striped_boxes()
{
    srand(time(nullptr));
    tris::engine my_game("striped boxes example", 800, 640, 60);
    my_game.update_ctx.add_entity(new tris::util::fps_ticker_entity());
//    my_game.update_ctx.add_entity(new tris::util::frame_benchmark_entity(300));

    for (int i = 0; i < 10; i++)
        my_game.update_ctx.add_entity(new striped_entity(rand() % my_game.graphics.window_width, rand() % my_game.graphics.window_height,
                1 + rand() % 3, 1 + rand() % 3, tris::color(128, 255, 128, 255), 2 + rand() % 40, tris::color(128, 128, 255, 127 + 128 * (rand() % 2))));
    my_game.run();
}

