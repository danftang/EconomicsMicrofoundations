//
// Created by daniel on 21/07/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_SIMULATION_H
#define ECONOMICSMICROFOUNDATIONS_SIMULATION_H

#include "mystd/Random.h"
#include "Person.h"
#include "mystd/MutableCategorical.h"
#include "Company.h"
#include "Agent.h"
#include "Bank.h"

class Simulation {
public:
    constexpr static int     initialAgentWealth = 1000;

    Bank                        bank;
    std::vector<Person>         people;
    MutableCategorical<Company> companies;  // container of companies from which we can draw for recruitment
    Bank::AccountID             aggregateDemandAccount;
    long int                    cumulativeDemand;   // demand since last reset

    Simulation(int nAgents);

    void step();
    MutableCategorical<Company>::iterator choosePotentialEmployer();
    Person &chooseAgent();
    Company *startNewCompany(int founderInvestmentExpectation);

    // diagnostics
    double proportionUnemployed();
    std::vector<int> firmSizes();
    void sanityCheck();
};

extern thread_local Simulation sim;

#endif //ECONOMICSMICROFOUNDATIONS_SIMULATION_H
