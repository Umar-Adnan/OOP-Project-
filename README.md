# Bank Account Simulator (C++)

A console-based bank account simulator demonstrating OOP concepts in C++ using an abstract base class (`Account`) and two derived classes (`SavingsAccount`, `CheckingAccount`). The program lets you create one savings and one checking account, then interact with them through a menu (deposit, withdraw, transfer, apply interest, display details).

## Features

- **Two account types**
  - **Savings**: monthly withdrawal limit (max 3), interest rate, interest application resets the monthly withdrawal counter
  - **Checking**: supports withdrawals with an overdraft limit
- **Operations**
  - Deposit / withdraw for both accounts
  - Savings deposit with optional bonus
  - Transfer between accounts (savings ↔ checking)
  - Display account details
- **OOP concepts used**
  - Abstract class + pure virtual functions (`deposit`, `withdraw`, `display`)
  - Inheritance and polymorphism (`SavingsAccount`, `CheckingAccount`)
  - Function overloading (`SavingsAccount::deposit(amount, bonus)`)
  - Operator overloading (`+`, `-`, comparison operators, stream insertion/extraction)
  - Friend function for transfer (`transfer(from, to, amount)`)

## Project Files

- `Bank Account Simulator Project.cpp`: main C++ source file (contains all classes and `main()`)

## Build & Run (Windows)

### Option A: g++ (MinGW-w64)

From the project folder:

```bash
g++ -std=c++17 -O2 -Wall -Wextra "Bank Account Simulator Project.cpp" -o bank-sim.exe
.\bank-sim.exe
```

### Option B: MSVC (Visual Studio Developer Command Prompt)

From the project folder:

```bat
cl /EHsc /std:c++17 "Bank Account Simulator Project.cpp"
Bank\ Account\ Simulator\ Project.exe
```

## How to Use

1. The program first asks you to **create a Savings Account** (number, holder name, initial balance, interest rate).
2. Then it asks you to **create a Checking Account** (number, holder name, initial balance, overdraft limit).
3. Use the menu options:

- **1** Display all account details
- **2** Deposit to savings
- **3** Deposit to savings with bonus
- **4** Withdraw from savings (max 3 withdrawals until interest is applied)
- **5** Deposit to checking
- **6** Withdraw from checking (can go negative up to overdraft limit)
- **7** Transfer from savings to checking
- **8** Transfer from checking to savings
- **9** Apply interest to savings (also resets monthly withdrawal count)
- **0** Exit

## Notes / Constraints (based on current code)

- The simulator creates **exactly one savings account and one checking account** per run.
- Savings withdrawals are limited to **3 per “month”**; applying interest resets this counter.
- Transfers only proceed if the **source account’s balance is at least the transfer amount** (the transfer check is based on current balance).

