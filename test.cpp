void main() {

    //Initial Conditions
    bankmap.insert(pair<string, Bank>("Kakao", Bank("Kakao")));
    bankmap.insert(pair<string, Bank>("Shinhan", Bank("Shinhan")));
    
    Account* Account1 = bankmap.at("Kakao").openAccount();
    Account* Account1 = bankmap.at("Shinhan").openAccount();
    Account* Account1 = bankmap.at("Kakao").openAccount();
    
    ATM* ATM1 = new ATM("Kakao", "111111", true, true, 50000);
    ATM* ATM2 = new ATM("Shinhan", "222222", false, false, 0);
    ATM* ATM1 = new ATM("Kakao", "333333", false, false, 2000);

    //Test Case : Action1
    ATM1->startSession();



    //Test Case : Action2


    //Test Case : Action3





}
