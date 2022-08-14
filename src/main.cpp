#include <iostream>
#include "Simulation.h"
#include "mystd/StlStream.h"
#include "mystd/Histogram.h"
#include "gnuplot-iostream/gnuplot-iostream.h"

thread_local Simulation sim(1000);

int main() {

    Histogram hUnemployed(0.0, 1.0, 0.01);
    Histogram hFirmSizes(-0.5,50.0, 1.0);
    std::vector<double> collectiveWellbeing;
    std::vector<double> gdp;
    double lastCumulativeDemand = 0.0;

    for(int month = 0; month < 1200; ++month) {
        sim.step();
        hUnemployed.push(sim.proportionUnemployed());
        collectiveWellbeing.push_back(sim.collectiveWellbeing());
        for(const Company &company : sim.companies) {
            hFirmSizes.push(company.employees.size());
        }
        if(month % 4 == 3) {
            gdp.push_back(sim.cumulativeDemand - lastCumulativeDemand);
            lastCumulativeDemand = sim.cumulativeDemand;
        }
        std::cout << sim.companies.size() << " " << sim.collectiveWellbeing() << std::endl;
    }

    Gnuplot gp2;
    gp2 << "plot '-' with lines title 'nUnemployed distribution'" << std::endl;
    gp2.send1d(hUnemployed.data);

    Gnuplot gpFS;
    gpFS << "plot '-' using 1:(log($2)) with points title 'firm size log frequency'" << std::endl;
    gpFS.send1d(hFirmSizes.data);

    Gnuplot gpGDP;
    gpGDP << "plot '-' with lines title 'GDP by time'" << std::endl;
    gpGDP.send1d(gdp);

    Gnuplot gpWellbeing;
    gpWellbeing << "plot '-' with lines title 'Collective wellbeing by time'" << std::endl;
    gpWellbeing.send1d(collectiveWellbeing);

    Histogram hWageDistribution(0.0, 2000.0, 25.0);
    for(const Person &person: sim.people) {
        if(person.isEmployed()) hWageDistribution.push(person.wageExpectation);
    }

    Gnuplot gpWages;
    gpWages << "plot '-' with lines title 'wage distribution'" << std::endl;
    gpWages.send1d(hWageDistribution.data);

    Histogram hWealthDistribution(0.0, 20000.0, 100.0);
    for(const Person &person: sim.people) {
        hWealthDistribution.push(person.bankAccount->balance());
    }

    Gnuplot gpWealth;
    gpWealth << "plot '-' with lines title 'wealth distribution'" << std::endl;
    gpWealth.send1d(hWealthDistribution.data);

    return 0;
}
