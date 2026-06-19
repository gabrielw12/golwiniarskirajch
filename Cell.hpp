#pragma once
#include "enums.hpp"
#include <cstdint>

class Cell {
    bool is_alive = false;
    std::uint32_t age = 0;
    bound_apex_normal boa = bound_apex_normal::NORMAL;

public:
    void set_boa(bound_apex_normal passed_boa) { 
        boa = passed_boa; 
    }

    bound_apex_normal get_boa() const { 
        return boa; 
    }

    bool get_is_alive() const { 
        return is_alive; 
    }

    std::uint32_t get_age() const { 
        return age; 
    }

    void set_is_alive(bool alive) { 
        is_alive = alive; 
        if (!alive) {
            age = 0; 
        }
    }

    void set_age(std::uint32_t new_age) { 
        age=new_age; 
    }
};