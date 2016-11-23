
#include "engine.hpp"

#include <ctime>


namespace tris
{



engine::engine(const string& window_title, int window_width, int window_height, uint fps)
:
graphics(window_title, window_width, window_height),
update_ctx(this),
fps(fps)
{
//    this->entity_data = new update_context();
    this->frame_delay = 1000 / fps;
}



void engine::check_input()
{
    const uint8_t* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_ESCAPE])
    {
        this->running = false;
    }
}


void engine::run()
{
    this->graphics.start_graphics();
    this->running = true;

    while (this->running)
    {
        std::clock_t start_frame_tick = clock();

//        printf("start frame: %ld\n", start_frame_tick);
        SDL_PumpEvents();
        this->check_input();

        this->update_ctx.update();
//        printf("after frame update: %d\n", clock() - start_frame_tick);

        this->graphics.clear();
//        printf("after frame clear: %d\n", clock() - start_frame_tick);
        this->graphics.prepare_boxes();
        this->graphics.render();
//        printf("after frame render: %d\n", clock() - start_frame_tick);
        this->graphics.present();
//        printf("after frame present: %d\n", clock() - start_frame_tick);

//        if ((this->loading_scene != nullptr) && (this->loading_scene->is_ready()))
//        {
////            printf("starting scene\n");
//            this->start_scene(this->loading_scene);
//            this->loading_scene = nullptr;
//        }

        std::clock_t end_frame_tick = clock();
        uint frame_time = (end_frame_tick - start_frame_tick) / (CLOCKS_PER_SEC / 1000);

//        printf("framesleep %d\n", this->frame_delay - frame_time);
        if (frame_time < this->frame_delay)
            SDL_Delay(this->frame_delay - frame_time);
    }
}


void engine::set_running(bool running)
{
    this->running = running;
}


}



