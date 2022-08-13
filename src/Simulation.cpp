//
// Created by daniel on 21/07/22.
//

#include <algorithm>
#include "Simulation.h"
#include "mystd/Random.h"


Simulation::Simulation(int nAgents): people(nAgents), cumulativeDemand(0.0) {
    for(int i=0; i<nAgents; ++i) {
        bank.transfer(bank.reserveAccount, people[i].bankAccount, initialAgentWealth);
        people[i].wageExpectation = initialAgentWealth;
        people[i].lastWage = initialAgentWealth;
    }
    aggregateDemandAccount = bank.openAccount();
}


void Simulation::step() {
    sanityCheck();
    bank.step();
    for(int i=0; i < people.size(); ++i) {
        chooseAgent().step(*this);
    }
//    std::cout << bank.assets() - bank.liabilities() << std::endl;
    auto companyIt = companies.begin();
    while(companyIt != companies.end()) {
        companyIt->step(*this);
        if(companyIt->employees.size() == 0) {
            companyIt = companies.erase(companyIt);
        } else {
            companies.set(companyIt, companyIt->bankAccount->balance());
            ++companyIt;
        }
    }
//    std::cout << bank.assets() - bank.liabilities() << std::endl;

    sanityCheck();
}


MutableCategorical<Company>::iterator Simulation::choosePotentialEmployer() {
    return companies(Random::gen);
}

std::vector<int> Simulation::firmSizes() {
    std::vector<int> sizes;
    for(const Company &company: companies) {
        sizes.push_back(company.employees.size());
    }
    return sizes;
}


double Simulation::proportionUnemployed() {
    int nUnemployed = 0;
    for(const Person &agent: people) if(!agent.isEmployed()) ++nUnemployed;
    return nUnemployed * 1.0 / people.size();
}

void Simulation::sanityCheck() {
    bank.sanityCheck();
    int totalEmployees = 0;
//    std::cout << companies.size() << " companies" << std::endl;
    for(auto companyIt = companies.begin(); companyIt != companies.end(); ++companyIt) {
//        std::cout << companyIt->bankAccount->balance() << " " << companies.weight(companyIt) << " " << companyIt->employees.size() << std::endl;
        assert(fabs(companyIt->bankAccount->balance() - companies.weight(companyIt)) < 1e-8);
        companyIt->sanityCheck();
        totalEmployees += companyIt->employees.size();
    }

    int totalEmployed = 0;
    for(const Person &agent: people) {
        if(agent.isEmployed()) {
            ++totalEmployed;
        }
    }
    assert(totalEmployed == totalEmployees);
}

Company *Simulation::startNewCompany(int founderInvestmentExpectation) {
    int loan = bank.getStartupLoan();
    if(loan > founderInvestmentExpectation) {
        auto newCompanyIt = companies.emplace(loan);
        bank.transfer(newCompanyIt->loanAccount, newCompanyIt->bankAccount, loan);
        return &*newCompanyIt;
    }
    return nullptr;
}

Person &Simulation::chooseAgent() { return people[Random::nextInt(0, people.size())]; }
