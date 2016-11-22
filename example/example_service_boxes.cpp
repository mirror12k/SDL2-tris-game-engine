
#include "example_service_boxes.hpp"


/*
    an example of a globally available service entity which is used by other entities to balance themselves

    box_counter_service keeps a record of how many boxes exist on the field and the boxes can modulate their numbers based on this record
*/


class box_counter_service : public tris::service_entity
{
public:
    int box_count = 0;
    int box_limit;
    box_counter_service(int box_limit) : tris::service_entity("box_counter_service"),
    box_limit(box_limit)
    {}

    void update(tris::engine* eng);

    void add_box()
    {
        this->box_count++;
    }
    void remove_box()
    {
        this->box_count--;
    }

    bool is_over_limit()
    {
        return this->box_count > this->box_limit;
    }

    int how_much_over_limit()
    {
        return this->box_count - this->box_limit;
    }
};

class cloning_box : public tris::entity
{
public:
    tris::box my_box;

    int sx = 0, sy = 0, sa = 0;

    int age = 0;

    cloning_box(int x, int y, tris::color rgba) :
    my_box(50, 50, x, y, rgba)
    {}

    virtual void update (tris::engine* eng)
    {

        this->sx = (this->sx - 1 + rand() % 3) % 10;
        this->sy = (this->sy - 1 + rand() % 3) % 10;
        this->sa = (this->sa - 1 + rand() % 3) % 10;

        this->my_box.rect.x += this->sx;
        if (this->my_box.rect.x + 25 >= eng->graphics.window_width || this->my_box.rect.x + 25 < 0)
        {
            this->sx = -this->sx;
            if (this->my_box.rect.x + 25 >= eng->graphics.window_width)
                this->my_box.rect.x = eng->graphics.window_width - 24;
            else
                this->my_box.rect.x = -25;
        }
        this->my_box.rect.y += this->sy;
        if (this->my_box.rect.y + 25 >= eng->graphics.window_height || this->my_box.rect.y + 25 < 0)
        {
            this->sy = -this->sy;
            if (this->my_box.rect.y + 25 >= eng->graphics.window_height)
                this->my_box.rect.y = eng->graphics.window_height - 24;
            else
                this->my_box.rect.y = -25;
        }
        this->my_box.angle += this->sa;


        box_counter_service* srv = eng->update_ctx.get_service<box_counter_service>("box_counter_service");


        this->age++;

        if (this->age > (256 + this->my_box.rgba.g + this->my_box.rgba.b) && rand() % 5 == 0)
            eng->update_ctx.remove_entity(this);
        else if ((rand() % std::max(256 * 2 - this->my_box.rgba.b - this->my_box.rgba.g - this->my_box.rgba.r, 1)) == 0)
            eng->update_ctx.remove_entity(this);
        else if (srv->is_over_limit() && (rand() % std::max((512 + this->my_box.rgba.b + this->my_box.rgba.r - this->my_box.rgba.g) / srv->how_much_over_limit(), 1)) == 0)
            eng->update_ctx.remove_entity(this);

        else
        {
            for (int i = 0; i < 4; i++)
                if (rand() % (256 * 3 - this->my_box.rgba.r - this->my_box.rgba.g) == 0)
                {
                int mutate_chance = 8;
                tris::color rgba = this->my_box.rgba;
                rgba.r += -mutate_chance + rand() % (mutate_chance * 2 + 1);
                rgba.g += -mutate_chance + rand() % (mutate_chance * 2 + 1);
                rgba.b += -mutate_chance + rand() % (mutate_chance * 2 + 1);
                eng->update_ctx.add_entity(new cloning_box(this->my_box.rect.x, this->my_box.rect.y, rgba));

                if (rand() % (512 - this->my_box.rgba.r) == 0)
                {
                    tris::color rgba = this->my_box.rgba;
                    rgba.r += -mutate_chance + rand() % (mutate_chance * 2 + 1);
                    rgba.g += -mutate_chance + rand() % (mutate_chance * 2 + 1);
                    rgba.b += -mutate_chance + rand() % (mutate_chance * 2 + 1);
                    eng->update_ctx.add_entity(new cloning_box(this->my_box.rect.x, this->my_box.rect.y, rgba));
                }
            }
        }
    }

    virtual void on_added(tris::engine* eng)
    {
        eng->graphics.add_box(&this->my_box);
        box_counter_service* srv = eng->update_ctx.get_service<box_counter_service>("box_counter_service");
        srv->add_box();
    }

    virtual void on_removed(tris::engine* eng)
    {
        eng->graphics.remove_box(&this->my_box);
        box_counter_service* srv = eng->update_ctx.get_service<box_counter_service>("box_counter_service");
        srv->remove_box();
    }
};



void box_counter_service::update(tris::engine* eng)
{
    if (this->box_count == 0)
        for (int i = 0; i < 10; i++)
            eng->update_ctx.add_entity(new cloning_box(0, 0, tris::color(rand() % 255, rand() % 255, rand() % 255, 255)));
}

void example_service_boxes ()
{
    srand(time(nullptr));
    tris::engine my_game("service boxes example", 800, 640, 60);
    my_game.update_ctx.add_entity(new tris::util::fps_ticker_entity());
    my_game.update_ctx.add_entity(new box_counter_service(200));
    my_game.run();
}
