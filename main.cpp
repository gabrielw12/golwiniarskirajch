#include <iostream>
#include <vector>
#include <ranges>
#include <enums.h>

using namespace std;



    /*
    TORUS - krawędzie sąsiadują ze sobą w logicznym sensie, tablica ma ksztalt torusa
    DEAD_ZONE - liczymy stan komorek tak, jakby wszystkie komorki za krawedzia byly zawsze martwe
    WIDE_DEAD_ZONE - margines kilku komorek za krawedzia, ktore podlegaja normalnym zasadom, udaje, ze za krawedzia jest nieskonczony obszar
    APEX_BUFF - zakladamy ze w polach za rogami sa zyjace komorki, tak aby te mogly miec szanse ozyc
    */



class Board
{
    size_t width;
    size_t height;
    boundary_mode bound;

    vector<Cell> current_state;
    vector<Cell> next_state;
    
    public:

    size_t getIndex(size_t x, size_t y)
    {
        return y*width+x;
    }

    Board(size_t width, size_t height, boundary_mode bound)
     :  width(width),
        height(height),
        bound(bound),
        current_state(width*height),
        next_state(width*height)
    {
        for( auto && [i, cells ] : views::enumerate(views::zip(current_state, next_state)) ) // wymaga c++23, petal sluzy oznaczeniu komorek na bokach i rogach
        {
            auto &&  [ currCell, nextCell ] = cells;
            int x = i % width;
            int y = i / width;
            if(x==0 || y==0 || x== width || y==height)
            {
                currCell.set_boa(bound_apex_normal::BOUNDARY);
                nextCell.set_boa(bound_apex_normal::BOUNDARY);
            }
            if((x==0 && y==0) || (x==width && y==height))
            {
                currCell.set_boa(bound_apex_normal::APEX);
                nextCell.set_boa(bound_apex_normal::APEX);
            }
        }
    }
    Board(){};

    
};




class Rules
{
    Board board;
    public:
    int count_neighbours()
    bool check if

}
