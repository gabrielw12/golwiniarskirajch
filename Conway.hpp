#pragma once
#include "Rules.hpp"

class Conway : public Rules {
public:
    bool next_state(bool is_alive, int neighbours) const override {
        if (is_alive) {
            return (neighbours == 2 || neighbours == 3);
        } else {
            return (neighbours == 3);
        }
    }
};