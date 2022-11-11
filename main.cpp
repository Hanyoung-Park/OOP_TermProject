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
    bool isUnilingual; // REQ1.3
    int amountOfCashes; // REQ1.4 참조
    // REQ1.9, History를 어떻게 담을지 고민. (어레이 형태?)
    string primaryBankName;

public:
    ATM(string primaryBankName, string serial, bool isSingleBank, bool isUnilingual, int amountOfCashes);
    void endSession(); // REQ2.2에 써먹기
    bool checkExceptionalCondition(); // REQ2.2에 써먹기 및 9번
    int selectLanguage(bool isUnilingual); //REQ1.3, true일 경우 그냥 0 리턴, false일 경우 영어 선택시 0 리턴, false일 경우 한국어 선택시 1 리턴

    void deposit(); //4번
    void withdrawal(); //5번
    void transfer(); //6번
    void displayHistory(); //7번
    void multiLanguageSupport(); //8번
    int calculateFee();


};

class Bank {
private:

public:
    Bank();
    int transactionFee[7] = {1000, 0, 1000, 2000, 2000, 3000, 4000}; // REQ1.8
    void openAccount(); // REQ1.5

};

class Account {
protected:
    string bankName;
    string userName;
    string accountNumber;
    int availableFund;
public:
    Account(string bankName, string userName, string accountNumber, int availableFund);

};

class User {
private:
    // REQ1.6, REQ1.7, 어떤 구조로 Bank별 Account를 담을지 고민

public:
    User();

};

int main() {
    Account account1 = Account("Kakao", "David", "111-111-111111", 100000);
    Account account2 = Account("Shinhan", "Jane", "222-222-222222", 0);
    Account account3 = Account("Kakao", "Kate", "333-333-333333", 3000);
    ATM atm1 = ATM("Kakao", "111111", true, true, 50000);
    ATM atm2 = ATM("Shinhan", "222222", false, false, 0);
    ATM atm3 = ATM("Shinhan", "333333", false, false, 2000);

    return 0;
}
