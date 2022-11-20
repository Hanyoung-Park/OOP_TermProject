#include <iostream>
#include <string>
using namespace std;


class ATM;
class Account;

int Bank::deposit = 0;
class Bank {
protected:
    string bankName;
    static int deposit;
public:
    Bank();
    ~Bank();
    Account* openAccount();
};

Bank::Bank() {}
Bank::~Bank() {}
Account* Bank::openAccount() {
    string bankName;
    string userName;
    int Accountnum;
    cout << "input Bank Name: " << endl;
    cin >> bankName;
    cout << "input User Name: " << endl;
    cin >> userName;
    cout << "input Account Number(12-digit): " << endl;
    cin >> Accountnum;
    Account* newAccount;
    //newAccount.bankName -> bankName; 

    return newAccount; 
}

int Bank::deposit = 0;