#include <iostream>
#include <vector>

using namespace std;

class Board
{
  size_t height =0;
  size_t width =0;
  vector <size_t> cmap;
  vector <size_t> nmap;

  public:

  Board(size_t a, size_t b): height(a), width(b), cmap(a*b) {};
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
  void count_nmap(vector <size_t>cmap, vector <size_t> nmap)// tu chcialem obliczyc nowa mape na podstawie starej, ale zorientowalem sie
  {
    for (int i = 0; i< height*width; i++)
    {
        if (cmap[i])
        {

        }
    }
  }
  
};

int main() 
{

    return 0;
}


/*
zastanawiamy się czy to powinna być tablica w postaci wektora wketrorów, czy pojedyńczy wektor
EDIT: generalnie prowadzący na rozkimne czy wektor czy wektor wektorow powiedzial ze to nie istotne, mamy uzyc abstrakcji do tego stopnia, by nie było istotne to, czy mapa jest tablica, wektore, wektorem wektorow itd,
szczegoly implementacji powinny byc poza logika programu (byc moze sie tu wzorce przydadza czy inz orporgrmowania XD.
*/
