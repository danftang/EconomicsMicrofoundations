//
// Created by daniel on 21/07/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_SIMULATION_H
#define ECONOMICSMICROFOUNDATIONS_SIMULATION_H

#include "Random.h"
#include "Agent.h"
#include "MutableCategoricalArray.h"

class Simulation {
public:
    std::vector<Agent>      agents;
    MutableCategoricalArray hiringDistribution; // by index into vector of agents
    double                  aggregateDemand;

    Simulation(int nAgents);

    void step();

    void setHiringProbability(Agent &agent, double p)   { hiringDistribution[&agent - agents.data()] = p; }
    Agent &choosePotentialEmployer(Agent &forAgent);
    Agent &chooseAgent()                                { return agents[Random::nextInt(0, agents.size())]; }

    double proportionOfEntrepreneurs();
    double proportionUnemployed();
    double proportionEmployed();
    std::vector<int> firmSizes();
    double totalWealth();

    void sanityCheck();
protected:

};


#endif //ECONOMICSMICROFOUNDATIONS_SIMULATION_H
