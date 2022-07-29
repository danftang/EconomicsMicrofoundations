//
// Created by daniel on 21/07/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_SIMULATION_H
#define ECONOMICSMICROFOUNDATIONS_SIMULATION_H

#include "mystd/Random.h"
#include "Agent.h"
#include "mystd/MutableCategoricalArray.h"

class Simulation {
public:
    std::vector<Agent>      agents;
    MutableCategoricalArray hiringDistribution; // by index into vector of agents
    double                  aggregateDemand;    // un-allocated demand for this step
    double                  cumulativeDemand;   // demand since last reset

    Simulation(int nAgents);

    void step();

    void setHiringProbability(Agent &agent, double p)   { hiringDistribution[&agent - agents.data()] = p; }
    Agent &choosePotentialEmployer(Agent &forAgent);
    Agent &chooseAgent()                                { return agents[Random::nextInt(0, agents.size())]; }

    double proportionEntrepreneurs();
    double proportionUnemployed();
    double proportionEmployed();
    std::vector<int> firmSizes();
    double totalWealth();

    void generateDemand(double amount) { aggregateDemand += amount; cumulativeDemand += amount; };
    void absorbDemand(double amount) { aggregateDemand -= amount; }

    void sanityCheck();
protected:

};


#endif //ECONOMICSMICROFOUNDATIONS_SIMULATION_H
