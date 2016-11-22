
#include "example_subscribing_boxes.hpp"




const uint field_tile_size = 16;



class energy_field_display_box : public tris::box
{
public:
    // same as its parent energy_field's field_energy
    uint field_size_x, field_size_y;
    uint* field_energy = nullptr;
    uint max_field_energy;

    energy_field_display_box(int w, int h, uint max_field_energy)
    : tris::box(w * field_tile_size, h * field_tile_size, 0, 0, tris::color(128, 255, 128, 255)),
    field_size_x(w), field_size_y(h), max_field_energy(max_field_energy)
    {}

    void render(SDL_Renderer* renderer)
    {
        this->tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->rect.w, this->rect.h);
        SDL_SetRenderTarget(renderer, this->tex);
        SDL_SetTextureBlendMode(this->tex, SDL_BLENDMODE_BLEND);

        SDL_Rect dest;
        dest.w = field_tile_size;
        dest.h = field_tile_size;
        for (uint y = 0; y < this->field_size_y; y++)
            for (uint x = 0; x < this->field_size_x; x++)
            {
//                printf("debug field_energy %d\n", (255 * this->field_energy[this->field_size_x * y + x]) / this->max_field_energy);
                dest.x = x * field_tile_size;
                dest.y = y * field_tile_size;
                SDL_SetRenderDrawColor(renderer, this->rgba.r, this->rgba.g, this->rgba.b,
                    (128 * this->field_energy[this->field_size_x * y + x]) / this->max_field_energy);
                SDL_RenderFillRect(renderer, &dest);
            }

        SDL_SetRenderTarget(renderer, nullptr);
    }
};

class energy_field : public tris::entity
{
public:
    energy_field_display_box field_display;

    uint* field_energy;
    uint field_size_x, field_size_y;
    uint max_tile_energy;

    energy_field(uint x, uint y, uint max_tile_energy)
    : max_tile_energy(max_tile_energy), field_energy(new uint[x * y]), field_display(x, y, max_tile_energy),
    field_size_x(x), field_size_y(y)
    {
        this->field_display.field_energy = this->field_energy;
        printf("debug field_energy %x\n", this->field_energy);
        uint* it_end = this->field_energy;
        it_end += this->field_size_x * this->field_size_y;
        for (uint* it = this->field_energy; it != it_end; it++)
            *it = 0;
    }

    void on_added(tris::engine* eng)
    {
        eng->graphics.add_box(&this->field_display);
    }

    void on_removed(tris::engine* eng)
    {
        eng->graphics.remove_box(&this->field_display);
    }

    void update(tris::engine* eng)
    {
        this->step_tile_energy();
        this->field_display.changed = true;
    }

    uint get_tile_energy(uint x, uint y)
    {
        return this->field_energy[x / field_tile_size + (y / field_tile_size) * this->field_size_x];
    }

    void add_tile_energy(uint x, uint y, uint amount)
    {
        this->field_energy[x / field_tile_size + (y / field_tile_size) * this->field_size_x] += amount;
    }

    void step_tile_energy()
    {
        uint* it_end = this->field_energy;
        it_end += this->field_size_x * this->field_size_y;
        for (uint* it = this->field_energy; it != it_end; it++)
            if ((*it += rand() % (2 + *it / 16)) > this->max_tile_energy)
                *it = this->max_tile_energy;
    }
};


void example_subscribing_boxes()
{
    srand(time(nullptr));
    tris::engine my_game("subscribing boxes example", 800, 640, 60);
    my_game.update_ctx.add_entity(new tris::util::fps_ticker_entity());
    my_game.update_ctx.add_entity(new energy_field(800 / field_tile_size, 640 / field_tile_size, 1024));
    my_game.run();
}
