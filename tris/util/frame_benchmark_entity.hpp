
#pragma once

#include "../tris.hpp"

namespace tris
{
namespace util
{


typedef unsigned int uint;


class frame_benchmark_entity : public tris::entity
{
public:
    uint frame = 0;
    uint required_frames;

    frame_benchmark_entity(uint required_frames);

    virtual void update(tris::engine* eng);
};



}
}

