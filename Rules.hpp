#pragma once

class Rules {
public:
    virtual ~Rules() = default; // Ważne: wirtualny destruktor
    
    // Zwraca stan komórki w nowej generacji
    virtual bool next_state(bool is_alive, int neighbours) const = 0;
};