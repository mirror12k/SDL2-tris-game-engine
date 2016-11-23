
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
        this->tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            this->rect.w, this->rect.h);
        SDL_SetRenderTarget(renderer, this->tex);
        SDL_SetTextureBlendMode(this->tex, SDL_BLENDMODE_BLEND);

        SDL_Rect dest;
        dest.w = field_tile_size;
        dest.h = field_tile_size;
        for (uint y = 0; y < this->field_size_y; y++)
            for (uint x = 0; x < this->field_size_x; x++)
            {
                dest.x = x * field_tile_size;
                dest.y = y * field_tile_size;
                SDL_SetRenderDrawColor(renderer, this->rgba.r, this->rgba.g, this->rgba.b,
                    (128 * this->field_energy[this->field_size_x * y + x]) / this->max_field_energy);
                SDL_RenderFillRect(renderer, &dest);
            }

        SDL_SetRenderTarget(renderer, nullptr);
    }
};

class life_energy_field : public tris::service_entity
{
public:
    energy_field_display_box field_display;

    uint* field_energy;
    uint field_size_x, field_size_y;
    uint max_tile_energy;

    life_energy_field(uint x, uint y, uint max_tile_energy)
        : tris::service_entity("life_energy_field"),
        field_display(x, y, max_tile_energy),
        field_energy(new uint[x * y]),
        field_size_x(x), field_size_y(y),
        max_tile_energy(max_tile_energy)
    {
        this->field_display.field_energy = this->field_energy;
        uint* it_end = this->field_energy;
        it_end += this->field_size_x * this->field_size_y;
        for (uint* it = this->field_energy; it != it_end; it++)
            *it = this->max_tile_energy / 2;
    }

    void on_added(tris::engine* eng)
    {
        tris::service_entity::on_added(eng);
        eng->graphics.add_box(&this->field_display);
    }

    void on_removed(tris::engine* eng)
    {
        tris::service_entity::on_removed(eng);
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

    void add_tile_energy(uint x, uint y, int amount)
    {
        this->field_energy[x / field_tile_size + (y / field_tile_size) * this->field_size_x] += amount;
    }

    void step_tile_energy()
    {
        uint* it_end = this->field_energy;
        it_end += this->field_size_x * this->field_size_y;
        for (uint* it = this->field_energy; it != it_end; it++)
            if ((*it += rand() % (4 + *it / 32)) > this->max_tile_energy)
                *it -= rand() % (4 + *it / 16);
    }
};


class life_box : public tris::entity
{
public:
    tris::box my_box;

    int sx = 0, sy = 0;

    int age = 0;
    int max_age;
    int energy;

    life_box(int x, int y, tris::color rgba, int energy)
    : my_box(8, 8, x, y, rgba), energy(energy)
    {
        this->max_age = 256 * 2 - rgba.r - rgba.g - rgba.b;
    }

    void on_added(tris::engine* eng)
    {
        eng->graphics.add_box(&this->my_box);

        tris::subscription_service_entity<life_box>* srv =
            eng->update_ctx.get_service<tris::subscription_service_entity<life_box>>("life_subscriber_service");
        srv->subscribe(this);
    }
    void on_removed(tris::engine* eng)
    {
        eng->graphics.remove_box(&this->my_box);

        tris::subscription_service_entity<life_box>* srv =
            eng->update_ctx.get_service<tris::subscription_service_entity<life_box>>("life_subscriber_service");
        srv->unsubscribe(this);

        tris::entity::on_removed(eng);
    }

    void update(tris::engine* eng)
    {
        this->my_box.rect.x += this->sx;
        if (this->my_box.rect.x + 5 >= eng->graphics.window_width || this->my_box.rect.x + 5 < 0)
        {
//            this->my_box.rect.x += eng->graphics.window_width;
//            this->my_box.rect.x %= eng->graphics.window_width;
//            this->sx = -this->sx;
            if (this->my_box.rect.x + 5 >= eng->graphics.window_width)
                this->my_box.rect.x = -4;
            else
                this->my_box.rect.x = eng->graphics.window_width - 6;
        }
        this->my_box.rect.y += this->sy;
        if (this->my_box.rect.y + 5 >= eng->graphics.window_height || this->my_box.rect.y + 5 < 0)
        {
            this->my_box.rect.y += eng->graphics.window_height;
            this->my_box.rect.y %= eng->graphics.window_height;
//            this->sy = -this->sy;
            if (this->my_box.rect.y + 5 >= eng->graphics.window_height)
                this->my_box.rect.y = -4;
            else
                this->my_box.rect.y = eng->graphics.window_height - 6;
        }
    }


    life_box* clone()
    {
        tris::color rgba = this->my_box.rgba;
        int mutate_chance = 3;
        int r = rgba.r -mutate_chance + rand() % (mutate_chance * 2 + 1);
        int g = rgba.g -mutate_chance + rand() % (mutate_chance * 2 + 1);
        int b = rgba.b -mutate_chance + rand() % (mutate_chance * 2 + 1);
        r = std::max(std::min(r, 255), 0);
        g = std::max(std::min(g, 255), 0);
        b = std::max(std::min(b, 255), 0);
        return new life_box(this->my_box.rect.x, this->my_box.rect.y, tris::color(r,g,b,255), this->energy);
    }

};

class life_subscriber_service : public tris::subscription_service_entity<life_box>
{
public:
    life_subscriber_service()
    : tris::subscription_service_entity<life_box>("life_subscriber_service")
    {}

    void update(tris::engine* eng)
    {
        life_energy_field* srv = eng->update_ctx.get_service<life_energy_field>("life_energy_field");
        for (auto it = this->subscribed_entities.begin(), it_end = this->subscribed_entities.end(); it != it_end; it++)
        {
            life_box* ent = *it;
            // increment age, check if its time to die of age
            if (ent->age++ > ent->max_age && rand() % 25 == 0)
                eng->update_ctx.remove_entity(ent);
            // decrease energy as living cost, check if its out of energy
            else if ((ent->energy -= 10 + ent->my_box.rgba.g / 64 + ent->my_box.rgba.b / 32 + ent->my_box.rgba.r / 64) < 0)
                eng->update_ctx.remove_entity(ent);
            else
            {
                if (rand() % 1024 < ent->my_box.rgba.b)
                {
//                float rand_sx = ent->sx - 1 + rand() % 3;
//                float rand_sy = ent->sy - 1 + rand() % 3;

                float boid_sx = 0;
                float boid_sy = 0;
                float boid_sum = 0;

                for (auto it2 = this->subscribed_entities.begin(), it2_end = this->subscribed_entities.end(); it2 != it2_end; it2++)
                {
                    life_box* other = *it2;
                    int dist = std::abs(other->my_box.rect.x - ent->my_box.rect.x + other->my_box.rect.y - ent->my_box.rect.y);
                    if (dist < 32 && rand() % (1 + std::abs(ent->my_box.rgba.r - other->my_box.rgba.r) +
                            std::abs(ent->my_box.rgba.g - other->my_box.rgba.g) + std::abs(ent->my_box.rgba.b - other->my_box.rgba.b)) < 32)
                    {
                        float boid_dist = 1.0 / (1 + dist);
                        if (boid_dist == 0)
                        {
                            printf("debug 0: %d\n", dist);
                            throw tris::generic_exception("got 0");
                        }
                        boid_sx += (float)other->sx / boid_dist;
                        boid_sy += (float)other->sy / boid_dist;
//                        printf("speeda %f %f,%f\n", boid_dist, boid_sx, boid_sy);
                        boid_sum += boid_dist;
                    }
                }

                boid_sx /= boid_sum;
                boid_sy /= boid_sum;

                ent->sx = boid_sx;
                ent->sy = boid_sy;
//                ent->sx = (boid_sx * (ent->my_box.rgba.b / 255.0) + 4 * rand_sx * (1 - ent->my_box.rgba.b / 255.0)) / 5;
//                ent->sy = (boid_sy * (ent->my_box.rgba.b / 255.0) + 4 * rand_sy * (1 - ent->my_box.rgba.b / 255.0)) / 5;
                }
//                printf("speed %d,%d + %d,%d\n", (int)boid_sx, (int)boid_sy, (int)rand_sx, (int)rand_sy);
//                printf("speed %f,%f\n", ent->my_box.rgba.b / 255.0, 1 - ent->my_box.rgba.b / 255.0);
                ent->sx += - 1 + rand() % 3;
                ent->sy += - 1 + rand() % 3;

                int speed_limit = 2 + ent->my_box.rgba.g / 64;
                float speed = pow(pow(ent->sx, 2) + pow(ent->sy, 2), 0.5);
                if (speed > speed_limit)
                {
//                    printf("pre speed: %f, %d,%d\n", speed, ent->sx, ent->sy);
                    ent->sx /= speed / (float)speed_limit;
                    ent->sy /= speed / (float)speed_limit;
//                    printf("resulting speed: %f, %d,%d\n", speed / (float)speed_limit, ent->sx, ent->sy);
                }
//                ent->sx = std::min(std::max(ent->sx, -speed_limit), speed_limit);
//                ent->sy = std::min(std::max(ent->sy, -speed_limit), speed_limit);

                // consume energy from the tile
                int energy = srv->get_tile_energy(ent->my_box.rect.x + 5, ent->my_box.rect.y + 5);
                int consumed = energy / (16 + rand() % 7 + ent->my_box.rgba.b / 32 - ent->my_box.rgba.r / 32 - ent->my_box.rgba.g / 64);
                ent->energy += consumed * ent->my_box.rgba.b / 64 + ent->my_box.rgba.g / 32;
//                consumed -= rand() % (1 + ent->my_box.rgba.r / 8);
                srv->add_tile_energy(ent->my_box.rect.x + 5, ent->my_box.rect.y + 5, -consumed);

                // check if it needs to multiply due to having enough energy
                if (ent->energy > 512 - ent->my_box.rgba.r + ent->my_box.rgba.b * 2 + ent->my_box.rgba.g * 2 && rand() % (4 + ent->my_box.rgba.b / 8) == 0)
                {
                    life_box* clone = ent->clone();
                    clone->energy /= 16 + ent->my_box.rgba.g / 64 - ent->my_box.rgba.r / 32;
//                    clone->energy += ent->my_box.rgba.r * 2;
                    ent->energy /= 16 + ent->my_box.rgba.g / 64 - ent->my_box.rgba.r / 32;
                    ent->energy += ent->my_box.rgba.r;
                    eng->update_ctx.add_entity(clone);

                    for (int i = 0; i < 2; i++)
                        if (rand() % 256 < ent->my_box.rgba.r)
                        {
                            life_box* clone = ent->clone();
                            clone->energy /= 16 + ent->my_box.rgba.g / 64 - ent->my_box.rgba.r / 32;
//                            clone->energy += ent->my_box.rgba.r * 2;
//                            ent->energy /= 16 + ent->my_box.rgba.g / 64 - ent->my_box.rgba.r / 64;
                            eng->update_ctx.add_entity(clone);
                        }
                }
            }
        }
    }
};


void example_subscribing_boxes()
{
    srand(time(nullptr));
    tris::engine my_game("subscribing boxes example", 800, 640, 60);
    my_game.update_ctx.add_entity(new tris::util::fps_ticker_entity());
    my_game.update_ctx.add_entity(new life_energy_field(800 / field_tile_size, 640 / field_tile_size, 1024));
    my_game.update_ctx.add_entity(new life_subscriber_service());
    for (int i = 0; i < 100; i++)
        my_game.update_ctx.add_entity(new life_box(rand() % (800 - 10), rand() % (640 - 10), tris::color(rand() % 256,
            rand() % 256, rand() % 256, 255), 20));
    my_game.run();
}
