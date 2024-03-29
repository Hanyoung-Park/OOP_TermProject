#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

class ATM;
class Bank;
class Account;

int fee[7] = {1000, 0, 1000, 2000, 2000, 3000, 4000};
map<string, Bank*> bankmap;

void init_fee() {
    cout << "Please type the deposit fee for non-primary banks" << endl;
    cin >> fee[0];
    cout << "Please type the deposit fee for primary banks" << endl;
    cin >> fee[1];
    cout << "Please type the withdrawal fee for non-primary banks" << endl;
    cin >> fee[2];
    cout << "Please type the withdrawal fee for non-primary banks" << endl;
    cin >> fee[3];
    cout << "Please type the transfer fee between primary banks" << endl;
    cin >> fee[4];
    cout << "Please type the transfer fee between primary and non-primary banks" << endl;
    cin >> fee[5];
    cout << "Please type the transfer fee between non-primary banks" << endl;
    cin >> fee[6];
}

//-----------------------------------------------------------------------------

class Account
{
protected:
    string userName;
    string accountNumber;
    string password;
    Bank* bank;
    int availableFund;
    bool isAdmin;

public:
    Account();
    virtual bool admin();
    int getFund();
    Bank* getBank();
    string getNum();
    string getPassword();
    string getuser();
};

Account::Account(){}

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

bool Account::admin(){
    return isAdmin;
}

string Account::getuser() {
    return userName;
}

class normalAccount : public Account {
    public:
        normalAccount(Bank* bank, string userName, string accountNumber, int availableFund, string password);
        Account &operator+=(int amount);
        Account &operator-=(int amount);
        bool admin() override;
};

normalAccount::normalAccount(Bank* bank, string userName, string accountNumber, int availableFund, string password) {
    this->isAdmin = false;
    this->bank = bank;
    this->userName = userName;
    this->accountNumber = accountNumber;
    this->availableFund = availableFund;
    this->password = password;
}

Account &normalAccount::operator+=(int amount)
{
    availableFund += amount;
    return *this;
}

Account &normalAccount::operator-=(int amount)
{
    availableFund -= amount;
    return *this;
}

bool normalAccount::admin() {
    return isAdmin;
}

class Admin : public Account {
    public:
        Admin(Bank* bank, string userName, string accountNumber);
        bool admin() override;
};

Admin::Admin(Bank* bank, string userName, string accountNumber) {
    this->isAdmin = true;
    this->bank = bank;
    this->userName = userName;
    this->accountNumber = accountNumber;
}

bool Admin::admin() {
    cout << "admin card insert" << endl;
    return isAdmin;

}

//-----------------------------------------------------------------------------
class Bank {
private:
    string bankName;
    map<string, normalAccount*> account_info;
    map<string, Admin*> admin_info;

public:
    Bank(string bankName);
    ~Bank();
    int validPassword(normalAccount userAccount, int num);
    normalAccount* returnAccount(string accountNumber, bool English);
    normalAccount* openAccount(bool isEnglish=true);
    string getBankName();
    map<string, normalAccount*> getAccountMap();
    map<string, Admin*> getAdminMap();
    normalAccount* initAccount(string bank, string user, string acc, string pass, int fund);
    Admin* initAdminAcc(Bank* bank, string name, string acc);

};


Bank::Bank(string bankName) {
    this->bankName = bankName;
    account_info = map<string, normalAccount*>();
    admin_info = map<string, Admin*>();

}

Bank::~Bank() {}

string Bank::getBankName() {
    return bankName;   
}

normalAccount* Bank::returnAccount(string accountNumber, bool English) {
    string inputpassword;
    if (English == true)
        cout << "Password?" << endl;
    else    
        cout << "비밀번호를 입력하여 주십시오" << endl;
    cin >> inputpassword;
    if (account_info[accountNumber]->getPassword() == inputpassword) {
        return account_info[accountNumber];
    }
    return nullptr;
}

map<string, normalAccount*> Bank::getAccountMap() {
    return account_info;
}

map<string, Admin*> Bank::getAdminMap() {
    return admin_info;
}

normalAccount* Bank::openAccount(bool isEnglish) {
    string userName;
    string accountNum;
    string password;
    int fund = 0;
    if (isEnglish){
        cout << "input User Name: " << endl;
        cin >> userName;
        cout << "input Account Number(12-digit): " << endl;
        cin >> accountNum;
        cout << "input Password: " << endl;
        cin >> password;
    }else{
        cout << "이름을 입력하여 주십시오: " << endl;
        cin >> userName;
        cout << "계좌번호를 입력하여 주십시오(12자리): " << endl;
        cin >> accountNum;
        cout << "비밀번호를 입력하여 주십시오: " << endl;
        cin >> password;
    }

    normalAccount* newAccount;
    newAccount = new normalAccount(this, userName, accountNum, fund, password); //Account class에 password 추가
    account_info.insert(pair<string, normalAccount*>(accountNum, newAccount));
    return newAccount;
}

normalAccount* Bank::initAccount(string bank, string user, string acc, string pass, int fund) {
    normalAccount* newAccount;
    newAccount = new normalAccount(this, user, acc, fund, pass); //Account class에 password 추가
    account_info.insert(pair<string, normalAccount*>(acc, newAccount));
    return newAccount;
}
Admin* Bank::initAdminAcc(Bank* bank, string name, string acc) {
    Admin* newAdmin;
    newAdmin = new Admin(this, name, acc); 
    admin_info.insert(pair<string, Admin*>(acc, newAdmin));
    return newAdmin;
}
//------------------------------------------------------------------------------------------------


class ATM {
private:
    string serial; // REQ1.1, 6-digit serial number
    bool isSingleBank; // REQ1.2, if the atm is single atm -> true
    bool isUnilingual; // REQ1.3
    int amountOfCashes; // REQ1.4 참조
    string history; // REQ1.9, History를 어떻게 담을지 고민. (어레이 형태?)
    string primaryBankName;
    bool isPrimaryBank;
    int TransactionID;
    //---Account Information
    normalAccount* usingAccount;
    string filePath = serial+"history.txt";
    int errorCheck=0; // Error checker
    int withdrawalCnt=0;
    
public:
    ATM(string bankname, string serialnum, bool SingleBank, bool Unilingual, int cashes);
    bool isEnglish;
    bool isAdmin;
    
    void readCardInfo(string accNum);
    void showInfo(string val);
    int startSession();
    void selectLanguage(); //REQ1.3, true일 경우 그냥 0 리턴, false일 경우 영어 선택시 0 리턴, false일 경우 한국어 선택시 1 리턴

    void deposit(); //4번
    void withdrawal(); //5번
    void transfer(); //6번
    void showHistory(); //7번
    
    int execute();
    int adminMenu();
    string getBankname();

    int getAmountOfCashes();
};

ATM::ATM(string bankname, string serialnum, bool SingleBank, bool Unilingual, int cashes) {
    
    // Reset history file
    ofstream writeFile(filePath.data());
    if (writeFile.is_open() ){
        writeFile << "";
        writeFile.close();
    }
    
    primaryBankName = bankname;
    serial = serialnum;
    isSingleBank = SingleBank;
    isUnilingual = Unilingual;
    amountOfCashes = cashes;
}

string ATM::getBankname() {
    return primaryBankName;
}


void ATM::readCardInfo(string accNum) {
    map<string, Bank*>::iterator it;
    int valid = 0;
    int n;
    for (it = bankmap.begin(); it!= bankmap.end(); it++) {
        map<string, normalAccount*> tempmap;
        tempmap = it->second->getAccountMap();
        map<string, normalAccount*>::iterator it2;
        for (it2 = tempmap.begin(); it2!= tempmap.end(); it2++) {
            if(accNum == it2->second->getNum()) {
                usingAccount = it2->second;
                valid = 1;
            }
        }
        if (!valid & (it->first ==this->getBankname())) {
            map<string, Admin*> adminTempmap;
            adminTempmap = it->second->getAdminMap();
            map<string, Admin*>::iterator it3;
            for (it3 = adminTempmap.begin(); it3!=adminTempmap.end(); it3++) {
                if(it3->second->getNum() == accNum) {
                    isAdmin = true;
                    valid = 2;
                    break;
                }
            }

        }
    }

    if (valid == 0){
        if(isEnglish)
            cout << "Error: No account found, returning card\n" << endl;
        else
            cout << "오류: 존재하지 않는 계좌입니다. 카드를 반환하겠습니다\n" << endl;

        errorCheck = 1;
    }
    else if (valid == 2){
        return;
    }

    try {
        if (valid == 0) {
            throw n;
        }
        else{
        isPrimaryBank = (primaryBankName==usingAccount->getBank()->getBankName());
        }
    }
    catch (int ex) {

    }
}

void ATM::showInfo(string val) {
    if(isAdmin == false) {
        cout << "Wrong Approach" << endl;
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
        return;
    }
    if(history=="") {
        cout << "No record" << endl;
        return;
    }
    cout << history << endl;
}

int ATM::startSession() {
    withdrawalCnt = 0;
    errorCheck = 0;
    isAdmin = 0;

    if(isEnglish) {
        cout << primaryBankName << " ATM" << endl;
        cout << boolalpha << "Unilingual: " << isUnilingual << endl;
        cout << boolalpha << "Single-ATM: " << isSingleBank << endl;
        cout << "Welcome\nTo start, please insert your debit card." << endl;
        cout << "Insert your card" << endl;
    }
    else{
        cout << "반갑습니다.\n시작하려면 카드를 기기에 넣어주십시오." << endl;
        cout << "카드를 삽입해주세요. " << endl;
        }


    string accNum;
    cin >> accNum;
    readCardInfo(accNum);
    if(isSingleBank==true && isPrimaryBank==false) {
        if(isEnglish==true) {
            cout << "The Card is invalid\n" << endl;
        } else {
            cout << "사용할 수 없는 카드입니다.\n" << endl;
        }
        return 1;
    }
    if(errorCheck==1) return 1;
    
    if(isAdmin){
        return 0;
    }

    int i = 0;
    while(i < 3){
        normalAccount* acc = usingAccount->getBank()->returnAccount(usingAccount->getNum(), isEnglish);
        if (acc == nullptr){
            if (isEnglish) 
                cout << "Wrong password, Please enter your password again" << endl;
            else
                cout << "잘못된 비밀번호입니다. 비밀번호를 다시 입력해주세요." << endl;
            i++;

        }else{
             return 0;
        }
     }
     return 1;
}
void ATM::selectLanguage() {
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
            return;
        } else if(language=="E" || language=="[E]" || language=="e" || language=="[e]") {
            cout << "It is set to English." << endl;
            isEnglish = true;
            return;
        } else {
            cout << "Please enter the correct character." << endl;
            cout << "Return to the initial screen." << endl;
            selectLanguage();
        }
    }
}

void ATM::deposit() {
    int depositMoney;
    int isCheck;
    string message;
    int moneyArr[4];
    int amountOfCheck;
    if(isEnglish==true) {
        cout << "Please choose between cash or check." << endl;
        cout << "Cash: 0, Check: 1, Cancel: 2" << endl;
        cin >> isCheck;
        if(isCheck==2) {
            cout << "Canceled" << endl;
            return;
        }
        if(isCheck==0) {
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
                return;
            }
            depositMoney=50000*moneyArr[0]+10000*moneyArr[1]+5000*moneyArr[2]+1000*moneyArr[3];
            
        } else {
            cout << "Please enter the amount of checks to deposit." << endl;
            cin >> amountOfCheck;
            if(amountOfCheck>30) {
                cout << "Too many checks" << endl;
                return;
            }
            cout << "Please enter the amount of fund to deposit." << endl;
            cin >> depositMoney;
        }
        cout << "If you want to cancel, please type [Y]" << endl;
        cout << "If you don't want to cancel, please type [N]" << endl;
        char temp;
        cin >> temp;
        if(temp=='Y' || temp=='y') {
            cout << "Canceled" << endl;
            return;
        }
        *usingAccount += depositMoney;
        cout << "Your deposit has been succesful." << endl;
        // cout << "Changed balance is " << usingAccount->getFund() << endl;
    } else {
        cout << "현금과 수표 중 사용하실 방법을 선택해주세요." << endl;
        cout << "현금: 0, 수표: 1, 취소: 2" << endl;
        cin >> isCheck;
        if(isCheck==2) {
            cout << "거래가 취소되었습니다." << endl;
            return;
        }
        if(isCheck==0) {
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
                return;
            }
            depositMoney=50000*moneyArr[0]+10000*moneyArr[1]+5000*moneyArr[2]+1000*moneyArr[3];
            
        } else {
            cout << "입금할 수표의 장수를 입력해주세요." << endl;
            cin >> amountOfCheck;
            if(amountOfCheck>30) {
                cout << "수표가 너무 많습니다." << endl;
                return;
            }
            cout << "입금할 금액을 입력해주세요." << endl;
            cin >> depositMoney;
        }
        cout << "취소를 원하신다면 [Y]를 입력해주세요." << endl;
        cout << "거래 진행을 원하신다면 [N]를 입력해주세요." << endl;
        char temp;
        cin >> temp;
        if(temp=='Y' || temp=='y') {
            cout << "거래가 취소되었습니다." << endl;
            return;
        }
        *usingAccount += depositMoney;
        cout << "입금이 성공적으로 완료되었습니다." << endl;
    }
    if(isCheck==false) {
        amountOfCashes += depositMoney;
    }
    int Fee;
    if(isPrimaryBank) {
        Fee = fee[1];
    } else {
        Fee = fee[0];
    }
    *usingAccount -= Fee;
    TransactionID += 1;
    message = to_string(TransactionID) + " | user: " +usingAccount->getuser() +" "+ usingAccount->getNum() + " deposit " + to_string(depositMoney) + "\n"; 
    history += message;
    if(isEnglish==true){
        cout <<"["<< usingAccount->getNum() << "] "<< "Changed balance is " << usingAccount->getFund() << endl;
        cout << "Deposit fee: " << Fee << endl;
    }else{
        cout <<"["<< usingAccount->getNum() << "] "<< "거래 후 잔액: " << usingAccount->getFund() << endl;
        cout << "입금 수수료: " << Fee << endl;
    }
    ofstream outfile;
    outfile.open(filePath, ios_base::app);
    outfile << message;
    outfile.close();
}

void ATM::withdrawal() {
    int withdrawalMoney;
    int withdrawalFee;
    string message;


    if(isPrimaryBank) {
        withdrawalFee = fee[2];
    } else {
        withdrawalFee = fee[3];
    }


    if(isEnglish==true) {
        if(withdrawalCnt==3) {
            cout << "You cannot withdraw more than 3 times at once" << endl;
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
            return;
        }
        if(amountOfCashes<withdrawalFee+withdrawalMoney) {
            cout << "Sorry, This ATM does not have enough money in it. " << endl;
            return;
        }
        if(usingAccount->getFund()<withdrawalFee+withdrawalMoney) {
            cout << "Sorry, there are not enough funds in the account. " << endl;
            return;
        }
        cout << "If you want to cancel, please type [Y]" << endl;
        cout << "If you don't want to cancel, please type [N]" << endl;
        char temp;
        cin >> temp;
        if(temp=='Y' || temp=='y') {
            cout << "Canceled" << endl;
            return;
        }
        *usingAccount -= withdrawalFee+withdrawalMoney;
        cout << "Your withdrawal has been successful." << endl;
        cout << "HERE" << endl;
        cout << "Changed balance is " << usingAccount->getFund() << endl;

    } else {
        if(withdrawalCnt==3) {
            cout << "3번까지만 출금할 수 있습니다." << endl;
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
            return;
        }
        if(amountOfCashes<withdrawalFee+withdrawalMoney) {
            cout << "죄송합니다만 ATM에 충분한 금액이 들어있지 않습니다." << endl;
            return;
        }
        if(usingAccount->getFund()<withdrawalFee+withdrawalMoney) {
            cout << "죄송합니다만 계좌에 충분한 금액이 들어있지 않습니다." << endl;
            return;
        }
        cout << "취소를 원하신다면 [Y]를 입력해주세요." << endl;
        cout << "거래 진행을 원하신다면 [N]를 입력해주세요." << endl;
        char temp;
        cin >> temp;
        if(temp=='Y' || temp=='y') {
            cout << "거래가 취소되었습니다." << endl;
            return;
        } 
        *usingAccount -= withdrawalFee+withdrawalMoney;
        cout << "출금이 성공적으로 완료되었습니다." << endl;
    }

    amountOfCashes -= withdrawalMoney;   
    TransactionID += 1;
    message = to_string(TransactionID) + " | user: " +usingAccount->getuser() +" "+ usingAccount->getNum() + " withdrawal " + to_string(withdrawalMoney) + "\n"; 
    history += message;
    if(isEnglish==true) {
    cout <<"["<< usingAccount->getNum() << "] "<< "Changed balance is " << usingAccount->getFund() << endl;
    cout << "Withdrawal Fee: " << withdrawalFee << endl;
    } else {
    cout <<"["<< usingAccount->getNum() << "] "<< "거래 후 잔액: " << usingAccount->getFund() << endl;
    cout << "출금 수수료: " << withdrawalFee << endl;
    }
    ofstream outfile;
    outfile.open(filePath, ios_base::app);
    outfile << message;
    outfile.close();
    withdrawalCnt += 1;
    
    
}

void ATM::transfer() {
    string accNum;
    string transferBankName;
    Bank* transferBank;
    normalAccount* transferAccount;
    map<string, normalAccount*> account_info;
    string message;
    int moneyArr[4];
    int isCashTf;
    int transferMoney;
    int transferFee;
    if(isEnglish==true) {
        cout << "Please choose between Account Transfer or Cash Transfer" << endl;
        cout << "1: Account Transfer, 2: Cash Transfer, 3: Cancel" << endl;
        cin >> isCashTf;
        if(isCashTf==3) {
            cout << "Canceled" << endl;
            return;
        }
        

        while(1) {
            int cnt = 0;
            cout << "Please enter the bank of the account you want to transfer money to." << endl;
            cin >> transferBankName;
            if(bankmap[transferBankName]!=NULL) {
                transferBank = bankmap[transferBankName];
                account_info = transferBank->getAccountMap();
                break;
            } else {
                cout << "Please enter an appropriate name of bank." << endl;
                cnt += 1;
            }
            if(cnt==3) {
                cout << "Session ended: Please get your card" << endl;
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
                return;
            }
        }
       
        if(isCashTf==2) {
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
                return;
            }
            transferMoney=50000*moneyArr[0]+10000*moneyArr[1]+5000*moneyArr[2]+1000*moneyArr[3];
            if(transferBankName == primaryBankName) transferFee = fee[4];
            else transferFee = fee[5];
            cout << "Please insert "<< transferMoney+transferFee << ". (including transfer fee)" << endl;
            string temp;
            cout << "Please check the amount you inserted: " << transferMoney+transferFee << endl;
            cout << "If it is not correct, please enter [C]" << endl;
            cout << "If it is correct, please enter [O]" << endl;
            cin >> temp;
            if(temp=="C" || temp=="c") {
                cout << "Canceled" << endl;
                return;
            }
            
        } else {
            cout << "Please enter the number of source account." << endl;
            string temp;
            cin >> temp;
            if(temp!=usingAccount->getNum()) {
                cout << "It is invalid account." << endl;
                return;
            }
            cout << "Please enter the amount of money to transfer." << endl;
            cin >> transferMoney;
            // Transfer Case 판별
            if(isPrimaryBank && transferBankName==primaryBankName) {
                transferFee = fee[4];
            } else if((isPrimaryBank&&transferBankName!=primaryBankName) || (isPrimaryBank==false&&transferBankName==primaryBankName)) {
                transferFee = fee[5];
            } else {
                transferFee = fee[6];
            }
            if(transferMoney+transferFee>usingAccount->getFund()) {
                cout << "Sorry, there are not enough funds in the account." << endl;
                return;
            }
            
        }
        cout << "Your transfer has been succesful." << endl;
        
        
    } else {
        cout << "계좌 송금을 할 지, 현금 송금을 할지 선택해주세요." << endl;
        cout << "1: 계좌 송금, 2: 현금 송금, 3: 취소" << endl;
        cin >> isCashTf;
        if(isCashTf==3) {
            cout << "취소되었습니다" << endl;
            return;
        }
        

        while(1) {
            int cnt = 0;
            cout << "송금할 계좌의 은행명을 입력해주세요." << endl;
            cin >> transferBankName;
            if(bankmap[transferBankName] != NULL) {
                transferBank = bankmap[transferBankName];
                account_info = transferBank->getAccountMap();
                break;
            } else {
                cout << "정확한 은행명을 입력해주세요." << endl;
                cnt += 1;
            }
            if(cnt==3) {
                cout << "세션이 종료되었습니다.: 카드를 가져가주세요." << endl;
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
                return;
            }
        }
       
        if(isCashTf==2) {
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
                return;
            }
            transferMoney=50000*moneyArr[0]+10000*moneyArr[1]+5000*moneyArr[2]+1000*moneyArr[3];
            if(transferBankName == primaryBankName) transferFee = 2000;
            else transferFee = 3000;
            cout << "총액 "<< transferMoney+transferFee << "을 기기에 넣어주세요. (송금 수수료 포함)" << endl;
            string temp;
            cout << "투입한 금액이 맞는지 확인해주세요.: " << transferMoney+transferFee << endl;
            cout << "금액이 맞지 않다면 [C]를 입력해주세요." << endl;
            cout << "금액이 맞다면 [O]를 입력해주세요." << endl;
            cin >> temp;
            if(temp=="C" || temp=="c") {
                cout << "취소되었습니다" << endl;
                return;
            }
        } else {
            cout << "출금할 계좌를 입력해주세요." << endl;
            string temp;
            cin >> temp;
            if(temp!=usingAccount->getNum()) {
                cout << "사용할 수 없는 계좌입니다." << endl;
                return;
            }

            cout << "송금할 금액을 입력해주세요." << endl;
            cin >> transferMoney;
            // Transfer Case 판별
            if(isPrimaryBank && transferBankName==primaryBankName) {
                transferFee = fee[4];
            } else if((isPrimaryBank&&transferBankName!=primaryBankName) || (isPrimaryBank==false&&transferBankName==primaryBankName)) {
                
                transferFee = fee[5];
            } else {
                transferFee = fee[6];
            }
            
            if(transferMoney+transferFee>usingAccount->getFund()) {
                cout << "죄송합니다만 계좌에 충분한 금액이 들어있지 않습니다." << endl;
                return;
            }
        }
        
    }
    if(isCashTf==1) {
        *usingAccount -= (transferMoney+transferFee);
    }
    *transferAccount += transferMoney;
    if(isCashTf==2) {
        amountOfCashes += (transferMoney+transferFee);
    }
    TransactionID += 1;
    if(isCashTf==1) message = to_string(TransactionID) + " | user: " +usingAccount->getuser() +" "+ usingAccount->getNum() + " transfer to " + transferAccount->getBank()->getBankName() + " " + transferAccount->getNum()+ " " + to_string(transferMoney) + "\n"; 
    else message = to_string(TransactionID) + " | user: " +usingAccount->getuser() +" "+ serial + " transfer to " + transferAccount->getBank()->getBankName() + " " + transferAccount->getNum()+ " " +to_string(transferMoney) + "\n"; 
    history += message;
    if(isEnglish==true){
        cout <<"["<< usingAccount->getNum() << "] "<< "Changed balance is " << usingAccount->getFund() << endl;
        cout << "["<< transferAccount->getNum() << "] "<<"Changed balance is " << transferAccount->getFund() << endl;
        cout << "Transfer fee: " << transferFee << endl;
    }else{
        cout <<"["<< usingAccount->getNum() << "] "<< "거래 후 잔액: " << usingAccount->getFund() << endl;
        cout << "["<< transferAccount->getNum() << "] "<< "거래 후 잔액: " << transferAccount->getFund() << endl;
        cout << "송금 수수료: " << transferFee << endl;
    }
    ofstream outfile;
    outfile.open(filePath, ios_base::app);
    outfile << message;
    outfile.close();
}

int ATM::adminMenu() {
    int work;
    string isUnilingualYN;
    string isSingleBankYN;
    while(1) {
        if(isEnglish) {
            cout << "Please select work what you want to do." << endl;
            cout << "1: Information of ATM, 2: History, 3: Cancel" << endl;
            cin >> work;
            switch (work) {
                case 1:
                    showInfo("serial");
                    showInfo("cash");
                    
                    if(isUnilingual) isUnilingualYN = "Y";
                    else isUnilingualYN = "N";
                    cout << "Unilingual: " << isUnilingualYN << endl;;
                    
                    if(isSingleBank) isSingleBankYN = "Y";
                    else isSingleBankYN = "N";
                    cout << "Single Bank: " << isSingleBankYN << endl;

                    // endSession();
                    break;
                case 2:
                    showHistory();
                    // endSession();
                    break;
                case 3:
                    cout << "Canceled" << endl;
                    return 0;
                default:
                    cout << "Wrong Approach" << endl;
                    break;
            }
        }
        else {
            cout << "원하는 작업을 골라주세요." << endl;
            cout << "1: ATM 정보, 2: 기록, 3: 취소" << endl;
            cin >> work;
            switch (work) {
                case 1:
                    showInfo("serial");
                    showInfo("cash");

                    cout << "시리얼 넘버: " << serial << endl;
                    
                    if(isUnilingual) isUnilingualYN = "Y";
                    else isUnilingualYN = "N";
                    cout << "유니링구얼 여부: " << isUnilingualYN << endl;;
                    
                    if(isSingleBank) isSingleBankYN = "Y";
                    else isSingleBankYN = "N";
                    cout << "싱글뱅크 여부: " << isSingleBankYN << endl;

                    // endSession();
                    break;
                case 2:
                    showHistory();
                    // endSession();
                    break;
                case 3:
                    cout << "취소" << endl;
                    return 0;
                default:
                    cout << "잘못된 접근입니다." << endl;
                    break;
            }

        }        
    }
    return 0;
    
}

int ATM::execute() {
    int cheack;
    int work;

    selectLanguage();
    try {
        if (startSession()) {
            throw cheack;
        }
    }
    catch (int n) {
        return 0;
    }
    while(1) {
        if(isAdmin) {
            adminMenu();
            return 0;
        }
        if(isEnglish) {
            cout << "Account balance: " << usingAccount->getFund() << endl;
            cout << "Please select work what you want to do." << endl;
            cout << "1: Deposit, 2: Withdrawal, 3: Transfer, 4: exit" << endl;
            cin >> work;
            switch (work) {
                case 1:
                    deposit();
                    break;
                case 2:
                    withdrawal();
                    break;
                case 3:
                    transfer();
                    break;
                case 4:
                    cout << "--Exit--" << endl;
                    this->isAdmin = true;
                    this->showHistory();
                    return 0;
                default:
                    cout << "Wrong Approach" << endl;
                    break;
            }
    } else {
            cout << "계좌 잔고: " << usingAccount->getFund() << endl;;
            cout << "원하는 작업을 선택해주세요" << endl;
            cout << "1: 입금, 2: 출금, 3: 송금, 4: 나가기" << endl;
            cin >> work;
            switch (work) {
                case 1:
                    deposit();
                    break;
                case 2:
                    withdrawal();
                    break;
                case 3:
                    transfer();
                    break;
                case 4:
                    cout << "--ATM 나감--" << endl;
                    this->isAdmin = true;
                    this->showHistory();
                    return 0;
                default:
                    cout << "잘못된 접근입니다. " << endl;
                    break;
            }
        }
    } 
    return 0;
}


int ATM::getAmountOfCashes() {
    return amountOfCashes;
}

int printAccountBalance(Account** accarr, int length) {
    int i;
    for(i=0;i<length;i++) {
        cout << "[Account " << i+1 << "] Balance: " << accarr[i]->getFund() << endl; 
    }
    return 0;    
}

int printATMCash(ATM** atmarr, int length) {
    int i;
    for(i=0;i<length;i++) {
        cout << "[ATM " << i+1 << "] Remaining Cash: " << atmarr[i]->getAmountOfCashes() << endl;
    }
    return 0;
}

ATM makeATM() {
    string bankname, serial;
    bool SingleBank, Unilingual;
    int cashes;
    cout << "Please enter the name of the bank" << endl;
    cin >> bankname;
    cout << "Please enter the serial of the ATM" << endl;
    cin >> serial;
    cout << "Is it single-bank ATM? If it is right, please enter [Y]" << endl;
    cout << "If it is multi-bank, please enter [N]" << endl;
    string temp1;
    cin >> temp1;
    if(temp1=="Y"||temp1=="y") SingleBank = true;
    else SingleBank = false;
    string temp2;
    cin >> temp2;
    cout << "Is it unilingual ATM? If it is right, please enter [Y]" << endl;
    cout << "If it is bilingual ATM, please enter [N]" << endl;
    if(temp2=="Y"||temp2=="y") Unilingual = true;
    else Unilingual = false;
    cout << "Please enter the amount of cashes in this ATM" << endl;
    cin >> cashes;

    return ATM(bankname, serial, SingleBank, Unilingual, cashes);
}
int main() {

    Bank Kakao = Bank("Kakao");
    Bank Daegu = Bank("Daegu");
    Bank Shinhan = Bank("Shinhan");

    //Initial Conditions
    bankmap.insert(pair<string, Bank*>("Kakao", &Kakao));
    bankmap.insert(pair<string, Bank*>("Daegu", &Daegu));
    bankmap.insert(pair<string, Bank*>("Shinhan", &Shinhan));

    Account* Account1 = bankmap.at("Kakao")->initAccount("Kakao", "David", "111-111-111111", "cooldavid", 5000);
    Account* Account2 = bankmap.at("Daegu")->initAccount("Daegu", "Jane", "222-222-222222", "cooljane", 5000);
    Account* Account3 = bankmap.at("Kakao")->initAccount("Kakao", "Kate", "333-333-333333", "coolkate", 5000);
    Account* Account4 = bankmap.at("Shinhan")->initAccount("Shinahn", "Daniel", "444-444-444444", "cooldan", 5000);

    Admin* kakao_account = bankmap.at("Kakao")->initAdminAcc(&Kakao, "Admin", "999-999-999999");    
    Admin* daegu_account = bankmap.at("Daegu")->initAdminAcc(&Daegu, "Admin", "888-888-888888");  

    ATM* ATM1 = new ATM("Kakao", "111111", false, true, 50000);
    ATM* ATM2 = new ATM("Daegu", "222222", false, false, 50000);
    ATM* ATM3 = new ATM("Daegu", "333333", false, false, 50000);

    //making array for print information
    const int numOfATM = 3;
    const int numOfAccount = 3;
    ATM* atmArray[numOfATM] = {ATM1, ATM2, ATM3};
    Account* accountArray[numOfAccount] = {Account1, Account2, Account3};

    // Testcase for opening account
    // Kakao.openAccount(true);

    //Test Case : Action1
    ATM1->execute();
    ATM1->execute();
    ATM1->execute();
    printAccountBalance(accountArray, numOfAccount);
    printATMCash(atmArray, numOfATM);
    //Test Case : Action2
    ATM2->execute();
    printAccountBalance(accountArray, numOfAccount);
    printATMCash(atmArray, numOfATM);

    //Test Case : Action3
    ATM3->execute();
    printAccountBalance(accountArray, numOfAccount);
    printATMCash(atmArray, numOfATM);

    return 0;
}
