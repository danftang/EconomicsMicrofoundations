//
// Created by daniel on 21/07/22.
//

#include <algorithm>
#include "Simulation.h"
#include "mystd/Random.h"


Simulation::Simulation(int nAgents): people(nAgents), ppl(nAgents), cumulativeDemand(0.0) {
    for(int i=0; i < people.size(); ++i) ppl[i] = &people[i];
}


// hiring (should we only hire if we can do payroll with no firing?)
// firing (if payroll cannot be payed with current cash reserves (who to fire?))
// payroll (for this month)
// production
// consumption
// (death)
void Simulation::step() {
    bank.step();
    auto companyIt = companies.begin();
    while(companyIt != companies.end()) {
        companyIt->step();
        if(companyIt->employees.size() == 0) {
            companyIt = companies.erase(companyIt);
        } else {
            companies.set(companyIt, companyIt->weight());
            ++companyIt;
        }
    }
    sanityCheck();
    std::shuffle(ppl.begin(), ppl.end(), Random::gen);
    for(int i=0; i < people.size(); ++i) ppl[i]->step();
}


MutableCategorical<Company>::iterator Simulation::chooseEmployerByWeight() {
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
        assert(fabs(companyIt->weight() - companies.weight(companyIt)) < 1e-8);
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

Company *Simulation::startNewCompany(int founderInvestmentExpectation, double product, double productivity) {
    int loan = bank.getStartupLoan();
    if(loan > founderInvestmentExpectation) {
        auto newCompanyIt = companies.emplace(1.0, product, productivity);
//        companies.set(newCompanyIt, newCompanyIt->weight());
        bank.transfer(newCompanyIt->loanAccount, newCompanyIt->bankAccount, loan);
        return &*newCompanyIt;
    }
    return nullptr;
}

Person &Simulation::chooseAgent() { return people[Random::nextInt(0, people.size())]; }

double Simulation::collectiveWellbeing() {
    double wellbeing = 0.0;
    for(const Person &person : people) {
        wellbeing += person.wellbeing();
    }
    return wellbeing;
}
