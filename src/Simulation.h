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

    Bank                        bank;
    std::vector<Person>         people;
    MutableCategorical<Company> companies;  // container of companies from which we can draw for recruitment
    long int                    cumulativeDemand;   // demand since last reset

    Simulation(int nAgents);

    void step();
    MutableCategorical<Company>::iterator chooseEmployerByWealth();
    Person &chooseAgent();
    Company *startNewCompany(int founderInvestmentExpectation, double product, double productivity);

    // diagnostics
    double proportionUnemployed();
    std::vector<int> firmSizes();
    double collectiveWellbeing();
    void sanityCheck();
};

extern thread_local Simulation sim;

#endif //ECONOMICSMICROFOUNDATIONS_SIMULATION_H
