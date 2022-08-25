#include <iostream>
#include "Simulation.h"
#include "mystd/StlStream.h"
#include "mystd/Histogram.h"
#include "mystd/gnuplot-iostream/gnuplot-iostream.h"

thread_local Simulation sim(1000);

void twoCompanyTest() {
    Company *A = sim.startNewCompany(0, 0.5, 1.0);
    Company *B = sim.startNewCompany(0, 0.5, 1.25);
    sim.pStartNewCompany = 0.0;
    A->hire(sim.people[0]);
    B->hire(sim.people[1]);

    for(int t=0; t<100; ++t) {
        sim.step();
        std::cout
                << "\tsize  = " << A->employees.size() << " / " << B->employees.size()
                << "\tstock = " << A->stock  << " / " << B->stock
                << "\tsales = " << A->postProductionStock - A->stock  << " / " << B->postProductionStock - B->stock
                << "\tprice = " << A->unitPrice  << " / " << B->unitPrice
                << "\tbank  = " << A->bankAccount->balance() << " / " << B->bankAccount->balance()
                << std::endl;
//        std::cout  << A->employees.size() << " " << A->unitPrice << std::endl;
    }
}


int main() {
    std::vector<double> unemployed;
    std::vector<double> collectiveWellbeing;
    std::vector<double> gdp;
    double lastCumulativeDemand = 0.0;
    const int burnin = 200;

    for(int month = 0; month < 2400; ++month) {
        sim.step();
        if(month >= burnin) {
            unemployed.push_back(sim.proportionUnemployed()*100.0);
            collectiveWellbeing.push_back(sim.collectiveWellbeing());
            gdp.push_back(sim.cumulativeDemand - lastCumulativeDemand);

        }
        lastCumulativeDemand = sim.cumulativeDemand;
    }

    Gnuplot gpTime;
    gpTime << "set multiplot layout 2,1" << std::endl;
    gpTime << "plot '-' with lines title 'GDP by time'" << std::endl;
    gpTime.send1d(gdp);
    gpTime << "plot '-' with lines title 'Collective wellbeing by time'" << std::endl;
    gpTime.send1d(collectiveWellbeing);
    gpTime << "unset multiplot" << std::endl;

    std::vector<double> wealthDistribution;
    Histogram wellbeingDistribution(0.0, 1.0, 0.01,"Wellbeing");
    Histogram consumptionWellbeing(0.0,1.0,0.01,"Consumption wellbeing");
    Histogram toilWellbeing(0.0,1.0,0.01,"ToilWellbeing");
    Histogram securityWellbeing(0.0,1.0,0.01,"Security wellbeing");
    std::vector<double> wageDistribution;
    for(const Person &person: sim.people) {
        if(person.isEmployed()) wageDistribution.push_back(person.wageExpectation);
        wealthDistribution.push_back(person.bankAccount->balance()/1000.0);
        wellbeingDistribution.push(person.wellbeing());
        consumptionWellbeing.push(person.currentConsumptionWellbeing);
        toilWellbeing.push(person.toilWellbeing());
        securityWellbeing.push(person.securityWellbeing());
    }

    Gnuplot gpPerson;
    gpPerson << "set term wxt size 1200,640" << std::endl;
    gpPerson << "set multiplot layout 3,3" << std::endl;
    gpPerson << Histogram(wageDistribution,100,"Wage distribution");
    gpPerson << Histogram(wealthDistribution,100,"Wealth (£k)");
    gpPerson << Histogram(unemployed,100,"Unemployed %");
    gpPerson << wellbeingDistribution;
    gpPerson << consumptionWellbeing;
    gpPerson << toilWellbeing;
    gpPerson << securityWellbeing;
    gpPerson << "unset multiplot" << std::endl;

    Histogram hProductDistribution(0.0, 1.0, 0.01, "Products");
    Histogram hProductivityDistribution(0.5, 1.5, 0.01,"Productivity");
    std::vector<int> firmSizes;
    std::vector<int> companyAge;
    std::vector<double> companyBalanceDistribution;
    std::vector<double> companyDebtDistribution;
    std::vector<std::pair<double,double>> priceProduct;

    const Company *largestCompany = nullptr;
    int maxEmployees = 0;
    for(const Company &company: sim.companies) {
        hProductDistribution.push(company.product, std::round(company.employees.size() * company.productivityPerEmployee / company.toilPerUnitproduct));
        hProductivityDistribution.push(company.productivityPerEmployee, company.employees.size());
        companyAge.push_back(company.age);
        companyBalanceDistribution.push_back(company.bankAccount->balance());
        companyDebtDistribution.push_back(company.loanAccount->balance());
        firmSizes.push_back(company.employees.size());
        priceProduct.push_back(std::pair(company.product, company.unitPrice));
        if(company.employees.size() > maxEmployees) {
            largestCompany = &company;
            maxEmployees = company.employees.size();
        }
//        std::cout << company.bankAccount->balance() << " " << company.loanAccount->balance() << " = " << company.bankAccount->balance() + company.loanAccount->balance() << std::endl;

    }
    Gnuplot gpCompanies;
    gpCompanies << "set multiplot layout 3,2" << std::endl;
    gpCompanies << hProductDistribution;
    gpCompanies << hProductivityDistribution;
    gpCompanies << Histogram(companyAge,100,"Company age");
    gpCompanies << Histogram(firmSizes,100,"Firm sizes");
    gpCompanies << "plot '-' with points title 'price by product'" << std::endl;
    gpCompanies.send1d(priceProduct);
    gpCompanies << "unset multiplot" << std::endl;


    std::cout << "nCompanies = " << sim.companies.size() << std::endl;
    std::cout << "Largest company has " << largestCompany->employees.size() << " employees. Product/Productivity is "
    << largestCompany->product << " / " << largestCompany->productivityPerEmployee
    << " Age = " << largestCompany->age
    << " Balance = " << largestCompany->bankAccount->balance()
    << " Loan = " << largestCompany->loanAccount->balance()
    << std::endl;
//    Histogram(companyBalanceDistribution,100).plot("Company liquidity distribution");
//    Histogram(companyDebtDistribution,100).plot("Company debt distribution");
    return 0;
}
