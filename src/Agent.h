//
// Created by daniel on 11/08/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_AGENT_H
#define ECONOMICSMICROFOUNDATIONS_AGENT_H

#include "Bank.h"

class Agent {
public:
    Agent();
    Agent(const Agent &other) = delete;
    Agent(Agent &&other) = delete;
    ~Agent();

    Bank::AccountID bankAccount;
};


#endif //ECONOMICSMICROFOUNDATIONS_AGENT_H
