#include <iostream>
//#include <ranges>
#include "Simulation.h"
#include "mystd/StlStream.h"
#include "mystd/Histogram.h"
#include "gnuplot-iostream/gnuplot-iostream.h"

int main() {

    Simulation mySim(1000);

    Histogram hUnemployed(0.0, 0.5, 0.01);
    Histogram hFirmSizes(-0.5,20.0, 1.0);
    std::vector<double> gdp;
    double lastCumulativeDemand = 0.0;

    for(int month = 0; month < 2400; ++month) {
        mySim.step();
        hUnemployed.push(mySim.proportionUnemployed());
        for(const auto &company : mySim.companies) {
            hFirmSizes.push(company.employees.size());
        }
        if(month % 4 == 3) {
            gdp.push_back(mySim.cumulativeDemand - lastCumulativeDemand);
            lastCumulativeDemand = mySim.cumulativeDemand;
        }
    }


    Gnuplot gp2;
    gp2 << "plot '-' with lines title 'nUnemployed'" << std::endl;
    gp2.send1d(hUnemployed.data);

    std::cout << hFirmSizes.data;
    Gnuplot gpFS;
    gpFS << "plot '-' using 1:(log($2)) with points title 'firm sizes'" << std::endl;
    gpFS.send1d(hFirmSizes.data);

    return 0;
}
