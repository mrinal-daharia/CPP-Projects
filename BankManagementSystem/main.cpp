#include <iostream>
#include <fstream>
#include <map>
#include <iomanip>
#include <sstream>
using namespace std;

class Account {
public:
    int accNumber;
    string name;
    double balance;

    Account() : accNumber(0), name(""), balance(0.0) {}

    void display() const {
        cout << "\nAccount Number: " << accNumber
             << "\nName: " << name
             << "\nCurrent Balance: rupees" << fixed << setprecision(2) << balance << "\n";
    }

    string toString() const {
        ostringstream oss;
        oss << accNumber << "," << name << "," << balance << "\n";
        return oss.str();
    }

    static Account fromString(const string& line) {
        istringstream iss(line);
        string token;
        Account acc;
        getline(iss, token, ','); acc.accNumber = stoi(token);
        getline(iss, token, ','); acc.name = token;
        getline(iss, token, ','); acc.balance = stod(token);
        return acc;
    }
};

class BankSystem {
    map<int, Account> accounts;
    const string filename = "accounts.txt";

    void loadAccounts() {
        accounts.clear();
        ifstream fin(filename);
        string line;
        while (getline(fin, line)) {
            if (!line.empty()) {
                Account acc = Account::fromString(line);
                accounts[acc.accNumber] = acc;
            }
        }
    }

    void saveAccounts() {
        ofstream fout(filename);
        for (const auto& pair : accounts) {
            fout << pair.second.toString();
        }
    }

public:
    BankSystem() { loadAccounts(); }
    ~BankSystem() { saveAccounts(); }

    void createAccount() {
        Account acc;
        cout << "Enter Account Number: ";
        cin >> acc.accNumber;

        if (accounts.count(acc.accNumber)) {
            cout << "Account already exists.\n";
            return;
        }

        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, acc.name);

        cout << "Enter Initial Deposit Amount (₹): ";
        cin >> acc.balance;

        if (acc.balance < 0) {
            cout << "Initial deposit must be positive.\n";
            return;
        }

        accounts[acc.accNumber] = acc;
        cout << "Account created successfully.\n";
    }

    void deposit() {
        int num;
        double amt;
        cout << "Enter Account Number: ";
        cin >> num;

        if (!accounts.count(num)) {
            cout << "Account not found.\n";
            return;
        }

        cout << "Enter amount to deposit (rupees): ";
        cin >> amt;

        if (amt <= 0) {
            cout << "Deposit must be a positive amount.\n";
            return;
        }

        accounts[num].balance += amt;
        cout << "Deposit successful.\n";
        cout << "New Balance: rupees" << fixed << setprecision(2) << accounts[num].balance << "\n";
    }

    void withdraw() {
        int num;
        double amt;
        cout << "Enter Account Number: ";
        cin >> num;

        if (!accounts.count(num)) {
            cout << "Account not found.\n";
            return;
        }

        cout << "Enter amount to withdraw (rupees): ";
        cin >> amt;

        if (amt <= 0) {
            cout << "Withdrawal must be a positive amount.\n";
            return;
        }

        if (amt > accounts[num].balance) {
            cout << "Insufficient balance. Withdrawal denied.\n";
            return;
        }

        accounts[num].balance -= amt;
        cout << "Withdrawal successful.\n";
        cout << "New Balance: rupees" << fixed << setprecision(2) << accounts[num].balance << "\n";
    }

    void showAccount() {
        int num;
        cout << "Enter Account Number: ";
        cin >> num;

        if (!accounts.count(num)) {
            cout << "Account not found.\n";
            return;
        }

        accounts[num].display();
    }

    void menu() {
        int choice;
        do {
            cout << "\nBANK MANAGEMENT SYSTEM\n"
                 << "----------------------\n"
                 << "1. Create Account\n"
                 << "2. Deposit\n"
                 << "3. Withdraw\n"
                 << "4. Show Account Details\n"
                 << "5. Exit\n"
                 << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: createAccount(); break;
                case 2: deposit(); break;
                case 3: withdraw(); break;
                case 4: showAccount(); break;
                case 5: cout << "Exiting program.\n"; break;
                default: cout << "Invalid choice. Please try again.\n";
            }

        } while (choice != 5);
    }
};

int main() {
    BankSystem system;
    system.menu();
    return 0;
}
