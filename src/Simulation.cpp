//
// Created by daniel on 21/07/22.
//

#include "Simulation.h"
#include "Random.h"

Simulation::Simulation(int nAgents): hiringDistribution(nAgents) {
    const double initialWealth = 100.0;
    agents.reserve(nAgents);
    for(int i=0; i<nAgents; ++i) {
        agents.emplace_back(initialWealth);
    }
}

void Simulation::step() {
    aggregateDemand = 0.0;
    for(int i=0; i<agents.size(); ++i) {
        int activeAgentIndex = Random::nextInt(0, agents.size());
        Agent &activeAgent = agents[activeAgentIndex];
        if(activeAgent.status() != Agent::ENTREPRENEUR) activeAgent.negotiateEmployment(agents[hiringDistribution(Random::gen)]);
        aggregateDemand += activeAgent.spend();
        aggregateDemand -= activeAgent.work(aggregateDemand);
        if(activeAgent.status() != Agent::EMPLOYED) {
            activeAgent.manageBusiness();
            hiringDistribution[activeAgentIndex] = activeAgent.wealth;
        }
    }
}


void Simulation::wagesAndFiring(Agent &activeAgent) {
    if(activeAgent.status() == Agent::ENTREPRENEUR) {
        for(auto employee = activeAgent.employees.begin(); employee != activeAgent.employees.end(); ++employee) {
            if(activeAgent.wealth >= (*employee)->wageExpectation) {
                (*employee)->wealth += (*employee)->wageExpectation;
                activeAgent.wealth -= (*employee)->wageExpectation;
            } else { // fire employee
                (*employee)->employer = nullptr;
                employee = activeAgent.employees.erase(employee);
                --employee;
            }
        }
    }
}

