#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

class ATM;
class Bank;
class Account;
class User;
class Card;

//-----------------------------------------------------------------------------

class Account
{
protected:
    string userName;
    string accountNumber;
    string password;
    Bank* bank;
    int availableFund;

public:
    Account(Bank* bank, string userName, string accountNumber, int availableFund, string password);
    int getFund();
    Bank* getBank();
    string getNum();
    string getPassword();
    Account &operator+=(int amount);
    Account &operator-=(int amount);
};

Account::Account(Bank* bank, string userName, string accountNumber, int availableFund, string password)
{
    cout << "Account constructor" << endl;
    this->bank = bank;
    this->userName = userName;
    this->accountNumber = accountNumber;
    this->availableFund = availableFund;
    this->password = password;
}

int Account::getFund()
{
    return availableFund;
}

Bank *Account::getBank()
{
    return bank;
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
    map<string, Account*> getAccountMap();
    

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

map<string, Account*> Bank::getAccountMap() {
    return account_info;
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
    newAccount = new Account(this, userName, accountNum, fund, password); //Account class에 password 추가
    account_info.insert(pair<string, Account*>(accountNum, newAccount));
    return newAccount;

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
    newAccount = new Account(this, userName, accountNum, fund, password); //Account class에 password 추가
    account_info.insert(pair<string, Account*>(accountNum, newAccount));
    return newAccount;

}

//------------------------------------------------------------------------------------------------

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
    int TransactionID;
    //---Account Information
    Account* usingAccount;
    string filePath = serial+"history.txt";
    
public:
    ATM(string bankname, string serialnum, bool SingleBank, bool Unilingual, int cashes);
    void showInfo(string val);
    void readCardInfo(Card* card);
    int startSession();
    void endSession(); // REQ2.2에 써먹기, 세션 종료 시 모든 카드 데이터 삭제
    bool checkExceptionalCondition(); // REQ2.2에 써먹기 및 9번
    void selectLanguage(bool isUnilingual); //REQ1.3, true일 경우 그냥 0 리턴, false일 경우 영어 선택시 0 리턴, false일 경우 한국어 선택시 1 리턴

    void deposit(); //4번
    void withdrawal(); //5번
    void transfer(); //6번
    void showHistory(); //7번
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
    isPrimaryBank = (primaryBankName==card->get_bank()->getBankName());
    usingAccount = card->get_account();
    if (usingAccount == NULL)
        isAdmin = true;
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

int ATM::startSession() {
    selectLanguage(isUnilingual);
    if(isEnglish==true) {
        cout << "Welcome\nTo start, please insert your debit card." << endl;
    } else {
        cout << "반갑습니다.\n시작하려면 카드를 기기에 넣어주십시오." << endl;
    }
    if(isSingleBank==true && isPrimaryBank==false) {
        if(isEnglish==true) {
            cout << "The Card is invalid" << endl;
        } else {
            cout << "사용할 수 없는 카드입니다." << endl;
        }
        endSession();
    }
    for (int i=0; i < 3; i++){
        Account* acc = usingAccount->getBank()->returnAccount(usingAccount->getNum());
         if (acc == nullptr)
             ;
         else
             break;
             return 0;
     }
     endSession();
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
    string message;
    int moneyArr[4];
    int amountOfCheck;
    if(isEnglish==true) {
        cout << "Please choose between cash or check." << endl;
        cout << "Cash: 0, Check: 1" << endl;
        cin >> isCheck;
        if(isCheck==false) {
            cout << "Please enter the number of 50,000 won notes to be deposited." << endl;
            cin >> moneyArr[0];
            cout << "Please enter the number of 10,000 won notes to be deposited." << endl;
            cin >> moneyArr[1];
            cout << "Please enter the number of 5,000 won notes to be deposited." << endl;
            cin >> moneyArr[2];
            cout << "Please enter the number of 1,000 won notes to be deposited." << endl;
            cin >> moneyArr[3];
            if(moneyArr[0]+moneyArr[1]+moneyArr[2]+moneyArr[3]>50) {
                cout << "Too many banknotes" << endl;
                endSession();
                return;
            }
            depositMoney=50000*moneyArr[0]+10000*moneyArr[1]+5000*moneyArr[2]+1000*moneyArr[3];
            
        } else {
            cout << "Please enter the amount of checks to deposit." << endl;
            cin >> amountOfCheck;
            if(amountOfCheck>30) {
                cout << "Too many checks" << endl;
                endSession();
                return;
            }
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
            cout << "입금할 5만원권 장수를 입력해주세요." << endl;
            cin >> moneyArr[0];
            cout << "입금할 만원권 장수를 입력해주세요" << endl;
            cin >> moneyArr[1];
            cout << "입금할 5천원권 장수를 입력해주세요" << endl;
            cin >> moneyArr[2];
            cout << "입금할 천원권 장수를 입력해주세요" << endl;
            cin >> moneyArr[3];
            if(moneyArr[0]+moneyArr[1]+moneyArr[2]+moneyArr[3]>50) {
                cout << "지폐가 너무 많습니다." << endl;
                endSession();
                return;
            }
            depositMoney=50000*moneyArr[0]+10000*moneyArr[1]+5000*moneyArr[2]+1000*moneyArr[3];
            
        } else {
            cout << "입금할 수표의 장수를 입력해주세요." << endl;
            cin >> amountOfCheck;
            if(amountOfCheck>30) {
                cout << "수표가 너무 많습니다." << endl;
                endSession();
                return;
            }
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
    TransactionID += 1;
    message = to_string(TransactionID) + ": "+ usingAccount->getNum() + " deposit " + to_string(depositMoney) + "\n"; 
    history += message;
    ofstream writeFile(filePath.data());
    if (writeFile.is_open() ){
        writeFile << message;
        writeFile.close();
    }
}

void ATM::withdrawal() {
    int withdrawalMoney;
    int includingFee;
    string message;
    int cnt = 0;
    string keepGoing;

    if(isPrimaryBank) {
        includingFee = 1000;
    } else {
        includingFee = 2000;
    }

    while(1) {
        if(isEnglish==true) {
            if(cnt!=0) {
                cout << "If you want further action, please enter [Yes]" << endl;
                cin >> keepGoing;
                if(keepGoing!="Yes" && keepGoing!="yes") {
                    cout << "Session ended" << endl;
                    endSession();
                    return;
                }
            }
            if(cnt==3) {
                cout << "You cannot withdraw more than 3 times at once" << endl;
                endSession();
                return;
            }

            cout << "Please enter the amount of fund to withdraw." << endl;
            cin >> withdrawalMoney;
            while(withdrawalMoney%1000!=0){
                cout << "You can use only banknotes" << endl;
                cout << "Please enter the amount of fund to withdraw." << endl;
                cin >> withdrawalMoney;
            }
            if(withdrawalMoney>500000) {
                cout << "Withdrawal limit exceeded" << endl;
                endSession();
                return;
            }
            includingFee += withdrawalMoney;
            if(amountOfCashes<includingFee) {
                cout << "Sorry, This ATM does not have enough money in it. " << endl;
                endSession();
                return;
            }
            usingAccount -= includingFee;
            cout << "Your withdrawal has been successful." << endl;

        } else {
            if(cnt!=0) {
                cout << "추가 작업을 원한다면 [Yes]를 입력해주세요." << endl;
                cin >> keepGoing;
                if(keepGoing!="Yes" && keepGoing!="yes") {
                    cout << "세션 종료됨" << endl;
                    endSession();
                    return;
                }
            }
            if(cnt==3) {
                cout << "3번까지만 출금할 수 있습니다." << endl;
                endSession();
                return;
            }
            cout << "출금할 금액을 입력해주세요." << endl;
            cin >> withdrawalMoney;
            while(withdrawalMoney%1000!=0){
                cout << "지폐만 사용할 수 있습니다." << endl;
                cout << "출금할 금액을 입력해주세요." << endl;
                cin >> withdrawalMoney;
            }
            if(withdrawalMoney>500000) {
                cout << "출금한도 초과" << endl;
                endSession();
                return;
            }
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
        TransactionID += 1;
        message = to_string(TransactionID) + ": "+ usingAccount->getNum() + " withdrawal " + to_string(withdrawalMoney) + "\n"; 
        history += message;
        ofstream writeFile(filePath.data());
        if (writeFile.is_open()){
            writeFile << message;
            writeFile.close();
        }
        cnt += 1;
    
    }
}

void ATM::transfer() {
    string accNum;
    string transferBankName;
    Bank* transferBank;
    Account* transferAccount;
    map<string, Account*> account_info;
    string message;
    int moneyArr[4];
    bool isCashTf;
    int transferMoney;
    int transferFee;
    if(isEnglish==true) {
        cout << "Please choose between Account Transfer or Cash Transfer" << endl;
        cout << "Account Transfer: 0, Cash Transfer: 1" << endl;
        cin >> isCashTf;
        
        

        while(1) {
            int cnt = 0;
            string transferBankName;
            cout << "Please enter the bank of the account you want to transfer money to." << endl;
            cin >> transferBankName;
            if(bankmap[transferBankName]!=NULL) {
                transferBank = &bankmap[transferBankName];
                account_info = transferBank->getAccountMap();
            } else {
                cout << "Please enter an appropriate name of bank." << endl;
                cnt += 1;
            }
            if(cnt==3) {
                cout << "Session ended: Please get your card" << endl;
                endSession();
                return;
            }
        }

        
        while(1) {
            int cnt = 0;
            cout << "Please enter the number of the account you want to transfer money to." << endl;
            cin >> accNum;
            if(account_info[accNum]!=NULL) {
                transferAccount = account_info[accNum];
                break;
            } else {
                cout << "Please enter an appropriate number of account." << endl;
                cnt += 1;
            }
            if (cnt==3) {
                cout << "Session ended: Please get your card" << endl;
                endSession();
                return;
            }
        }
       
        if(isCashTf) {
            cout << "Please enter the number of 50,000 won notes to be deposited." << endl;
            cin >> moneyArr[0];
            cout << "Please enter the number of 10,000 won notes to be deposited." << endl;
            cin >> moneyArr[1];
            cout << "Please enter the number of 5,000 won notes to be deposited." << endl;
            cin >> moneyArr[2];
            cout << "Please enter the number of 1,000 won notes to be deposited." << endl;
            cin >> moneyArr[3];
            if(moneyArr[0]+moneyArr[1]+moneyArr[2]+moneyArr[3]>50) {
                cout << "Too many banknotes" << endl;
                endSession();
                return;
            }
            transferMoney=50000*moneyArr[0]+10000*moneyArr[1]+5000*moneyArr[2]+1000*moneyArr[3];
            if(transferBankName == primaryBankName) transferFee = 2000;
            else transferFee = 3000;
            cout << "Please insert "<< transferMoney+transferFee << ". (including transfer fee)" << endl;
            
        } else {
            cout << "Please enter the amount of money to transfer." << endl;
            cin >> transferMoney;
            // Transfer Case 판별
            if(isPrimaryBank && transferBankName==primaryBankName) {
                transferFee = 2000;
            } else if((isPrimaryBank&&transferBankName!=primaryBankName) || (isPrimaryBank==false&&transferBankName==primaryBankName)) {
                transferFee = 3000;
            } else {
                transferFee = 4000;
            }
            if(transferMoney+transferFee>usingAccount->getFund()) {
                cout << "Sorry, there are not enough funds in the account." << endl;
                endSession();
                return;
            }
        }

        
        
    } else {
        cout << "계좌 송금을 할 지, 현금 송금을 할지 선택해주세요." << endl;
        cout << "계좌 송금: 0, 현금 송금: 1" << endl;
        cin >> isCashTf;
        
        

        while(1) {
            int cnt = 0;
            string transferBankName;
            cout << "송금할 계좌의 은행명을 입력해주세요." << endl;
            cin >> transferBankName;
            if(bankmap[transferBankName]!=NULL) {
                transferBank = &bankmap[transferBankName];
                account_info = transferBank->getAccountMap();
            } else {
                cout << "정확한 은행명을 입력해주세요." << endl;
                cnt += 1;
            }
            if(cnt==3) {
                cout << "세션이 종료되었습니다.: 카드를 가져가주세요." << endl;
                endSession();
                return;
            }
        }

        
        while(1) {
            int cnt = 0;
            cout << "송금할 계좌번호를 입력해주세요." << endl;
            cin >> accNum;
            if(account_info[accNum]!=NULL) {
                transferAccount = account_info[accNum];
                break;
            } else {
                cout << "정확한 계좌번호를 입력해주세요." << endl;
                cnt += 1;
            }
            if (cnt==3) {
                cout << "세션이 종료되었습니다.: 카드를 가져가주세요." << endl;
                endSession();
                return;
            }
        }
       
        if(isCashTf) {
            cout << "입금할 5만원권 장수를 입력해주세요." << endl;
            cin >> moneyArr[0];
            cout << "입금할 만원권 장수를 입력해주세요." << endl;
            cin >> moneyArr[1];
            cout << "입금할 5천원권 장수를 입력해주세요." << endl;
            cin >> moneyArr[2];
            cout << "입금할 천원권 장수를 입력해주세요." << endl;
            cin >> moneyArr[3];
            if(moneyArr[0]+moneyArr[1]+moneyArr[2]+moneyArr[3]>50) {
                cout << "지폐가 너무 많습니다." << endl;
                endSession();
                return;
            }
            transferMoney=50000*moneyArr[0]+10000*moneyArr[1]+5000*moneyArr[2]+1000*moneyArr[3];
            if(transferBankName == primaryBankName) transferFee = 2000;
            else transferFee = 3000;
            cout << "총액 "<< transferMoney+transferFee << "을 기기에 넣어주세요. (송금 수수료 포함)" << endl;
            
        } else {
            cout << "송금할 금액을 입력해주세요." << endl;
            cin >> transferMoney;
            // Transfer Case 판별
            if(isPrimaryBank && transferBankName==primaryBankName) {
                transferFee = 2000;
            } else if((isPrimaryBank&&transferBankName!=primaryBankName) || (isPrimaryBank==false&&transferBankName==primaryBankName)) {
                transferFee = 3000;
            } else {
                transferFee = 4000;
            }
            if(transferMoney+transferFee>usingAccount->getFund()) {
                cout << "죄송합니다만 계좌에 충분한 금액이 들어있지 않습니다." << endl;
                endSession();
                return;
            }
        }
        
    }
    if(isCashTf==false) usingAccount -= (transferMoney+transferFee);
    transferAccount += transferMoney;
    TransactionID += 1;
    if(isCashTf==false) message = to_string(TransactionID) + ": "+ usingAccount->getNum() + " transfer to " + transferAccount->getNum()+ to_string(transferMoney) + "\n"; 
    else message = to_string(TransactionID) + ": "+ serial + " transfer to " + transferAccount->getNum()+ to_string(transferMoney) + "\n"; 
    history += message;

    ofstream writeFile(filePath.data());
    if (writeFile.is_open() ){
        writeFile << message;
        writeFile.close();
    }
}

void ATM::endSession() {
    usingAccount = NULL;
}

map<string, Bank> bankmap;
