//
// Created by daniel on 11/08/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_BANK_H
#define ECONOMICSMICROFOUNDATIONS_BANK_H

#include <list>
#include "mystd/Random.h"
class Company;

class Bank {
public:
    class Account {
    public:
        Account(int initialBalance): _balance(initialBalance) {}

        int balance() const { return _balance; }
        friend class Bank;
    protected:
        int _balance;
    };

    typedef std::list<Account>::iterator AccountID;

    std::list<Account>  accounts;
    std::list<Account>  loans;
    AccountID           reserveAccount;
    AccountID           loanInterestAccount;
    AccountID           creditInterestAccount;
    constexpr static int              reserveMultiplier = 5; // total loans < reserveAccount*reserveMultiplier

    Bank() {
        reserveAccount = openLoanAccount();
        loanInterestAccount = openLoanAccount();
        creditInterestAccount = openAccount();
    }

    AccountID openAccount() {
        accounts.emplace_front(0);
        return accounts.begin();
    }

    AccountID openLoanAccount() {
//        std::cout << "Opening loan account" << std::endl;
        loans.emplace_front(0);
        return loans.begin();
    }

    void step();

    void closeCreditAccount(AccountID accountId) {
        if(accountId != accounts.end()) {
//            std::cout << "Closing credit account " << accountId->balance() << std::endl;
            transfer(accountId, creditInterestAccount, accountId->balance());
            assert(accountId->balance() == 0);
            accounts.erase(accountId);
        }
    }

    void closeLoanAccount(AccountID accountId) {
        if(accountId != loans.end()) {
//            std::cout << "Closing loan account" << accountId->balance() << std::endl;
            transfer(accountId, loanInterestAccount, accountId->balance());
            assert(accountId->balance() == 0);
            loans.erase(accountId);
        }
    }

    bool transfer(AccountID sender, AccountID recipient, int amount);

    int getStartupLoan();


    int assets();
    int liabilities();
    void sanityCheck();
};

#endif //ECONOMICSMICROFOUNDATIONS_BANK_H
