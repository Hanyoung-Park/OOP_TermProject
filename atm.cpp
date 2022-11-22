#include <iostream>
#include <string>

using namespace std;

class ATM;
class Bank;
class Account;
class User;

class ATM {
protected:
    string serial; // REQ1.1, 6-digit serial number
    bool isSingleBank; // REQ1.2, if the atm is single atm -> true
    bool isUnilingual; // REQ1.3
    int amountOfCashes; // REQ1.4 참조
    // REQ1.9, History를 어떻게 담을지 고민. (어레이 형태?)
    string primaryBankName;
    bool isEnglish;
public:
    ATM(string bankname, string serialnum, bool SingleBank, bool Unilingual, int cashes);
    void showInfo(string val);
    void startSession();
    void endSession(); // REQ2.2에 써먹기
    bool checkExceptionalCondition(); // REQ2.2에 써먹기 및 9번
    void selectLanguage(bool isUnilingual); //REQ1.3, true일 경우 그냥 0 리턴, false일 경우 영어 선택시 0 리턴, false일 경우 한국어 선택시 1 리턴

    void deposit(); //4번
    void withdrawal(); //5번
    void transfer(); //6번
    void displayHistory(); //7번
    void multiLanguageSupport(); //8번
    int calculateFee();

    
};

ATM::ATM(string bankname, string serialnum, bool SingleBank, bool Unilingual, int cashes) {
    primaryBankName = bankname;
    serial = serialnum;
    isSingleBank = SingleBank;
    isUnilingual = Unilingual;
}

void ATM::showInfo(string val) {
    if(val=="serial") {
        cout << "Serial number is: " << serial << endl;
    } else if(val=="cash") {
        cout << "Amount of cashes is: " << amountOfCashes << endl;
    }
}

void ATM::startSession() {
    selectLanguage(isUnilingual);
    if(isEnglish==true) {
        cout << "Welcome\nTo start, please insert your debit card." << endl;
    } else {
        cout << "반갑습니다.\n시작하려면 카드를 기기에 넣어주십시오." << endl;
    }

}

void ATM::selectLanguage(bool isUnilingual) {
    if(isUnilingual==true) {
        cout << "It is set to English." << endl;
        isEnglish = true;
    } else {
        cout << "Language Setting:\nIf you want to use English, please type [E]\nElse if you want to use Korean, please type [K]" << endl;
        string language;
        cin >> language;
        if(language=="K" || language=="[K]" || language=="k" || language=="[k]") {
            cout << "한국어로 설정되었습니다." << endl;
            isEnglish = false;
        } else if(language=="E" || language=="[E]" || language=="e" || language=="[e]") {
            cout << "It is set to English." << endl;
            isEnglish = true;
        } else {
            cout << "Please enter the correct character." << endl;
            cout << "Return to the initial screen." << endl;
            selectLanguage(isUnilingual);
        }
    }
}

void ATM::deposit() {
    if(isEnglish==true) {
        cout << "Please choose between cash or check." << endl;
        cout << "Cash: 0, Check: 1" << endl;
        bool isCheck;
        cin >> isCheck;
    } else {
        cout << "현금과 수표 중 사용하실 방법을 선택해주세요." << endl;
        cout << "현금: 0, 수표: 1" << endl;
        bool isCheck;
        cin >> isCheck;
    }
}

void ATM::withdrawal() {
    int withdrawalMoney;
    if(isEnglish==true) {
        cout << "Please enter the amount of fund to withdraw." << endl;
        cin >> withdrawalMoney;
        if(amountOfCashes<withdrawalMoney) {
            cout << "Sorry, This ATM does not have enough money in it. " << endl;
            endSession();
            return;
        }

    } else {
        cout << "출금할 금액을 입력해주세요." << endl;
        cin >> withdrawalMoney;
        if(amountOfCashes<withdrawalMoney) {
            cout << "죄송합니다만 ATM에 충분한 금액이 들어있지 않습니다." << endl;
            endSession();
            return;
        } 
    }
    

}

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
    ~Account();

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
