
#include <stdio.h>


#include "tris/engine.hpp"


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
        if (this->my_box.rect.x >= eng->graphics.window_width || this->my_box.rect.x < 0)
            this->sx = -this->sx;
        this->my_box.rect.y += this->sy;
        if (this->my_box.rect.y >= eng->graphics.window_height || this->my_box.rect.y < 0)
            this->sy = -this->sy;
        this->my_box.angle += 0.5;
    }

    virtual void on_added(tris::engine* eng)
    {
        eng->graphics.add_box(&this->my_box);
    }
};


int main ()
{
    printf("hello world\n");
    tris::engine my_game("test engine", 800, 640, 60);
//    my_game.graphics.add_box(new tris::box(50, 100, 50, 50, 128, 0, 128, 255));
    my_game.update_ctx.add_entity(new my_entity(100, 100, 1, 1));
    my_game.update_ctx.add_entity(new my_entity(100, 100, 1, 2));
    my_game.update_ctx.add_entity(new my_entity(100, 100, 3, 1));
    my_game.run();

    return 0;
}
