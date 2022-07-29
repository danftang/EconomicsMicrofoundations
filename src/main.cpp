#include <iostream>
#include "Simulation.h"
#include "mystd/StlStream.h"
#include "mystd/Histogram.h"
#include "gnuplot-iostream/gnuplot-iostream.h"

int main() {

//    std::pair<double,int> p(1.23,0);
//
//    std::cout << p << std::endl;
//
//    ++p.second;
//
//    std::cout << p << std::endl;

    Simulation mySim(1000);

    Histogram hEmployed(0.4, 0.9, 0.01);
    Histogram hUnemployed(0.0, 0.5, 0.01);
    Histogram hEntrepreneurs(0.05, 0.3, 0.01);
    Histogram hFirmSizes(-0.5,20.0, 1.0);
    std::vector<double> gdp;
    double lastCumulativeDemand = 0.0;

    for(int month = 0; month < 2400; ++month) {
        mySim.step();
        hEmployed.push(mySim.proportionEmployed());
        hUnemployed.push(mySim.proportionUnemployed());
        hEntrepreneurs.push(mySim.proportionEntrepreneurs());
        for(const auto &agent : mySim.agents) {
            if(agent.status() == Agent::ENTREPRENEUR) hFirmSizes.push(agent.employees.size());
        }
        if(month % 4 == 3) {
            gdp.push_back(mySim.cumulativeDemand - lastCumulativeDemand);
            lastCumulativeDemand = mySim.cumulativeDemand;
        }
        //        std::cout
//            << mySim.proportionEmployed() << " "
//            << mySim.proportionOfEntrepreneurs() << " "
//            << mySim.proportionUnemployed() << " "
//            << mySim.proportionEmployed() + mySim.proportionOfEntrepreneurs() + mySim.proportionUnemployed()
//            << std::endl;
    }

    Gnuplot gp;
    gp << "plot '-' with lines title 'nEmployed'" << std::endl;
    gp.send1d(hEmployed.data);

    Gnuplot gp2;
    gp2 << "plot '-' with lines title 'nUnemployed'" << std::endl;
    gp2.send1d(hUnemployed.data);

    Gnuplot gp3;
    gp3 << "plot '-' with lines title 'nEntrepreneurs'" << std::endl;
    gp3.send1d(hEntrepreneurs.data);

    std::cout << hFirmSizes.data;
    Gnuplot gpFS;
    gpFS << "plot '-' using 1:(log($2)) with points title 'firm sizes'" << std::endl;
    gpFS.send1d(hFirmSizes.data);

//    Gnuplot gpGDP;
//    gpGDP << "plot '-' with lines" << std::endl;
//    gpGDP.send1d(gdp);

    return 0;
}
