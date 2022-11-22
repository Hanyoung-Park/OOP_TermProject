#include <iostream>
#include <string>
#include <map>

using namespace std;

// class ATM;
class Bank;
class Account;
class User;
//-----------------------------------------------------------------------------

class Account
{
protected:
    string bankName;
    string userName;
    string accountNumber;
    string password;
    int availableFund;

public:
    Account(string bankName, string userName, string accountNumber, int availableFund, string password);
    int getFund();
    string getBank();
    string getNum();
    Account &operator+=(int amount);
    Account &operator-=(int amount);
};

Account::Account(string bankName, string userName, string accountNumber, int availableFund, string password)
{
    cout << "Account constructor" << endl;
    this->bankName = bankName;
    this->userName = userName;
    this->accountNumber = accountNumber;
    this->availableFund = availableFund;
    this->password = password;

}

int Account::getFund()
{
    return availableFund;
}

string Account::getBank()
{
    return bankName;
}

string Account::getNum()
{
    return accountNumber;
}

Account &Account::operator+=(int amount)
{
    availableFund += amount;
    return *this;
}

Account &Account::operator-=(int amount)
{
    availableFund -= amount;
    return *this;
}

class Card
{
protected:
    Bank *bank;
    string accNum;

public:
    Card();
    Card(Bank *b, Account *acc);
    virtual string get_card_num() = 0;
    virtual Bank *get_bank();
};

Card::Card(Bank *b, Account *acc)
{
    this->bank = b;
    this->accNum = acc->getNum();
}

string Card::get_card_num()
{
    return accNum;
}

Bank *Card::get_bank()
{
    return bank;
}

class AdminCard : Card
{
public:
    AdminCard(Bank *b, Account *acc);
    virtual string get_card_num();
    virtual Bank *get_bank();
};

AdminCard::AdminCard(Bank *b, Account *acc)
{
    this->bank = b;
    this->accNum = acc->getNum();
}

//-----------------------------------------------------------------------------
