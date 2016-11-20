
#include "exception.hpp"



namespace tris
{



generic_exception::generic_exception(const string& reason)
: reason(reason)
{}


const char* generic_exception::what() const noexcept
{
    return this->reason.c_str();
}



}


