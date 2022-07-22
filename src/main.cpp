#include <iostream>
#include "Simulation.h"

int main() {

    Simulation mySim(1000);

    for(int month = 0; month < 1200; ++month) {
        std::cout << "Month " << month << std::endl;
        mySim.step();
    }

    return 0;
}
