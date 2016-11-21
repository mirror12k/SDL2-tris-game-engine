
#pragma once

#include "../tris.hpp"

namespace tris
{
namespace util
{


typedef unsigned int uint;


class fps_ticker_entity : public tris::entity
{
public:
    uint tick = 0;
    uint frame = 0;

    virtual void update(tris::engine* eng);
};



}
}
