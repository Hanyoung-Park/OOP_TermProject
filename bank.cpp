#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class ATM;
class Account;




class Bank {
protected:
    string bankName;
    map<string, Account*> account_info;

public:
    Bank(string bankName);
    ~Bank();
    int validPassword(Account userAccount, int num);
    Account* returnAccount(string accountNumber);
    Account* openAccount();
    string getBankName();
    

};

Bank::Bank(string bankName) {
    this->bankName = bankName;
    account_info = map<string, Account*>();
}

Bank::~Bank() {}

string Bank::getBankName() {
    return bankName;   
}

Account* Bank::returnAccount(string accountNumber) {
    string inputpassword;
    cout << "Password?" << endl;
    cin >> inputpassword;
    if (account_info[accountNumber]->getPassword() == inputpassword) {
        return account_info[accountNumber];
    }
    return nullptr;
}

Account* Bank::openAccount() {
    string bankName;
    string userName;
    string accountNum;
    string password;
    int fund;
    cout << "input Bank Name: " << endl;
    cin >> bankName;
    cout << "input User Name: " << endl;
    cin >> userName;
    cout << "input Account Number(12-digit): " << endl;
    cin >> accountNum;
    cout << "input Password: " << endl;
    cin >> password;
    cout << "input available fund: " << endl;
    cin >> fund;

    Account* newAccount;
    newAccount = new Account(bankName, userName, accountNum, fund, password); //Account class에 password 추가
    account_info.insert(pair<string, Account*>(accountNum, newAccount));
    return newAccount;

}

/*
int Bank::validPassword(Account userAccount, string password) {
    if(password == userAccount.getPassword) {
        return 1;
    }
    else {
        return 0;
    }
}
*/
