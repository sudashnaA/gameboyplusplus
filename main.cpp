#include "Cart.h"
//#include "SDL.h"

int main(int argc, char* argv[])
{
    (int)argc; (char*)argv;

    Cart c{};
    //c.cartLoad("01-special.gb");
    c.cartLoad("dmg-acid2.gb");
    return 0;
}