#include <iostream>
#include "Simulation.h"
#include "StlStream.h"

int main() {

    Simulation mySim(1000);

    for(int month = 0; month < 1200; ++month) {
        mySim.step();
        std::cout
            << mySim.proportionEmployed() << " "
            << mySim.proportionOfEntrepreneurs() << " "
            << mySim.proportionUnemployed() << " "
            << mySim.proportionEmployed() + mySim.proportionOfEntrepreneurs() + mySim.proportionUnemployed()
            << std::endl;
    }

    return 0;
}
