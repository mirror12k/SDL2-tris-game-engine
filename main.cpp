
#include <stdio.h>


#include "tris/engine.hpp"


#include <cmath>
const double pi = std::acos(-1);


class my_entity : public tris::entity
{
public:
    tris::box my_box;

    int sx, sy;

    my_entity(int x, int y, int sx, int sy)
    : my_box(50, 50, x-25, y-25, 128, 255, 128, 255), sx(sx), sy(sy)
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

class my_invis : public my_entity
{
public:
    int step_a;

    my_invis(int x, int y, int sx, int sy, int step_angle, int step_a)
    : my_entity(x, y, sx, sy), step_a(step_a)
    {
        this->my_box.angle = step_angle;
    }

    virtual void update (tris::engine* eng)
    {
        my_entity::update(eng);
        this->my_box.rgba.a = 128 + 127 * std::sin(pi * (this->step_a / 180.0));
        this->my_box.changed = true;

        this->step_a = (this->step_a + 8) % 360;
    }
};

class fps_ticker_entity : public tris::entity
{
public:
    uint tick = 0;
    uint frame = 0;

    virtual void update(tris::engine* eng)
    {
        if (this->tick == time(nullptr))
            this->frame++;
        else
        {
            printf("fps: %d\n", this->frame);
            this->tick = time(nullptr);
            this->frame = 0;
        }
    }
};


int main ()
{

    srand(time(nullptr));
    printf("hello world\n");
    tris::engine my_game("test engine", 800, 640, 60);
//    my_game.graphics.add_box(new tris::box(50, 100, 50, 50, 128, 0, 128, 255));
//    my_game.update_ctx.add_entity(new my_invis(100, 100, 1, 1, 100));
//    my_game.update_ctx.add_entity(new my_invis(100, 100, 1, 2, 0));
//    my_game.update_ctx.add_entity(new my_entity(100, 100, 3, 1));
//    my_game.update_ctx.add_entity(new my_entity(100, 100, 1, 3));
    my_game.update_ctx.add_entity(new fps_ticker_entity());

    for (int i = 0; i < 400; i++)
        my_game.update_ctx.add_entity(new my_invis(rand() % my_game.graphics.window_width, rand() % my_game.graphics.window_height,
                1 + rand() % 5, 1 + rand() % 5, rand() % 360, 1 + rand() % 360));
    my_game.run();

    return 0;
}
