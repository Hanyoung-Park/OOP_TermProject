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
    map<string, vector<Account*>> user_info;

public:
    Bank();
    ~Bank();
    int validPassword(Account userAccount, int num);
    Account* returnAccount(string userName);
    Account* openAccount();

};


Bank::Bank() {}
Bank::~Bank() {}
Account* Bank::returnAccount(string userName) {
    int inputpassword;
    cout << "Password?" << endl;
    cin >> inputpassword;
    for (vector<Account*>::iterator itr = user_info[userName].begin(); itr != user_info[userName].end; ++itr) {
        if(itr->password == inputpassword) {
        return itr;
        }
    }
    return nullptr;
    
}

Account* Bank::openAccount() {
    string bankName;
    string userName;
    int accountNum;
    int password;
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
    newAccount = new Account(bankName, userName, accountNum, password, fund); //Account class에 password 추가
    if (user_info.count(userName)) {
        user_info[userName].push_back(newAccount);
        }
    else {
        vector<Account*> v_account = {newAccount};
        user_info.insert(pair<string, vector<Account*>(userName, v_account));
        }
    return newAccount; 
}


int Bank::validPassword(Account userAccount, int num) {
    if(num == userAccount.getPassword) {
        return 1;
    }
    else {
        return 0;
    }
}

