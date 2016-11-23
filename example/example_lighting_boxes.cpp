
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



void example_lighting_boxes()
{
    srand(time(nullptr));
    tris::engine my_game("lighting boxes example", 800, 640, 60);
    my_game.update_ctx.add_entity(new tris::util::fps_ticker_entity());
    for (int x = 0; x < 800; x += 32)
        for (int y = 0; y < 640; y += 32)
            my_game.update_ctx.add_entity(new sparkle_scale_box(x, y, rand() % 360, 4));
//            my_game.update_ctx.add_entity(new sparkle_scale_box(x, y, x / 32 + (y / 32) * (800 / 32), 16));
//            my_game.update_ctx.add_entity(new sparkle_scale_box(x, y, x / 32 + y / 32, 4));
//            my_game.update_ctx.add_entity(new sparkle_scale_box(x, y, x / 32 + y / 32 + std::abs(90 * ((x / 32 - y / 32) % 4)), 64));
//    my_game.update_ctx.add_entity(new box_counter_service(200));
    my_game.run();
}
