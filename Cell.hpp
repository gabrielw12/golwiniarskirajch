#include "enums.hpp"
#include <iostream>
using namespace std;

class Cell
{
    bool is_alive;
    uint8_t age;
    
   bound_apex_normal boa;

    public:
    void set_boa(bound_apex_normal passed_boa);
    bound_apex_normal get_boa() const;
    bool get_is_alive() const;
    uint8_t get_age() const;
    void set_is_alive(bool alive);
    void increment_age();
};