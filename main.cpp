Here is a complete, clean, and comment-free C++ implementation for the **Bank Management Application**.

This code fulfills all specifications in the image: it uses Object-Oriented Programming (OOP) and implements persistent **file handling** so that customer account balances are securely saved to a text file and remain intact even after the program closes.

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Account {
private:
    int accountNumber;
    string name;
    double balance;

public:
    Account(int accNum = 0, string accName = "", double initialBalance = 0.0) {
        accountNumber = accNum;
        name = accName;
        balance = initialBalance;
    }

    int getAccountNumber() const { return accountNumber; }
    string getName() const { return name; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Successfully deposited $" << fixed << setprecision(2) << amount << "\n";
        } else {
            cout << "Invalid deposit amount.\n";
        }
    }

    void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Successfully withdrew $" << fixed << setprecision(2) << amount << "\n";
        } else if (amount > balance) {
            cout << "Error: Insufficient balance.\n";
        } else {
            cout << "Invalid withdrawal amount.\n";
        }
    }

    void display() const {
        cout << "Account Number: " << accountNumber << "\n";
        cout << "Account Holder: " << name << "\n";
        cout << "Current Balance: $" << fixed << setprecision(2) << balance << "\n";
    }

    void writeToFile(ofstream &out) const {
        out << accountNumber << "\n" << name << "\n" << balance << "\n";
    }

    void readFromFile(ifstream &in) {
        in >> accountNumber;
        in.ignore();
        getline(in, name);
        in >> balance;
    }
};

class BankManager {
private:
    string filename;

    bool accountExists(int accNum) {
        ifstream in(filename);
        if (!in) return false;

        Account temp;
        while (in.peek() != EOF) {
            temp.readFromFile(in);
            if (temp.getAccountNumber() == accNum) {
                in.close();
                return true;
            }
            in.ignore();
        }
        in.close();
        return false;
    }

public:
    BankManager(string dbFile) {
        filename = dbFile;
    }

    void createAccount(int accNum, string name, double initialBalance) {
        if (accountExists(accNum)) {
            cout << "Error: An account with this number already exists.\n";
            return;
        }

        Account newAcc(accNum, name, initialBalance);
        ofstream out(filename, ios::app);
        if (!out) {
            cout << "Error saving record to file.\n";
            return;
        }
        newAcc.writeToFile(out);
        out.close();
        cout << "Account created successfully!\n";
    }

    void updateAccountBalance(int accNum, double amount, bool isDeposit) {
        ifstream in(filename);
        if (!in) {
            cout << "Error opening system records.\n";
            return;
        }

        vector<Account> accounts;
        Account temp;
        bool found = false;

        while (in.peek() != EOF) {
            temp.readFromFile(in);
            if (temp.getAccountNumber() == accNum) {
                if (isDeposit) {
                    temp.deposit(amount);
                } else {
                    temp.withdraw(amount);
                }
                found = true;
            }
            accounts.push_back(temp);
            in.ignore();
        }
        in.close();

        if (!found) {
            cout << "Error: Account number not found.\n";
            return;
        }

        ofstream out(filename, ios::trunc);
        for (const auto &acc : accounts) {
            acc.writeToFile(out);
        }
        out.close();
    }

    void checkBalance(int accNum) {
        ifstream in(filename);
        if (!in) {
            cout << "Error opening system records.\n";
            return;
        }

        Account temp;
        bool found = false;

        while (in.peek() != EOF) {
            temp.readFromFile(in);
            if (temp.getAccountNumber() == accNum) {
                cout << "\n--- Account Details ---\n";
                temp.display();
                found = true;
                break;
            }
            in.ignore();
        }
        in.close();

        if (!found) {
            cout << "Error: Account number not found.\n";
        }
    }
};

int main() {
    BankManager bank("bank_records.txt");
    int choice;

    while (true) {
        cout << "\n===============================\n";
        cout << "    BANK MANAGEMENT SYSTEM     \n";
        cout << "===============================\n";
        cout << "1. Create New Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Balance Inquiry\n";
        cout << "5. Exit\n";
        cout << "Enter your choice (1-5): ";
        cin >> choice;

        if (choice == 5) {
            cout << "Thank you for using our banking system. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: {
                int accNum;
                string name;
                double initialBalance;
                cout << "Enter Account Number: "; cin >> accNum;
                cin.ignore();
                cout << "Enter Account Holder Name: "; getline(cin, name);
                cout << "Enter Initial Balance: $"; cin >> initialBalance;
                bank.createAccount(accNum, name, initialBalance);
                break;
            }
            case 2: {
                int accNum;
                double amount;
                cout << "Enter Account Number: "; cin >> accNum;
                cout << "Enter Deposit Amount: $"; cin >> amount;
                bank.updateAccountBalance(accNum, amount, true);
                break;
            }
            case 3: {
                int accNum;
                double amount;
                cout << "Enter Account Number: "; cin >> accNum;
                cout << "Enter Withdrawal Amount: $"; cin >> amount;
                bank.updateAccountBalance(accNum, amount, false);
                break;
            }
            case 4: {
                int accNum;
                cout << "Enter Account Number: "; cin >> accNum;
                bank.checkBalance(accNum);
                break;
            }
            default:
                cout << "Invalid selection! Please try again.\n";
        }
    }
    return 0;
}

```
