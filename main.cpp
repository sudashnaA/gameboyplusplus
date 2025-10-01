#include "Cart.h"
#include "Emulator.h"

int main(int argc, char* argv[])
{
    (int)argc, (char*)argv;

    Emulator e{};
    e.run("Super Mario Land (World).gb");
    return 0;
}