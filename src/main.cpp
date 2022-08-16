#include <iostream>
#include "Simulation.h"
#include "mystd/StlStream.h"
#include "mystd/Histogram.h"
#include "mystd/gnuplot-iostream/gnuplot-iostream.h"

thread_local Simulation sim(1000);

int main() {

    std::vector<double> unemployed;
    std::vector<int> firmSizes;
    std::vector<double> collectiveWellbeing;
    std::vector<double> gdp;
    double lastCumulativeDemand = 0.0;
    const int burnin = 200;

    for(int month = 0; month < 1200; ++month) {
        sim.step();
        if(month >= burnin) {
            unemployed.push_back(sim.proportionUnemployed());
            collectiveWellbeing.push_back(sim.collectiveWellbeing());
            for (const Company &company: sim.companies) {
                firmSizes.push_back(company.employees.size());
            }
            gdp.push_back(sim.cumulativeDemand - lastCumulativeDemand);

        }
        lastCumulativeDemand = sim.cumulativeDemand;

//        std::cout << sim.companies.size() << " " << sim.collectiveWellbeing() << std::endl;
    }

//    Histogram(unemployed,100).plot("nUnemployed distribution");
    Histogram(firmSizes,100).plot("Firm size distribution");

    Gnuplot gpGDP;
    gpGDP << "plot '-' with lines title 'GDP by time'" << std::endl;
    gpGDP.send1d(gdp);

    Gnuplot gpWellbeing;
    gpWellbeing << "plot '-' with lines title 'Collective wellbeing by time'" << std::endl;
    gpWellbeing.send1d(collectiveWellbeing);

    std::vector<double> wageDistribution;
    for(const Person &person: sim.people) {
        if(person.isEmployed()) wageDistribution.push_back(person.wageExpectation);
    }
    Histogram(wageDistribution,100).plot("wage distribution");

    std::vector<double> wealthDistribution;
    std::vector<double> wellbeingDistribution;
    for(const Person &person: sim.people) {
        wealthDistribution.push_back(person.bankAccount->balance());
        wellbeingDistribution.push_back(person.wellbeing());
    }
    Histogram(wealthDistribution,100).plot("wealth distribution");
    Histogram(wellbeingDistribution,100).plot("wellbeing distribution");

    Histogram hProductDistribution(0.0, 1.0, 0.01);
    Histogram hProductivityDistribution(0.5, 1.5, 0.01);
    std::vector<int> companyAge;
    std::vector<double> companyBalanceDistribution;
    std::vector<double> companyDebtDistribution;
    const Company *largestCompany = nullptr;
    int maxEmployees = 0;
    for(const Company &company: sim.companies) {
        hProductDistribution.push(company.product, std::round(company.employees.size() * company.productivityPerEmployee / company.toilPerUnitproduct));
        hProductivityDistribution.push(company.productivityPerEmployee, company.employees.size());
        companyAge.push_back(company.age);
        companyBalanceDistribution.push_back(company.bankAccount->balance());
        companyDebtDistribution.push_back(company.loanAccount->balance());
        if(company.employees.size() > maxEmployees) {
            largestCompany = &company;
            maxEmployees = company.employees.size();
        }
//        std::cout << company.bankAccount->balance() << " " << company.loanAccount->balance() << " = " << company.bankAccount->balance() + company.loanAccount->balance() << std::endl;
    }
    hProductDistribution.plot("Production distribution");
    hProductivityDistribution.plot("Productivity distribution");
    std::cout << "Largest company has " << largestCompany->employees.size() << " employees. Product/Productivity is "
    << largestCompany->product << " / " << largestCompany->productivityPerEmployee
    << " Balance = " << largestCompany->bankAccount->balance()
    << " Loan = " << largestCompany->loanAccount->balance()
    << std::endl;
//    Histogram(companyAge,100).plot("Company age");
//    Histogram(companyBalanceDistribution,100).plot("Company liquidity distribution");
//    Histogram(companyDebtDistribution,100).plot("Company debt distribution");
    return 0;
}
