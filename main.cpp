#include <iostream>
#include <vector>
#include <conio.h>  // Wymagane na Windowsie dla _kbhit() i _getch()
#include <thread>
#include <chrono>

#include "enums.hpp"
#include "Conway.hpp"
#include "Simulation.hpp"

// Funkcja pomocnicza ukrywająca/pokazująca kursor w konsoli (kod ANSI)
void set_cursor_visible(bool visible) {
    if (visible) std::cout << "\033[?25h";
    else std::cout << "\033[?25l";
}

// Funkcja pomocnicza resetująca kursor na początek ekranu
void reset_cursor() {
    std::cout << "\033[1;1H"; 
}

int main() {
    // 1. Definiujemy reguły gry i silnik symulacji (100x100, tryb Torus)
    Conway conway_rules;
    Simulation sim(50, 50, boundary_mode::TORUS, &conway_rules);
    Board& board = sim.get_board(); // Referencja dla wygody

    // --- UMIESZCZANIE STRUKTUR (DZIKIE RZECZY) ---

    // 1. Działo Gliderów Gospera (Gosper Glider Gun) - w lewym górnym rogu
    // To struktura, która w nieskończoność produkuje i wystrzeliwuje małe statki pod kątem 45 stopni.
    std::vector<std::pair<int, int>> glider_gun = {
        {24, 1}, {22, 2}, {24, 2}, {12, 3}, {13, 3}, {20, 3}, {21, 3}, {34, 3}, {35, 3},
        {11, 4}, {15, 4}, {20, 4}, {21, 4}, {34, 4}, {35, 4}, {1, 5}, {2, 5}, {10, 5},
        {16, 5}, {20, 5}, {21, 5}, {1, 6}, {2, 6}, {10, 6}, {14, 6}, {16, 6}, {17, 6},
        {22, 6}, {24, 6}, {10, 7}, {16, 7}, {24, 7}, {11, 8}, {15, 8}, {12, 9}, {13, 9}
    };
    int gun_x = 5, gun_y = 5; // Przesunięcie działa
    for (const auto& cell : glider_gun) {
        board.set_alive(gun_x + cell.first, gun_y + cell.second, true);
    }

    // 2. R-pentomino - na środku planszy
    // Składa się tylko z 5 komórek, ale generuje olbrzymi chaos przez ponad 1100 pokoleń!
    int rx = 50, ry = 50;
    board.set_alive(rx, ry + 1, true);
    board.set_alive(rx + 1, ry, true);
    board.set_alive(rx + 1, ry + 1, true);
    board.set_alive(rx + 1, ry + 2, true);
    board.set_alive(rx + 2, ry, true);

    // 3. Żołędź (Acorn) - w prawym dolnym rogu
    // Tylko 7 komórek, które ewoluują w potężną chmurę, żyjąc przez 5206 pokoleń i zostawiając po sobie setki obiektów.
    int ax = 80, ay = 80;
    board.set_alive(ax, ay + 2, true);
    board.set_alive(ax + 1, ay, true);
    board.set_alive(ax + 1, ay + 2, true);
    board.set_alive(ax + 3, ay + 1, true);
    board.set_alive(ax + 4, ay + 2, true);
    board.set_alive(ax + 5, ay + 2, true);
    board.set_alive(ax + 6, ay + 2, true);

    // --- PRZYGOTOWANIE KONSOLI I START ---

    std::cout << "\033[2J"; // Czyścimy całkowicie ekran przed startem
    set_cursor_visible(false); // Ukrywamy mrugający kursor systemowy
    
    char key = 0;
    int generation = 0;

    // Główna pętla gry - działa dopóki nie wciśniesz 'q' lub 'Q'
    while (key != 'q' && key != 'Q') {
        
        // Jeśli użytkownik nacisnął klawisz, przechwytujemy go
        if (_kbhit()) {
            key = _getch();
        }

        reset_cursor(); // Wracamy na samą górę ekranu

        std::cout << "Automaty Komorkowe | Generacja: " << generation << "\n";
        std::cout << "Nacisnij 'q' aby wyjsc.\n\n";

        sim.print(); // Wyświetlenie planszy (Twoja metoda print z Simulation.hpp)
        sim.step();  // Wyliczenie nowej generacji
        
        generation++;

        // Usypiamy symulację na 40 milisekund (około 25 klatek na sekundę)
        // Możesz zmienić tę wartość, jeśli wolisz szybszą/wolniejszą grę
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }

    // --- ZAKOŃCZENIE PROGRAMU ---q

}