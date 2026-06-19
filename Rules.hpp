#pragma once

class Rules {
public:
    virtual ~Rules() = default; 
    
    // Zwraca stan komórki w nowej generacji
    virtual bool next_state(bool is_alive, int neighbours) const = 0;
};