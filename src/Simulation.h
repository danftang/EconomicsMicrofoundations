//
// Created by daniel on 21/07/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_SIMULATION_H
#define ECONOMICSMICROFOUNDATIONS_SIMULATION_H

#include "mystd/Random.h"
#include "Person.h"
#include "mystd/MutableCategorical.h"
#include "Company.h"
#include "VCFund.h"
#include "Agent.h"

class Simulation {
public:
    std::vector<Person>      agents;
    MutableCategorical<Company> companies;  // container of companies from which we can draw for recruitment
    VCFund                  fund;
    Agent                   aggregateDemandAccount;
    double                  cumulativeDemand;   // demand since last reset

    Simulation(int nAgents);

    void step();

    void setHiringProbability(MutableCategorical<Company>::iterator company, double p)   { companies.set(company, p); }
    Company &choosePotentialEmployer();
    Person &chooseAgent()                                { return agents[Random::nextInt(0, agents.size())]; }
    Company *startNewCompany(double founderInvestmentExpectation);

    double proportionUnemployed();
    std::vector<int> firmSizes();
    double totalWealth();

//    void generateDemand(double amount) { aggregateDemand += amount; cumulativeDemand += amount; };
//    void absorbDemand(double amount) { aggregateDemand -= amount; }

    void sanityCheck();
protected:

};


#endif //ECONOMICSMICROFOUNDATIONS_SIMULATION_H
