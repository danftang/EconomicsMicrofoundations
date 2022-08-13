//
// Created by daniel on 11/08/22.
//

#include <cmath>
#include <cassert>
#include "Bank.h"
#include "Company.h"

void Bank::sanityCheck() {
//    std::cout << "assets = " << assets() << " liabilities = " << liabilities() << std::endl;
    assert(assets() - liabilities() == 0);
    for(const Account &ac : loans) assert(ac.balance() <= 0);
    for(const Account &ac : accounts) assert(ac.balance() >= 0);
}

int Bank::assets() {
    int total = 0;
    for(const Account &ac : loans) total -= ac.balance();
    return total;
}

int Bank::liabilities() {
    int total = 0;
    for(const Account &ac : accounts) total += ac.balance();
    return total;
}

void Bank::step() {
    // charge loan interest and distribute credit interest.
    sanityCheck();
    if(loans.size() > 2) {
        int loanInterest = loanInterestAccount->balance()/((int)loans.size() - 2);
        for (auto loanIt = loans.begin(); loanIt != loans.end(); ++loanIt) {
            if (loanIt != reserveAccount && loanIt != loanInterestAccount)
                transfer(loanInterestAccount, loanIt, loanInterest);
        }
    }
    int creditInterest = creditInterestAccount->balance()/((int)accounts.size()-1);
    for (auto accountIt = accounts.begin(); accountIt != accounts.end(); ++accountIt) {
        if(accountIt != creditInterestAccount) transfer(creditInterestAccount, accountIt, creditInterest);
    }
    sanityCheck();
}

bool Bank::transfer(Bank::AccountID sender, Bank::AccountID recipient, int amount) {
    // maintain credit and loan account status
    if(sender->balance() > 0 && sender->balance() - amount < 0) {
        std::cout << "Refusing transfer from credit account" << std::endl;
        return false;
    }
    if(recipient->balance() < 0 && recipient->balance() + amount > 0) {
        std::cout << "Refusing transfer to loan account" << std::endl;
        return false;
    }
    sender->_balance -= amount;
    recipient->_balance += amount;
    return true;
}

int Bank::getStartupLoan() {
    int loanUpperLimit = -reserveAccount->balance()*reserveMultiplier - assets();
//        std::cout << "Loan upper limit = " << loanUpperLimit << std::endl;
//        std::cout << "reserve = " << -reserveAccount->balance() << std::endl;
    double pApproveLoan = loanUpperLimit * 1.0/ (-reserveAccount->balance()*reserveMultiplier);
//    std::cout << pApproveLoan << std::endl;
    if(Random::nextDouble() < pApproveLoan) return Random::nextInt(0, loanUpperLimit/100);
    return(0);
}
