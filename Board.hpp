#pragma once
#include <vector>
#include "Cell.hpp"
#include "enums.hpp"

class Board {
    int logical_width;
    int logical_height;
    int real_width;
    int real_height;
    boundary_mode bound;

    std::vector<Cell> current_state;
    std::vector<Cell> next_state;

    // Prywatna metoda mapująca płaskie indeksy na wektor z marginesem
    int to_index(int real_x, int real_y) const {
        return real_y * real_width + real_x;
    }

    // Inicjalizacja marginesów i tagowania (odpalana w konstruktorze)
    void init_ghost_cells() {
        for (int y = 0; y < real_height; ++y) {
            for (int x = 0; x < real_width; ++x) {
                int idx = to_index(x, y);
                
                bool is_corner = (x == 0 && y == 0) ||
                                 (x == real_width - 1 && y == 0) ||
                                 (x == 0 && y == real_height - 1) ||
                                 (x == real_width - 1 && y == real_height - 1);
                                 
                bool is_edge = (x == 0 || x == real_width - 1 ||
                                y == 0 || y == real_height - 1) && !is_corner;

                // Rozdawanie tagów z enuma
                if (is_corner) {
                    current_state[idx].set_boa(bound_apex_normal::APEX);
                    next_state[idx].set_boa(bound_apex_normal::APEX);
                    
                    if (bound == boundary_mode::APEX_BUFF) {
                        current_state[idx].set_is_alive(true);
                        next_state[idx].set_is_alive(true);
                    }
                } else if (is_edge) {
                    current_state[idx].set_boa(bound_apex_normal::BOUNDARY);
                    next_state[idx].set_boa(bound_apex_normal::BOUNDARY);
                } else {
                    current_state[idx].set_boa(bound_apex_normal::NORMAL);
                    next_state[idx].set_boa(bound_apex_normal::NORMAL);
                }
            }
        }
    }

public:
    Board(int w, int h, boundary_mode b) 
        : logical_width(w), logical_height(h), 
          real_width(w + 2), real_height(h + 2), 
          bound(b), 
          current_state((w + 2) * (h + 2)), 
          next_state((w + 2) * (h + 2)) 
    {
        init_ghost_cells();
    }

    int get_width() const { return logical_width; }
    int get_height() const { return logical_height; }

    // Ustawienie komórki przed startem gry
    void set_alive(int logical_x, int logical_y, bool alive) {
        if (logical_x >= 0 && logical_x < logical_width && 
            logical_y >= 0 && logical_y < logical_height) {
            current_state[to_index(logical_x + 1, logical_y + 1)].set_is_alive(alive);
        }
    }

    // Synchronizacja krawędzi przed każdym nowym krokiem
    void sync_edges() {
        if (bound != boundary_mode::TORUS) return; 

        for (int x = 1; x <= logical_width; ++x) {
            current_state[to_index(x, 0)].set_is_alive(current_state[to_index(x, logical_height)].get_is_alive());
            current_state[to_index(x, real_height - 1)].set_is_alive(current_state[to_index(x, 1)].get_is_alive());
        }

        for (int y = 1; y <= logical_height; ++y) {
            current_state[to_index(0, y)].set_is_alive(current_state[to_index(logical_width, y)].get_is_alive());
            current_state[to_index(real_width - 1, y)].set_is_alive(current_state[to_index(1, y)].get_is_alive());
        }

        current_state[to_index(0, 0)].set_is_alive(current_state[to_index(logical_width, logical_height)].get_is_alive());
        current_state[to_index(real_width - 1, 0)].set_is_alive(current_state[to_index(1, logical_height)].get_is_alive());
        current_state[to_index(0, real_height - 1)].set_is_alive(current_state[to_index(logical_width, 1)].get_is_alive());
        current_state[to_index(real_width - 1, real_height - 1)].set_is_alive(current_state[to_index(1, 1)].get_is_alive());
    }

    int count_neighbours(int logical_x, int logical_y) const {
        int real_x = logical_x + 1;
        int real_y = logical_y + 1;
        int count = 0;

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0) continue;
                
                if (current_state[to_index(real_x + dx, real_y + dy)].get_is_alive()) {
                    count++;
                }
            }
        }
        return count;
    }

    bool is_alive_at(int logical_x, int logical_y) const {
        return current_state[to_index(logical_x + 1, logical_y + 1)].get_is_alive();
    }

    std::uint32_t get_age_at(int logical_x, int logical_y) const {
        return current_state[to_index(logical_x + 1, logical_y + 1)].get_age();
    }

    void set_next_state_at(int logical_x, int logical_y, bool alive) {
        int idx = to_index(logical_x + 1, logical_y + 1);
        
        // 1. Zawsze nadpisujemy fizyczny stan komórki
        next_state[idx].set_is_alive(alive);
        
        // 2. Precyzyjna kontrola wieku (czyszczenie brudnej pamięci)
        if (alive) {
            if (current_state[idx].get_is_alive()) {
                // Sytuacja A: Komórka przetrwała. Starzejemy ją z zabezpieczeniem.
                std::uint32_t old_age = current_state[idx].get_age();
                if (old_age < 4000000000) {
                    next_state[idx].set_age(old_age + 1);
                } else {
                    next_state[idx].set_age(4000000000);
                }
            } else {
                // Sytuacja B: Komórka właśnie się narodziła. Musi być wyzerowana!
                next_state[idx].set_age(0);
            }
        } else {
            // Sytuacja C: Komórka umiera. Czyścimy pamięć, by nie zostawić "ducha".
            next_state[idx].set_age(0);
        }
    }

    void swap_states() {
        current_state.swap(next_state);
    }
};