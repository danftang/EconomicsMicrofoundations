#include <iostream>
#include "Simulation.h"
#include "mystd/StlStream.h"
#include "mystd/Histogram.h"
#include "gnuplot-iostream/gnuplot-iostream.h"

thread_local Simulation sim(1000);

int main() {

    Histogram hUnemployed(0.0, 1.0, 0.01);
    Histogram hFirmSizes(-0.5,50.0, 1.0);
    std::vector<double> gdp;
    double lastCumulativeDemand = 0.0;

    for(int month = 0; month < 2400; ++month) {
        sim.step();
        hUnemployed.push(sim.proportionUnemployed());
        for(const auto &company : sim.companies) {
            hFirmSizes.push(company.employees.size());
        }
        if(month % 4 == 3) {
            gdp.push_back(sim.cumulativeDemand - lastCumulativeDemand);
            lastCumulativeDemand = sim.cumulativeDemand;
        }
        std::cout << sim.companies.size() << std::endl;
    }


    Gnuplot gp2;
    gp2 << "plot '-' with lines title 'nUnemployed'" << std::endl;
    gp2.send1d(hUnemployed.data);

    Gnuplot gpFS;
    gpFS << "plot '-' using 1:(log($2)) with points title 'log frequency by firm size'" << std::endl;
    gpFS.send1d(hFirmSizes.data);

    Gnuplot gpGDP;
    gpGDP << "plot '-' with lines title 'GDP by time'" << std::endl;
    gpGDP.send1d(gdp);

    return 0;
}
