#include "enums.hpp"
#include <iostream>
using namespace std;

class Cell
{
    bool is_alive=false;
    uint8_t age=0;
    
   bound_apex_normal boa;

    public:
    void set_boa(bound_apex_normal passed_boa)
    {
        boa=passed_boa;
    }

    bound_apex_normal get_boa() const
    {
        return boa;
    }

    bool get_is_alive() const
    {
        return is_alive;
    }

    uint8_t get_age() const
    {
        return age;
    }

    void set_is_alive(bool alive)
    {
        is_alive = alive;
    }

    void increment_age()
    {
        age++;
    }
};