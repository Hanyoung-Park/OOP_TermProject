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
    string getPassword();
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

string Account::getPassword()
{
    return password;
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
    Account *cardAcc;
public:
    Card ();
    string get_card_num();
    virtual Bank *get_bank();
    virtual Account *get_account();
};

Card::Card()
{
}

string Card::get_card_num()
{
    return accNum;
}

Bank *Card::get_bank()
{
    return bank;
}

Account *Card::get_account(){
    return cardAcc;
}

class NormCard: Card
{
public:
    NormCard(Bank *b, Account *acc);
};

NormCard::NormCard(Bank *b, Account *acc)
{
    this->bank = b;
    this->cardAcc = acc;
    this->accNum = acc->getNum();
}

class AdminCard : Card
{
public:
    AdminCard(string accNum);
    Bank *get_bank() override;
    Account *get_account() override;
};

AdminCard::AdminCard(string accNum)
{
    this->accNum = accNum;
}

Bank *AdminCard::get_bank()
{
    return NULL;
}

Account *AdminCard::get_account(){
    return NULL;
}

//-----------------------------------------------------------------------------

