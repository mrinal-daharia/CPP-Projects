#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

class Student {
    int roll;
    string name;
    string branch;
    float marks;

public:
    Student() : roll(0), name(""), branch(""), marks(0.0) {}

    void input() {
        cout << "Enter Roll Number: ";
        cin >> roll;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Branch: ";
        getline(cin, branch);
        cout << "Enter Marks: ";
        cin >> marks;
    }

    void display() const {
        cout << left << setw(10) << roll
             << setw(20) << name
             << setw(15) << branch
             << setw(6) << marks << endl;
    }

    int getRoll() const { return roll; }
    void update() {
        cout << "Update details for Roll No. " << roll << ":\n";
        cin.ignore();
        cout << "New Name: ";
        getline(cin, name);
        cout << "New Branch: ";
        getline(cin, branch);
        cout << "New Marks: ";
        cin >> marks;
    }

    // Serialize/Deserialize functions
    void writeToFile(fstream& fs) const {
        fs.write(reinterpret_cast<const char*>(this), sizeof(Student));
    }

    void readFromFile(fstream& fs) {
        fs.read(reinterpret_cast<char*>(this), sizeof(Student));
    }
};

class StudentManager {
    const string filename = "students.dat";

public:
    void addStudent() {
        fstream fs(filename, ios::binary | ios::app);
        Student s;
        s.input();
        s.writeToFile(fs);
        fs.close();
        cout << "Student added successfully.\n";
    }

    void viewAll() {
        fstream fs(filename, ios::binary | ios::in);
        if (!fs) {
            cout << "No student records found.\n";
            return;
        }
        Student s;
        cout << "\nStudent Records:\n";
        cout << left << setw(10) << "Roll"
             << setw(20) << "Name"
             << setw(15) << "Branch"
             << setw(6) << "Marks\n";
        cout << string(50, '-') << endl;
        while (fs.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
            s.display();
        }
        fs.close();
    }

    void searchStudent(int rollNo) {
        fstream fs(filename, ios::binary | ios::in);
        Student s;
        bool found = false;

        while (fs.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
            if (s.getRoll() == rollNo) {
                cout << "Student Found:\n";
                s.display();
                found = true;
                break;
            }
        }
        if (!found) cout << "Student with roll number " << rollNo << " not found.\n";
        fs.close();
    }

    void deleteStudent(int rollNo) {
        fstream fs(filename, ios::binary | ios::in);
        fstream temp("temp.dat", ios::binary | ios::out);
        Student s;
        bool found = false;

        while (fs.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
            if (s.getRoll() != rollNo) {
                s.writeToFile(temp);
            } else {
                found = true;
            }
        }

        fs.close();
        temp.close();
        remove(filename.c_str());
        rename("temp.dat", filename.c_str());

        if (found)
            cout << "Student deleted successfully.\n";
        else
            cout << "Student not found.\n";
    }

    void updateStudent(int rollNo) {
        fstream fs(filename, ios::binary | ios::in | ios::out);
        Student s;
        bool found = false;

        while (fs.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
            if (s.getRoll() == rollNo) {
                s.update();
                fs.seekp(-static_cast<int>(sizeof(Student)), ios::cur);
                s.writeToFile(fs);
                found = true;
                cout << "Student updated successfully.\n";
                break;
            }
        }

        if (!found)
            cout << "Student not found.\n";
        fs.close();
    }
};

void showMenu() {
    cout << "\n=========== STUDENT DATABASE SYSTEM ===========\n";
    cout << "1. Add Student\n";
    cout << "2. View All Students\n";
    cout << "3. Search Student by Roll No\n";
    cout << "4. Update Student\n";
    cout << "5. Delete Student\n";
    cout << "0. Exit\n";
    cout << "===============================================\n";
    cout << "Enter choice: ";
}

int main() {
    StudentManager manager;
    int choice;

    do {
        showMenu();
        cin >> choice;
        switch (choice) {
            case 1:
                manager.addStudent();
                break;
            case 2:
                manager.viewAll();
                break;
            case 3: {
                int roll;
                cout << "Enter Roll No to search: ";
                cin >> roll;
                manager.searchStudent(roll);
                break;
            }
            case 4: {
                int roll;
                cout << "Enter Roll No to update: ";
                cin >> roll;
                manager.updateStudent(roll);
                break;
            }
            case 5: {
                int roll;
                cout << "Enter Roll No to delete: ";
                cin >> roll;
                manager.deleteStudent(roll);
                break;
            }
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
