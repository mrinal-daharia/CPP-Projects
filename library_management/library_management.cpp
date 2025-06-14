#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
using namespace std;

class Book {
public:
    int bookID;
    string title, author;
    bool isIssued;
    int issuedTo;

    Book() : bookID(0), isIssued(false), issuedTo(-1) {}

    void input() {
        cout << "Enter Book ID: ";
        cin >> bookID;
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
    }

    void display() const {
        cout << left << setw(8) << bookID
             << setw(25) << title
             << setw(20) << author
             << setw(10) << (isIssued ? "Issued" : "Available") << endl;
    }
};

class Member {
public:
    int memberID;
    string name;
    int borrowedBookID;
    time_t issueDate;

    Member() : memberID(0), borrowedBookID(-1), issueDate(0) {}

    void input() {
        cout << "Enter Member ID: ";
        cin >> memberID;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
    }

    void display() const {
        cout << left << setw(10) << memberID
             << setw(25) << name
             << (borrowedBookID == -1 ? "No Book Issued" : ("Issued Book ID: " + to_string(borrowedBookID))) << endl;
    }
};

class Library {
    string bookFile = "books.dat";
    string memberFile = "members.dat";

public:
    void addBook() {
        fstream fout(bookFile, ios::binary | ios::app);
        Book b;
        b.input();
        fout.write((char*)&b, sizeof(Book));
        fout.close();
        cout << "Book added.\n";
    }

    void viewBooks() {
        fstream fin(bookFile, ios::binary | ios::in);
        Book b;
        cout << "\nBook List:\n";
        cout << left << setw(8) << "ID" << setw(25) << "Title" << setw(20) << "Author" << setw(10) << "Status\n";
        cout << string(65, '-') << endl;
        while (fin.read((char*)&b, sizeof(Book))) {
            b.display();
        }
        fin.close();
    }

    void addMember() {
        fstream fout(memberFile, ios::binary | ios::app);
        Member m;
        m.input();
        fout.write((char*)&m, sizeof(Member));
        fout.close();
        cout << "Member added.\n";
    }

    void viewMembers() {
        fstream fin(memberFile, ios::binary | ios::in);
        Member m;
        cout << "\nMember List:\n";
        cout << left << setw(10) << "ID" << setw(25) << "Name" << "Issued Book\n";
        cout << string(55, '-') << endl;
        while (fin.read((char*)&m, sizeof(Member))) {
            m.display();
        }
        fin.close();
    }

    void issueBook(int bookID, int memberID) {
        fstream bfile(bookFile, ios::in | ios::out | ios::binary);
        fstream mfile(memberFile, ios::in | ios::out | ios::binary);
        Book b;
        Member m;
        bool bookFound = false, memberFound = false;

        while (bfile.read((char*)&b, sizeof(Book))) {
            if (b.bookID == bookID && !b.isIssued) {
                bookFound = true;
                break;
            }
        }

        if (!bookFound) {
            cout << "Book not available or doesn't exist.\n";
            return;
        }

        while (mfile.read((char*)&m, sizeof(Member))) {
            if (m.memberID == memberID && m.borrowedBookID == -1) {
                memberFound = true;
                break;
            }
        }

        if (!memberFound) {
            cout << "Member not found or already has a book.\n";
            return;
        }

        b.isIssued = true;
        b.issuedTo = memberID;
        m.borrowedBookID = bookID;
        m.issueDate = time(0);

        // Write updated book
        bfile.clear();
        bfile.seekp(-sizeof(Book), ios::cur);
        bfile.write((char*)&b, sizeof(Book));

        // Write updated member
        mfile.clear();
        mfile.seekp(-sizeof(Member), ios::cur);
        mfile.write((char*)&m, sizeof(Member));

        cout << "Book issued successfully.\n";
        bfile.close();
        mfile.close();
    }

    void returnBook(int bookID, int memberID) {
        fstream bfile(bookFile, ios::in | ios::out | ios::binary);
        fstream mfile(memberFile, ios::in | ios::out | ios::binary);
        Book b;
        Member m;
        bool bookFound = false, memberFound = false;

        while (bfile.read((char*)&b, sizeof(Book))) {
            if (b.bookID == bookID && b.isIssued && b.issuedTo == memberID) {
                bookFound = true;
                break;
            }
        }

        while (mfile.read((char*)&m, sizeof(Member))) {
            if (m.memberID == memberID && m.borrowedBookID == bookID) {
                memberFound = true;
                break;
            }
        }

        if (!bookFound || !memberFound) {
            cout << "Invalid return attempt.\n";
            return;
        }

        // Calculate fine
        time_t now = time(0);
        double days = difftime(now, m.issueDate) / (60 * 60 * 24);
        if (days > 14) {
            double fine = (days - 14) * 2;
            cout << "Book returned late. Fine: ₹" << fine << endl;
        } else {
            cout << "Book returned on time.\n";
        }

        // Update records
        b.isIssued = false;
        b.issuedTo = -1;
        m.borrowedBookID = -1;
        m.issueDate = 0;

        bfile.clear();
        bfile.seekp(-sizeof(Book), ios::cur);
        bfile.write((char*)&b, sizeof(Book));

        mfile.clear();
        mfile.seekp(-sizeof(Member), ios::cur);
        mfile.write((char*)&m, sizeof(Member));

        bfile.close();
        mfile.close();
    }
};

void showLibraryMenu() {
    cout << "\n======= LIBRARY MANAGEMENT =======\n";
    cout << "1. Add Book\n";
    cout << "2. View Books\n";
    cout << "3. Add Member\n";
    cout << "4. View Members\n";
    cout << "5. Issue Book\n";
    cout << "6. Return Book\n";
    cout << "0. Exit\n";
    cout << "==================================\n";
    cout << "Enter choice: ";
}

int main() {
    Library lib;
    int choice;

    do {
        showLibraryMenu();
        cin >> choice;
        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.viewBooks(); break;
            case 3: lib.addMember(); break;
            case 4: lib.viewMembers(); break;
            case 5: {
                int bookID, memberID;
                cout << "Enter Book ID to issue: ";
                cin >> bookID;
                cout << "Enter Member ID: ";
                cin >> memberID;
                lib.issueBook(bookID, memberID);
                break;
            }
            case 6: {
                int bookID, memberID;
                cout << "Enter Book ID to return: ";
                cin >> bookID;
                cout << "Enter Member ID: ";
                cin >> memberID;
                lib.returnBook(bookID, memberID);
                break;
            }
            case 0:
                cout << "Exiting Library System...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
