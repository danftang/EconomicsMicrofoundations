//
// Created by daniel on 21/07/22.
//

#include <algorithm>
#include "Simulation.h"
#include "mystd/Random.h"


Simulation::Simulation(int nAgents): aggregateDemandAccount(0.0), fund(0.0), cumulativeDemand(0.0) {
    const double initialWealth = 10.0;
    agents.reserve(nAgents);
    for(int i=0; i<nAgents; ++i) agents.emplace_back(initialWealth);
}

void Simulation::step() {
    for(int i=0; i<agents.size(); ++i) {
        chooseAgent().step(*this);
    }
    auto companyIt = companies.begin();
    while(companyIt != companies.end()) {
        companyIt->step(*this);
        if(companyIt->employees.size() == 0) {
            fund.wealth += companyIt->wealth;
            companyIt = companies.erase(companyIt);
        } else {
            companies.set(companyIt, companyIt->wealth);
            ++companyIt;
        }
    }

//    sanityCheck();
}


Company &Simulation::choosePotentialEmployer() {
    return *companies(Random::gen);
}

std::vector<int> Simulation::firmSizes() {
    std::vector<int> sizes;
    for(const Company &company: companies) {
        sizes.push_back(company.employees.size());
    }
    return sizes;
}

double Simulation::totalWealth() {
    double total = 0.0;
    for(const Person &agent: agents) total += agent.wealth;
    for(const Company &company: companies) total += company.wealth;
    total += fund.wealth + aggregateDemandAccount.wealth;
    return total;
}

double Simulation::proportionUnemployed() {
    int nUnemployed = 0;
    for(const Person &agent: agents) if(!agent.isEmployed()) ++nUnemployed;
    return nUnemployed * 1.0/agents.size();
}

void Simulation::sanityCheck() {
    assert(fabs(totalWealth() - 10000.0) < 0.0001);

    int totalEmployees = 0;
    for(auto companyIt = companies.begin(); companyIt != companies.end(); ++companyIt) {
        assert(fabs(companyIt->wealth - companies.weight(companyIt)) < 1e-8);
        for(const Person *employee: companyIt->employees) assert(employee->employer == &*companyIt);
        totalEmployees += companyIt->employees.size();
    }

    int totalEmployed = 0;
    for(const Person &agent: agents) {
        if(agent.isEmployed()) {
            ++totalEmployed;
        }
        assert(agent.wealth >= 0.0);
    }
    assert(totalEmployed == totalEmployees);
}

Company *Simulation::startNewCompany(double founderInvestmentExpectation) {
    double investment = fund.getStartupInvestment();
    if(investment > founderInvestmentExpectation) {
        fund.wealth -= investment;
        auto newCompanyIt = companies.add(Company(investment), investment);
        return &*newCompanyIt;
    }
    return nullptr;
}
