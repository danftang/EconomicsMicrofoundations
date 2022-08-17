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
            unemployed.push_back(sim.proportionUnemployed()*100.0);
            collectiveWellbeing.push_back(sim.collectiveWellbeing());
            for (const Company &company: sim.companies) {
                firmSizes.push_back(company.employees.size());
            }
            gdp.push_back(sim.cumulativeDemand - lastCumulativeDemand);

        }
        lastCumulativeDemand = sim.cumulativeDemand;

//        std::cout << sim.companies.size() << " " << sim.collectiveWellbeing() << std::endl;
    }


    Gnuplot gpTime;
    gpTime << "set multiplot layout 2,1" << std::endl;
    gpTime << "plot '-' with lines title 'GDP by time'" << std::endl;
    gpTime.send1d(gdp);
    gpTime << "plot '-' with lines title 'Collective wellbeing by time'" << std::endl;
    gpTime.send1d(collectiveWellbeing);
    gpTime << "unset multiplot" << std::endl;

    std::vector<double> wealthDistribution;
    std::vector<double> wellbeingDistribution;
    std::vector<double> wageDistribution;
    for(const Person &person: sim.people) {
        if(person.isEmployed()) wageDistribution.push_back(person.wageExpectation);
        wealthDistribution.push_back(person.bankAccount->balance()/1000.0);
        wellbeingDistribution.push_back(person.wellbeing());
    }

    Gnuplot gpPerson;
    gpPerson << "set multiplot layout 2,2" << std::endl;
    gpPerson << Histogram(wageDistribution,100,"Wage distribution");
    gpPerson << Histogram(wealthDistribution,100,"Wealth (£k)");
    gpPerson << Histogram(wellbeingDistribution,100,"Wellbeing");
    gpPerson << Histogram(unemployed,100,"Unemployed %");
    gpPerson << "unset multiplot" << std::endl;

    Histogram hProductDistribution(0.0, 1.0, 0.01, "Products");
    Histogram hProductivityDistribution(0.5, 1.5, 0.01,"Productivity");
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
    Gnuplot gpCompanies;
    gpCompanies << "set multiplot layout 2,2" << std::endl;
    gpCompanies << hProductDistribution;
    gpCompanies << hProductivityDistribution;
    gpCompanies << Histogram(companyAge,100,"Company age");
    gpCompanies << Histogram(firmSizes,100,"Firm sizes");
    gpCompanies << "unset multiplot" << std::endl;


    std::cout << "Largest company has " << largestCompany->employees.size() << " employees. Product/Productivity is "
    << largestCompany->product << " / " << largestCompany->productivityPerEmployee
    << " Balance = " << largestCompany->bankAccount->balance()
    << " Loan = " << largestCompany->loanAccount->balance()
    << std::endl;
//    Histogram(companyBalanceDistribution,100).plot("Company liquidity distribution");
//    Histogram(companyDebtDistribution,100).plot("Company debt distribution");
    return 0;
}
