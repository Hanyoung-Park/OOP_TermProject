#include <iostream>
#include <string>
#include <map>

using namespace std;

class ATM;
class Bank;
class Account;
class User;
class Card;

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



class User {
private:
    // REQ1.6, REQ1.7, 어떤 구조로 Bank별 Account를 담을지 고민

public:
    User();

};

class Card
{
protected:
    Bank *bank;
    string accNum;
    Account *cardAcc;

public:
    Card();
    Card(Bank *b, Account *acc);
    virtual string get_card_num() = 0;
    virtual Bank *get_bank();
    virtual Account *get_account();
};

Card::Card(Bank *b, Account *acc)
{
    this->bank = b;
    this->cardAcc = acc;
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

Account *Card::get_account(){
    return cardAcc;
}

class AdminCard : Card
{
public:
    AdminCard(Bank *b, Account *acc);
    virtual string get_card_num();
    virtual Bank *get_bank();
    virtual Account *get_account();
};

AdminCard::AdminCard(Bank *b, Account *acc)
{
    this->bank = b;
    this->accNum = acc->getNum();
    this->cardAcc = acc;
}


#include <iostream>
#include <string>
#include <map>

using namespace std;

class ATM;
class Bank;
class Account;
class User;
class Card;

class ATM {
protected:
    string serial; // REQ1.1, 6-digit serial number
    bool isSingleBank; // REQ1.2, if the atm is single atm -> true
    bool isUnilingual; // REQ1.3
    int amountOfCashes; // REQ1.4 참조
    string history; // REQ1.9, History를 어떻게 담을지 고민. (어레이 형태?)
    string primaryBankName;
    bool isEnglish;
    bool isAdmin;
    bool isPrimaryBank;

    //---Account Information
    
    Account* usingAccount;
    
public:
    ATM(string bankname, string serialnum, bool SingleBank, bool Unilingual, int cashes);
    void showInfo(string val);
    void readCardInfo(Card* card);
    void startSession();
    void endSession(); // REQ2.2에 써먹기, 세션 종료 시 모든 카드 데이터 삭제
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
    amountOfCashes = cashes;
}

void ATM::readCardInfo(Card* card) {
    isPrimaryBank = (primaryBankName==card->get_bank()->bankName);
    if(isSingleBank==true && isPrimaryBank==false) {
        cout << "The Card is invalid" << endl;
        endSession();
        return;
    }
    usingAccount = card->get_account();
}

void ATM::showInfo(string val) {
    if(isAdmin == false) {
        cout << "Wrong Approach" << endl;
        endSession();
        return;
    }
    if(val=="serial") {
        cout << "Serial number is: " << serial << endl;
    } else if(val=="cash") {
        cout << "Amount of cashes is: " << amountOfCashes << endl;
    }
}

void ATM::showHistory() {
    if(isAdmin==false) {
        cout << "Wrong Approach" << endl;
        endSession();
        return;
    }
    if(history=="") {
        cout << "No record" << endl;
        endSession();
        return;
    }
    cout << history << endl;
    endSession();
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
    int depositMoney;
    bool isCheck;
    if(isEnglish==true) {
        cout << "Please choose between cash or check." << endl;
        cout << "Cash: 0, Check: 1" << endl;
        cin >> isCheck;
        if(isCheck==false) {
            while (1) {
                cout << "Please enter the amount of fund to deposit." << endl;
                cin >> depositMoney;
                if(depositMoney%1000!=0) {
                    cout << "You can only use banknotes. Please enter the amount of fund again." << endl;
                    depositMoney = 0;
                } else {
                    break;
                }
            }
        } else {
            cout << "Please enter the amount of fund to deposit." << endl;
            cin >> depositMoney;
        }
        usingAccount += depositMoney;
        cout << "Your deposit has been succesful." << endl;
    } else {
        cout << "현금과 수표 중 사용하실 방법을 선택해주세요." << endl;
        cout << "현금: 0, 수표: 1" << endl;
        cin >> isCheck;
        if(isCheck==false) {
            while (1) {
                cout << "입금할 금액을 입력해주세요." << endl;
                cin >> depositMoney;
                if(depositMoney%1000!=0) {
                    cout << "지폐만 사용할 수 있습니다. 입금할 금액을 다시 입력해주세요." << endl;
                    depositMoney = 0;
                } else {
                    break;
                }
            }
        } else {
            cout << "입금할 금액을 입력해주세요." << endl;
            cin >> depositMoney;
        }
        
        usingAccount += depositMoney;
        cout << "입금이 성공적으로 완료되었습니다." << endl;
    }
    if(isCheck==false) {
        amountOfCashes += depositMoney;
    }
    if(isPrimaryBank==false) {
        usingAccount -= 1000;
    }
}

void ATM::withdrawal() {
    int withdrawalMoney;
    int includingFee;
    if(isPrimaryBank) {
        includingFee = 1000;
    } else {
        includingFee = 2000;
    }
    if(isEnglish==true) {
        cout << "Please enter the amount of fund to withdraw." << endl;
        cin >> withdrawalMoney;
        includingFee += withdrawalMoney;
        if(amountOfCashes<includingFee) {
            cout << "Sorry, This ATM does not have enough money in it. " << endl;
            endSession();
            return;
        }
        usingAccount -= includingFee;
        cout << "Your withdrawal has been successful." << endl;

    } else {
        cout << "출금할 금액을 입력해주세요." << endl;
        cin >> withdrawalMoney;
        includingFee += withdrawalMoney;
        if(amountOfCashes<includingFee) {
            cout << "죄송합니다만 ATM에 충분한 금액이 들어있지 않습니다." << endl;
            endSession();
            return;
        } 
        usingAccount -= includingFee;
        cout << "출금이 성공적으로 완료되었습니다." << endl;
    }
    amountOfCashes -= withdrawalMoney;   
}

void ATM::transfer() {
    string accNum;
    string transferBank;
    int transferMoney;
    int transferCase;
    if(isEnglish==true) {
        cout << "Please enter the bank of the account you want to transfer money to." << endl;
        cin >> transferBank;
        cout << "Please enter the number of the account you want to transfer money to." << endl;
        cin >> accNum;
        cout << "Please enter the amount of money to transfer." << endl;
        cin >> transferMoney;
        if(transferMoney>usingAccount->getFund()) {
            cout << "Sorry, there are not enough funds in the account." << endl;
            endSession();
            return;
        }
        usingAccount -= transferMoney;
    } else {
        cout << "송금하려는 계좌의 은행을 입력해주세요." << endl;
        cin >> transferBank;
        cout << "송금하려는 계좌의 계좌번호를 입력해주세요." << endl;
        cin >> accNum;
        cout << "이체할 금액을 입력해주세요." << endl;
        cin >> transferMoney;
        if(transferMoney>usingAccount->getFund()) {
            cout << "죄송합니다. 계좌에 충분한 금액이 들어있지 않습니다." << endl;
            endSession();
            return;
        }
        usingAccount -= transferMoney;
    }
}

void ATM::endSession() {
    usingAccount = NULL;
}





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


