#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  CLASS DEFINITIONS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**
 * @class Account
 * @brief An abstract base class representing a generic bank account.
 */
class Account {
protected:
    string accountNumber;
    string accountHolderName;
    double balance;

public:
    // Constructors and Destructor
    Account();
    Account(const string& num, const string& name, double bal);
    Account(const Account& other);
    virtual ~Account();

    // Pure virtual functions for core banking operations
    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
    virtual void display() const = 0;

    // Getters and Setters
    string getAccountNumber() const;
    string getAccountHolderName() const;
    double getBalance() const;
    void setBalance(double newBalance);

    // Operator overloading
    Account& operator+(double amount);
    Account& operator-(double amount);
    bool operator>(const Account& other) const;
    bool operator<(const Account& other) const;
    bool operator==(const Account& other) const;

    // Friend functions for stream operators and fund transfer
    friend ostream& operator<<(ostream& os, const Account& acc);
    friend istream& operator>>(istream& is, Account& acc);
    friend void transfer(Account& from, Account& to, double amount);
};

/**
 * @class SavingsAccount
 * @brief Represents a savings account, inheriting from Account.
 */
class SavingsAccount : public Account {
private:
    double interestRate;
    int withdrawalsThisMonth;
    static const int MAX_WITHDRAWALS = 3;

public:
    // Constructors
    SavingsAccount();
    SavingsAccount(const string& num, const string& name, double bal, double rate = 0.01);
    SavingsAccount(const SavingsAccount& other);

    // Overridden virtual functions
    void deposit(double amount) override;
    void withdraw(double amount) override;
    void display() const override;

    // Overloaded function for depositing with a bonus
    void deposit(double amount, double bonus);

    // Member function to apply interest
    void applyInterest();
};

/**
 * @class CheckingAccount
 * @brief Represents a checking account, inheriting from Account.
 */
class CheckingAccount : public Account {
private:
    double overdraftLimit;

public:
    // Constructors
    CheckingAccount();
    CheckingAccount(const string& num, const string& name, double bal, double limit = 500.0);
    CheckingAccount(const CheckingAccount& other);

    // Overridden virtual functions
    void deposit(double amount) override;
    void withdraw(double amount) override;
    void display() const override;
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  METHOD IMPLEMENTATIONS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// --- Account Implementation ---

Account::Account() : accountNumber(""), accountHolderName(""), balance(0.0) {}

Account::Account(const string& num, const string& name, double bal)
    : accountNumber(num), accountHolderName(name), balance(bal) {}

Account::Account(const Account& other)
    : accountNumber(other.accountNumber), accountHolderName(other.accountHolderName), balance(other.balance) {}

Account::~Account() {}

string Account::getAccountNumber() const { return accountNumber; }
string Account::getAccountHolderName() const { return accountHolderName; }
double Account::getBalance() const { return balance; }
void Account::setBalance(double newBalance) { balance = newBalance; }

Account& Account::operator+(double amount) {
    this->deposit(amount);
    return *this;
}

Account& Account::operator-(double amount) {
    this->withdraw(amount);
    return *this;
}

bool Account::operator>(const Account& other) const { return this->balance > other.balance; }
bool Account::operator<(const Account& other) const { return this->balance < other.balance; }
bool Account::operator==(const Account& other) const { return this->balance == other.balance; }

// --- SavingsAccount Implementation ---

SavingsAccount::SavingsAccount()
    : Account(), interestRate(0.01), withdrawalsThisMonth(0) {}

SavingsAccount::SavingsAccount(const string& num, const string& name, double bal, double rate)
    : Account(num, name, bal), interestRate(rate), withdrawalsThisMonth(0) {}

SavingsAccount::SavingsAccount(const SavingsAccount& other)
    : Account(other), interestRate(other.interestRate), withdrawalsThisMonth(other.withdrawalsThisMonth) {}

void SavingsAccount::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        cout << "Deposited $" << fixed << setprecision(2) << amount << " to Savings Account #" << accountNumber << endl;
    } else {
        cout << "Deposit amount must be positive." << endl;
    }
}

void SavingsAccount::deposit(double amount, double bonus) {
    if (amount > 0 && bonus >= 0) {
        balance += amount + bonus;
        cout << "Deposited $" << fixed << setprecision(2) << amount << " + bonus $" << bonus << " to Savings Account #" << accountNumber << endl;
    } else {
        cout << "Deposit and bonus amounts must be positive." << endl;
    }
}

void SavingsAccount::withdraw(double amount) {
    if (withdrawalsThisMonth >= MAX_WITHDRAWALS) {
        cout << "Withdrawal limit reached for Savings Account #" << accountNumber << endl;
        return;
    }
    if (amount > 0) {
        if (balance >= amount) {
            balance -= amount;
            withdrawalsThisMonth++;
            cout << "Withdrew $" << fixed << setprecision(2) << amount << " from Savings Account #" << accountNumber << endl;
        } else {
            cout << "Insufficient funds for withdrawal from Savings Account #" << accountNumber << endl;
        }
    } else {
        cout << "Withdrawal amount must be positive." << endl;
    }
}

void SavingsAccount::applyInterest() {
    double interest = balance * interestRate;
    balance += interest;
    cout << "Interest $" << fixed << setprecision(2) << interest << " applied to Savings Account #" << accountNumber << endl;
    withdrawalsThisMonth = 0; // Reset withdrawal count
}

void SavingsAccount::display() const {
    cout << "Savings Account #" << accountNumber
         << ", Holder: " << accountHolderName
         << ", Balance: $" << fixed << setprecision(2) << balance
         << ", Withdrawals this month: " << withdrawalsThisMonth << endl;
}

// --- CheckingAccount Implementation ---

CheckingAccount::CheckingAccount()
    : Account(), overdraftLimit(500.0) {}

CheckingAccount::CheckingAccount(const string& num, const string& name, double bal, double limit)
    : Account(num, name, bal), overdraftLimit(limit) {}

CheckingAccount::CheckingAccount(const CheckingAccount& other)
    : Account(other), overdraftLimit(other.overdraftLimit) {}

void CheckingAccount::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        cout << "Deposited $" << fixed << setprecision(2) << amount << " to Checking Account #" << accountNumber << endl;
    } else {
        cout << "Deposit amount must be positive." << endl;
    }
}

void CheckingAccount::withdraw(double amount) {
    if (amount > 0) {
        if (balance + overdraftLimit >= amount) {
            balance -= amount;
            cout << "Withdrew $" << fixed << setprecision(2) << amount << " from Checking Account #" << accountNumber << endl;
        } else {
            cout << "Withdrawal failed. Amount exceeds overdraft limit for Checking Account #" << accountNumber << endl;
        }
    } else {
        cout << "Withdrawal amount must be positive." << endl;
    }
}

void CheckingAccount::display() const {
    cout << "Checking Account #" << accountNumber
         << ", Holder: " << accountHolderName
         << ", Balance: $" << fixed << setprecision(2) << balance
         << ", Overdraft Limit: $" << fixed << setprecision(2) << overdraftLimit << endl;
}

// --- Friend Function & Global Function Implementations ---

ostream& operator<<(ostream& os, const Account& acc) {
    acc.display();
    return os;
}

istream& operator>>(istream& is, Account& acc) {
    cout << "Enter Account Number: ";
    is >> acc.accountNumber;
    cout << "Enter Account Holder Name: ";
    is.ignore(); // consume newline
    getline(is, acc.accountHolderName);
    cout << "Enter Initial Balance: ";
    is >> acc.balance;
    return is;
}

void transfer(Account& from, Account& to, double amount) {
    if (from.getBalance() >= amount) {
        from.withdraw(amount);
        to.deposit(amount);
        cout << "Transferred $" << amount << " from Account #" << from.getAccountNumber()
                  << " to Account #" << to.getAccountNumber() << endl;
    } else {
        cout << "Transfer failed. Insufficient funds in Account #" << from.getAccountNumber() << endl;
    }
}

void showMenu() {
    cout << "\n--- Bank Account Simulator Menu ---" << endl;
    cout << "1. Display All Account Details" << endl;
    cout << "2. Deposit to Savings Account" << endl;
    cout << "3. Deposit with Bonus to Savings Account" << endl;
    cout << "4. Withdraw from Savings Account" << endl;
    cout << "5. Deposit to Checking Account" << endl;
    cout << "6. Withdraw from Checking Account" << endl;
    cout << "7. Transfer from Savings to Checking" << endl;
    cout << "8. Transfer from Checking to Savings" << endl;
    cout << "9. Apply Interest to Savings Account" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    cout << fixed << setprecision(2);

    string s_num, s_name, c_num, c_name;
    double s_bal, s_rate, c_bal, c_limit;

    cout << "--- Create Savings Account ---" << endl;
    cout << "Enter Account Number: ";
    cin >> s_num;
    cout << "Enter Account Holder Name: ";
    cin.ignore();
    getline(cin, s_name);
    cout << "Enter Initial Balance: ";
    cin >> s_bal;
    cout << "Enter Interest Rate (e.g., 0.04 for 4%): ";
    cin >> s_rate;
    SavingsAccount savings(s_num, s_name, s_bal, s_rate);
    cout << "Savings Account created successfully!" << endl;

    cout << "\n--- Create Checking Account ---" << endl;
    cout << "Enter Account Number: ";
    cin >> c_num;
    cout << "Enter Account Holder Name: ";
    cin.ignore();
    getline(cin, c_name);
    cout << "Enter Initial Balance: ";
    cin >> c_bal;
    cout << "Enter Overdraft Limit: ";
    cin >> c_limit;
    CheckingAccount checking(c_num, c_name, c_bal, c_limit);
    cout << "Checking Account created successfully!" << endl;


    int choice;
    double amount, bonus;

    do {
        showMenu();
        cin >> choice;
        cout << endl;

        switch (choice) {
            case 1:
                cout << savings << endl;
                cout << checking << endl;
                break;
            case 2:
                cout << "Enter amount to deposit to savings: ";
                cin >> amount;
                savings.deposit(amount);
                break;
            case 3:
                cout << "Enter amount to deposit to savings: ";
                cin >> amount;
                cout << "Enter bonus amount: ";
                cin >> bonus;
                savings.deposit(amount, bonus);
                break;
            case 4:
                cout << "Enter amount to withdraw from savings: ";
                cin >> amount;
                savings.withdraw(amount);
                break;
            case 5:
                cout << "Enter amount to deposit to checking: ";
                cin >> amount;
                checking.deposit(amount);
                break;
            case 6:
                cout << "Enter amount to withdraw from checking: ";
                cin >> amount;
                checking.withdraw(amount);
                break;
            case 7:
                cout << "Enter amount to transfer from savings to checking: ";
                cin >> amount;
                transfer(savings, checking, amount);
                break;
            case 8:
                cout << "Enter amount to transfer from checking to savings: ";
                cin >> amount;
                transfer(checking, savings, amount);
                break;
            case 9:
                savings.applyInterest();
                break;
            case 0:
                cout << "Thank you for using the Bank Account Simulator!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 0);

    return 0;
}

