
#pragma once



namespace tris
{


class engine;

class entity
{
public:
    virtual ~entity();
    virtual void update(engine* eng);
    virtual void on_added(engine* eng);
    virtual void on_removed(engine* eng);
};



}


