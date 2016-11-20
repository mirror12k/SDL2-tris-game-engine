
#pragma once


#include <string>
using std::string;

#include "graphics_context.hpp"


namespace tris
{


class engine
{
private:
//    update_context* entity_data;
    graphics_context graphics;

    bool running = false;
    uint fps;
    uint frame_delay;

//    scene* loading_scene = nullptr;

public:
    engine(const string& window_title, int window_width, int window_height, uint fps);

    void check_input();

    void run();
//    void load_scene(scene* scene);
//    void start_scene(scene* scene);

    void set_running(bool running);

};



}

