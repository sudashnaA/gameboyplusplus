#include "Cart.h"
#include "Emulator.h"

int main(int argc, char* argv[])
{
    (int)argc, (char*)argv;

    auto e{ std::make_shared<Emulator>() };
    e->init();

    e->run("mem_timing.gb");
    return 0;
}