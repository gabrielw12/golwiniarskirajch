using namespace std;


enum class bound_apex_normal {APEX, BOUNDARY, NORMAL};
enum class boundary_mode {TORUS, DEAD_ZONE, WIDE_DEAD_ZONE, APEX_BUFF, INFINITE}; 

   /*
    TORUS - krawędzie sąsiadują ze sobą w logicznym sensie, tablica ma ksztalt torusa
    DEAD_ZONE - liczymy stan komorek tak, jakby wszystkie komorki za krawedzia byly zawsze martwe
    WIDE_DEAD_ZONE - margines kilku komorek za krawedzia, ktore podlegaja normalnym zasadom, udaje, ze za krawedzia jest nieskonczony obszar
    APEX_BUFF - zakladamy ze w polach za rogami sa zyjace komorki, tak aby te mogly miec szanse ozyc
    */
