#include <iostream>
#include <string>

using namespace std;

class ATM;
class Bank;
class Account;
class User;

class ATM {
private:
    string serial; // REQ1.1, 6-digit serial number
    bool isSingleBank; // REQ1.2, if the atm is single atm -> true
    int lingual; // REQ1.3, 0: unilingual (ENG), 1: bilingual (KOR), 2: bilingual (ENG)
    int amountOfCashes; // REQ1.4 참조
    // REQ1.9, History를 어떻게 담을지 고민. (어레이 형태?)

public:

};

class Bank {
private:

public:
    Bank();
    int transactionFee[7] = {1000, 0, 1000, 2000, 2000, 3000, 4000}; // REQ1.8
    void openAccount(); // REQ1.5

};

class Account {
private:
    string num;
    string pw; // REQ3.3

};

class User {
private:
    // REQ1.6, REQ1.7, 어떤 구조로 Bank별 Account를 담을지 고민

public:

};

int main() {

    return 0;
}
