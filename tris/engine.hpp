
#pragma once


#include <string>
using std::string;

#include "update_context.hpp"
#include "graphics_context.hpp"


namespace tris
{


class engine
{
public:
    graphics_context graphics;
    update_context update_ctx;

    bool running = false;
    uint fps;
    uint frame_delay;

//    scene* loading_scene = nullptr;

    engine(const string& window_title, int window_width, int window_height, uint fps);

    void check_input();

    void run();
//    void load_scene(scene* scene);
//    void start_scene(scene* scene);

    void set_running(bool running);

};



}

