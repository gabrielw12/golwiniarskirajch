#include <iostream>
#include <vector>
#include <algorithm>
#include <optional> 
#include <cstdint>  
#include <random>       
#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "enums.hpp"
#include "Conway.hpp"
#include "Simulation.hpp"

// --- SYSTEM KOLORÓW (HSV -> RGB) ---

sf::Color hsv_to_rgb(float h, float s, float v) {
    int hi = static_cast<int>(std::floor(h / 60.0f)) % 6;
    float f = (h / 60.0f) - std::floor(h / 60.0f);
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    std::uint8_t V = static_cast<std::uint8_t>(v * 255);
    std::uint8_t P = static_cast<std::uint8_t>(p * 255);
    std::uint8_t Q = static_cast<std::uint8_t>(q * 255);
    std::uint8_t T = static_cast<std::uint8_t>(t * 255);

    switch (hi) {
        case 0: return sf::Color(V, T, P);
        case 1: return sf::Color(Q, V, P);
        case 2: return sf::Color(P, V, T);
        case 3: return sf::Color(P, Q, V);
        case 4: return sf::Color(T, P, V);
        case 5: return sf::Color(V, P, Q);
        default: return sf::Color::Black;
    }
}

sf::Color get_age_color(std::uint32_t age) {
    if (age == 0) return sf::Color(255, 255, 255);

    // Maksymalny wiek: 36 000 generacji (ok. 10 minut przy 60 Hz)
    const std::uint32_t MAX_AGE = 36000; 
    
    std::uint32_t clamped_age = std::min(age, MAX_AGE);
    float progress = static_cast<float>(clamped_age) / MAX_AGE;
    float hue = progress * 280.0f; // Od czerwieni (0) do fioletu (280)

    return hsv_to_rgb(hue, 1.0f, 1.0f);
}

// --- FUNKCJE GENERUJĄCE PLANSZĘ ---

void load_demo(Board& board) {
    std::vector<std::pair<int, int>> glider_gun = {
        {24, 1}, {22, 2}, {24, 2}, {12, 3}, {13, 3}, {20, 3}, {21, 3}, {34, 3}, {35, 3},
        {11, 4}, {15, 4}, {20, 4}, {21, 4}, {34, 4}, {35, 4}, {1, 5}, {2, 5}, {10, 5},
        {16, 5}, {20, 5}, {21, 5}, {1, 6}, {2, 6}, {10, 6}, {14, 6}, {16, 6}, {17, 6},
        {22, 6}, {24, 6}, {10, 7}, {16, 7}, {24, 7}, {11, 8}, {15, 8}, {12, 9}, {13, 9}
    };
    int gun_x = 5, gun_y = 5;
    for (const auto& cell : glider_gun) {
        board.set_alive(gun_x + cell.first, gun_y + cell.second, true);
    }

    int rx = std::max(0, static_cast<int>(board.get_width()) / 2 - 10);
    int ry = std::max(0, static_cast<int>(board.get_height()) / 2 - 10);
    if (rx > 0 && ry > 0) {
        board.set_alive(rx, ry + 1, true);
        board.set_alive(rx + 1, ry, true);
        board.set_alive(rx + 1, ry + 1, true);
        board.set_alive(rx + 1, ry + 2, true);
        board.set_alive(rx + 2, ry, true);
    }
}

void load_random(Board& board, float fill_probability) {
    std::random_device rd;
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    int width = static_cast<int>(board.get_width());
    int height = static_cast<int>(board.get_height());

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (dis(gen) < fill_probability) {
                board.set_alive(x, y, true);
            }
        }
    }
}

// --- START APLIKACJI ---

int main() {
    int width, height, mode_choice, demo_choice;
    
    std::cout << "--- Game of Life - Konfiguracja ---\n";
    std::cout << "Podaj szerokosc planszy logicznej (np. 150): ";
    std::cin >> width;
    std::cout << "Podaj wysokosc planszy logicznej (np. 150): ";
    std::cin >> height;
    
    std::cout << "Wybierz tryb krawedzi:\n1. TORUS\n2. DEAD_ZONE\n3. APEX_BUFF\nWybor: ";
    std::cin >> mode_choice;
    boundary_mode bound = boundary_mode::TORUS;
    if (mode_choice == 2) bound = boundary_mode::DEAD_ZONE;
    else if (mode_choice == 3) bound = boundary_mode::APEX_BUFF;

    std::cout << "Wybierz tryb startowy:\n";
    std::cout << "1. Pusta plansza (rysuj myszka)\n";
    std::cout << "2. Demo (Glider Gun, R-pentomino)\n";
    std::cout << "3. Losowa plansza (Zupa pierwotna - 25% gestosci)\n";
    std::cout << "Wybor: ";
    std::cin >> demo_choice;

    Conway conway_rules;
    Simulation sim(width, height, bound, &conway_rules);
    
    if (demo_choice == 2) load_demo(sim.get_board());
    else if (demo_choice == 3) load_random(sim.get_board(), 0.25f);

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    const unsigned int window_width = desktop.size.x;
    const unsigned int window_height = desktop.size.y - 80;

    float cell_w = static_cast<float>(window_width) / width;
    float cell_h = static_cast<float>(window_height) / height;

    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), "Game of Life");
    window.setVerticalSyncEnabled(true); 

    sf::RectangleShape cell_shape(sf::Vector2f({cell_w - 1.0f, cell_h - 1.0f}));

    bool is_running = (demo_choice == 2 || demo_choice == 3);
    sf::Clock timer;
    float delay = 0.05f; 
    unsigned long long generation = 0;

    std::cout << "\n--- SILNIK URUCHOMIONY ---\n";
    std::cout << "[SPACJA]  - Start / Pauza\n";
    std::cout << "[STRZALKA W GORE] - Przyspiesz symulacje (az do trybu MAX)\n";
    std::cout << "[STRZALKA W DOL]  - Zwolnij symulacje\n";
    std::cout << "[LPM / PPM] - Ozywia/Zabija komorki\n";

    while (window.isOpen()) {
        
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
                
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    is_running = !is_running;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Up) {
                    delay = std::max(0.0f, delay - 0.005f);
                }
                else if (keyPressed->code == sf::Keyboard::Key::Down) {
                    delay += 0.005f;
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
            bool ozyw = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
            
            sf::Vector2i pixel_pos = sf::Mouse::getPosition(window);
            sf::Vector2f world_pos = window.mapPixelToCoords(pixel_pos);
            
            int grid_x = static_cast<int>(world_pos.x / cell_w);
            int grid_y = static_cast<int>(world_pos.y / cell_h);
            
            if (grid_x >= 0 && grid_x < width && grid_y >= 0 && grid_y < height) {
                sim.get_board().set_alive(grid_x, grid_y, ozyw);
            }
        }

        if (is_running) {
            if (delay <= 0.0001f) {
                sim.step();
                generation++;
                timer.restart();
            } 
            else {
                float dt = timer.getElapsedTime().asSeconds();
                if (dt > delay) {
                    int steps_to_catch_up = static_cast<int>(dt / delay);
                    for (int i = 0; i < steps_to_catch_up; ++i) {
                        sim.step();
                        generation++;
                    }
                    timer.restart();
                }
            }
        }

        std::string speed_str = (delay <= 0.0001f) ? "MAX Speed, MIN Delay (VSync)" : std::to_string(delay).substr(0, 5) + "s";
        std::string title = "Game of Life | Generacja: " + std::to_string(generation) + 
                            " | Opoznienie: " + speed_str +
                            (is_running ? " | [GRA]" : " | [PAUZA]");
        window.setTitle(title);

        window.clear(sf::Color(20, 20, 20));

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (sim.get_board().is_alive_at(x, y)) {
                    std::uint32_t age = sim.get_board().get_age_at(x, y);
                    
                    cell_shape.setFillColor(get_age_color(age));
                    cell_shape.setPosition(sf::Vector2f({x * cell_w, y * cell_h}));
                    
                    window.draw(cell_shape);
                }
            }
        }

        window.display();
    }

    return 0;
}