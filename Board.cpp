#include <iostream>
#include <vector>
#include "Cell.hpp"
#include "enums.hpp"

using namespace std;

class Board
{
    size_t width;
    size_t height;
    boundary_mode bound;

    vector<Cell> current_state;
    vector<Cell> next_state;
    
    public:

  Board(size_t a, size_t b): height(a), width(b), current_state(a*b) {};
  void count_neighbours(size_t & cell)
  {
    size_t counter =0;
    for(size_t i = -1; i<2; i++)
    {
    if (cell - 1 + i*width)
        ++counter;
    if (cell + 1 + i*width)
        ++counter;
    }

  }
  void count_nextmap(vector <size_t>cmap, vector <size_t> nmap)// tu chcialem obliczyc nowa mape na podstawie starej, ale zorientowalem sie
  {
    for (int i = 0; i< height*width; i++)
    {
        if (cmap[i])
        {
        //test gita na pc
            //test gita na nowym kompie
        }
    }
  }
  
};