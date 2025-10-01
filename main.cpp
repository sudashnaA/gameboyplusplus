#include "Cart.h"
#include "Emulator.h"

int main(int argc, char* argv[])
{
    (int)argc, (char*)argv;

    Emulator e{};
    e.run("dmg-acid2.gb");
    return 0;
}