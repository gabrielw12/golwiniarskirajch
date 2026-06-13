#include "enums.h"
#include <iostream>
using namespace std;

class Cell
{
    bool is_alive=false;
    uint8_t age=0;
    
   bound_apex_normal boa;

    public:
    void set_boa(bound_apex_normal passed_boa)
    {
        boa=passed_boa;
    }
};