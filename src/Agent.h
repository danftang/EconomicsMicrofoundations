//
// Created by daniel on 11/08/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_AGENT_H
#define ECONOMICSMICROFOUNDATIONS_AGENT_H


class Agent {
public:
    Agent(double initialWealth): wealth(initialWealth) { }

    bool transferMoneyTo(Agent &recipient, double amount) {
        if(amount > wealth) return false;
        wealth -= amount;
        recipient.wealth += amount;
        return true;
    }

    double wealth;


};


#endif //ECONOMICSMICROFOUNDATIONS_AGENT_H
