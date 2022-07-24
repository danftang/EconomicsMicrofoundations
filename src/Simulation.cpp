//
// Created by daniel on 21/07/22.
//

#include <algorithm>
#include "Simulation.h"
#include "Random.h"

Simulation::Simulation(int nAgents): hiringDistribution(nAgents), aggregateDemand(0.0) {
    const double initialWealth = 10.0;
    agents.reserve(nAgents);
    for(int i=0; i<nAgents; ++i) {
        Agent &newAgent = agents.emplace_back(initialWealth);
        setHiringProbability(newAgent, initialWealth);
    }
}

void Simulation::step() {
    for(int i=0; i<agents.size(); ++i) {
        chooseAgent().step(*this);
    }
    sanityCheck();
}


Agent &Simulation::choosePotentialEmployer(Agent &forAgent) {
    for(int i=0; i<1000; ++i) {
        Agent &employer = agents[hiringDistribution(Random::gen)];
        assert(employer.status() != Agent::EMPLOYED);
        assert(fabs(hiringDistribution[&employer - agents.data()] - employer.wealth) < 0.00001);
        if(&employer != &forAgent) return employer;
    }
    assert(false);
    return forAgent; // likely the only employer
}

std::vector<int> Simulation::firmSizes() {
    std::vector<int> sizes;
    for(const Agent &agent: agents) {
        sizes.push_back(agent.employees.size());
    }
    return sizes;
}

double Simulation::totalWealth() {
    double total = 0.0;
    for(const Agent &agent: agents) total += agent.wealth;
    return total;
}

double Simulation::proportionUnemployed() {
    int nUnemployed = 0;
    for(const Agent &agent: agents) if(agent.status() == Agent::UNEMPLOYED) ++nUnemployed;
    return nUnemployed * 1.0/agents.size();
}

double Simulation::proportionOfEntrepreneurs() {
    int n = 0;
    for(const Agent &agent: agents) if(agent.status() == Agent::ENTREPRENEUR) ++n;
    return n * 1.0/agents.size();
}

double Simulation::proportionEmployed() {
    int n = 0;
    for(const Agent &agent: agents) if(agent.status() == Agent::EMPLOYED) ++n;
    return n * 1.0/agents.size();
}

void Simulation::sanityCheck() {
    double totalMoney = 0.0;
    for(const Agent &agent: agents) {
        if(agent.status() == Agent::EMPLOYED) {
            assert(fabs(hiringDistribution[&agent - agents.data()]) < 0.000001);
        } else {
//            std::cout << agent.wealth << " " << hiringDistribution[&agent - agents.data()] << " " << agent.status() << std::endl;
            assert(fabs(hiringDistribution[&agent - agents.data()] - agent.wealth) < 0.0001);
        }
        if(agent.status() == Agent::ENTREPRENEUR) {
            for(const Agent *employee: agent.employees) assert(employee->employer == &agent);
        } else if(agent.status() == Agent::EMPLOYED) {
            assert(find(agent.employer->employees.begin(), agent.employer->employees.end(), &agent) != agent.employer->employees.end());
        }
        totalMoney += agent.wealth;
        assert(agent.wealth >= 0.0);
        if(agent.employees.size() > 0) assert(agent.employer == &agent);

    }
    totalMoney += aggregateDemand;
    assert(fabs(totalMoney - 10000.0) < 0.0001);
}
