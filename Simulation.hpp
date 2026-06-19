#pragma once
#include "Board.hpp"
#include "Rules.hpp"
#include <iostream>

class Simulation {
    Board board;
    const Rules* rules;

public:
    Simulation(int width, int height, boundary_mode bound, const Rules* r) 
        : board(width, height, bound), rules(r) {}

    
    Board& get_board() { 
        return board; 
    }

    
    void step() {
        board.sync_edges(); 

        for (int y = 0; y < board.get_height(); ++y) {
            for (int x = 0; x < board.get_width(); ++x) {
                
                int neighbours = board.count_neighbours(x, y);
                bool current_alive = board.is_alive_at(x, y);
                
                
                bool future_alive = rules->next_state(current_alive, neighbours);
                
                board.set_next_state_at(x, y, future_alive);
            }
        }
        board.swap_states();
    }

    
    void print() const {
        for (int y = 0; y < board.get_height(); ++y) {
            for (int x = 0; x < board.get_width(); ++x) {
                if (board.is_alive_at(x, y)) {
                    std::cout << "O "; // Żywa komórka
                } else {
                    std::cout << ". "; // Martwa komórka
                }
            }
            std::cout << "\n";
        }
        std::cout << "----------------------\n";
    }
};