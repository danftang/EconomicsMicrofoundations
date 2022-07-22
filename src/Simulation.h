//
// Created by daniel on 21/07/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_SIMULATION_H
#define ECONOMICSMICROFOUNDATIONS_SIMULATION_H


#include <random>
#include "Agent.h"
#include "MutableCategoricalArray.h"

class Simulation {
public:
    std::vector<Agent> agents;
    MutableCategoricalArray hiringDistribution; // by index into vector of agents
    double aggregateDemand;

    Simulation(int nAgents);

    void step();

protected:

    int nAgents();

    void wagesAndFiring(Agent &);


};


#endif //ECONOMICSMICROFOUNDATIONS_SIMULATION_H
