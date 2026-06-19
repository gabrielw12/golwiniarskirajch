#pragma once

enum class bound_apex_normal { APEX, BOUNDARY, NORMAL };

enum class boundary_mode { 
    TORUS, 
    DEAD_ZONE,  
    APEX_BUFF, 
};
// DEAD_ZONE - uznajemy komórki za krawędziami jak martwe
// APEX_BUFF _ uznajemy komórki za krawędziami jaki martwe poza wierzchołkami, które są zawsze żywe