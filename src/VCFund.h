//
// Created by daniel on 02/08/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_VCFUND_H
#define ECONOMICSMICROFOUNDATIONS_VCFUND_H


class VCFund: public Agent {
public:
    double totalShares;

    VCFund(double initialWealth): Agent(initialWealth) { }

    double getStartupInvestment() {
        return Random::nextDouble(0.0, wealth/2.0);
    }
};


#endif //ECONOMICSMICROFOUNDATIONS_VCFUND_H
