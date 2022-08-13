//
// Created by daniel on 11/08/22.
//

#include "Agent.h"
#include "Simulation.h"

Agent::Agent(): bankAccount(sim.bank.openAccount()) { }

Agent::~Agent() { sim.bank.closeCreditAccount(bankAccount); }
