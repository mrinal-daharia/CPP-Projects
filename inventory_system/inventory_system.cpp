#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

class Product {
public:
    int productID;
    string name;
    int quantity;
    float price;
    int reorderLevel;

    Product() : productID(0), quantity(0), price(0), reorderLevel(5) {}

    void input() {
        cout << "Enter Product ID: ";
        cin >> productID;
        cin.ignore();
        cout << "Enter Product Name: ";
        getline(cin, name);
        cout << "Enter Quantity: ";
        cin >> quantity;
        cout << "Enter Price: ";
        cin >> price;
        cout << "Enter Reorder Level: ";
        cin >> reorderLevel;
    }

    void display() const {
        cout << left << setw(10) << productID
             << setw(20) << name
             << setw(10) << quantity
             << setw(10) << price
             << setw(15) << reorderLevel
             << (quantity <= reorderLevel ? "Reorder Needed" : "") << endl;
    }
};

class Inventory {
    string file = "products.dat";

public:
    void addProduct() {
        Product p;
        fstream fout(file, ios::binary | ios::app);
        p.input();
        fout.write((char*)&p, sizeof(Product));
        fout.close();
        cout << "Product added successfully.\n";
    }

    void viewProducts() {
        fstream fin(file, ios::binary | ios::in);
        Product p;
        cout << "\nPRODUCT LIST\n";
        cout << left << setw(10) << "ID" << setw(20) << "Name"
             << setw(10) << "Qty" << setw(10) << "Price"
             << setw(15) << "Reorder Level" << "Status\n";
        cout << string(70, '-') << endl;
        while (fin.read((char*)&p, sizeof(Product))) {
            p.display();
        }
        fin.close();
    }

    void updateQuantity(int id, int change) {
        fstream fileStream(file, ios::binary | ios::in | ios::out);
        Product p;
        bool found = false;

        while (fileStream.read((char*)&p, sizeof(Product))) {
            if (p.productID == id) {
                found = true;
                p.quantity += change;
                if (p.quantity < 0) p.quantity = 0;
                fileStream.seekp(-sizeof(Product), ios::cur);
                fileStream.write((char*)&p, sizeof(Product));
                cout << "Quantity updated successfully.\n";
                break;
            }
        }

        if (!found) {
            cout << "Product not found.\n";
        }
        fileStream.close();
    }

    void deleteProduct(int id) {
        ifstream fin(file, ios::binary);
        ofstream fout("temp.dat", ios::binary);
        Product p;
        bool found = false;

        while (fin.read((char*)&p, sizeof(Product))) {
            if (p.productID == id) {
                found = true;
                continue;
            }
            fout.write((char*)&p, sizeof(Product));
        }

        fin.close();
        fout.close();
        remove(file.c_str());
        rename("temp.dat", file.c_str());

        if (found) {
            cout << "Product deleted successfully.\n";
        } else {
            cout << "Product not found.\n";
        }
    }

    void searchProduct(int id) {
        ifstream fin(file, ios::binary);
        Product p;
        bool found = false;

        while (fin.read((char*)&p, sizeof(Product))) {
            if (p.productID == id) {
                p.display();
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Product not found.\n";
        }
        fin.close();
    }
};

void showInventoryMenu() {
    cout << "\n======= INVENTORY MANAGEMENT =======\n";
    cout << "1. Add Product\n";
    cout << "2. View Products\n";
    cout << "3. Update Quantity\n";
    cout << "4. Delete Product\n";
    cout << "5. Search Product\n";
    cout << "0. Exit\n";
    cout << "====================================\n";
    cout << "Enter choice: ";
}

int main() {
    Inventory inventory;
    int choice;

    do {
        showInventoryMenu();
        cin >> choice;
        switch (choice) {
            case 1: inventory.addProduct(); break;
            case 2: inventory.viewProducts(); break;
            case 3: {
                int id, qty;
                cout << "Enter Product ID: ";
                cin >> id;
                cout << "Enter Quantity to Add/Subtract: ";
                cin >> qty;
                inventory.updateQuantity(id, qty);
                break;
            }
            case 4: {
                int id;
                cout << "Enter Product ID to Delete: ";
                cin >> id;
                inventory.deleteProduct(id);
                break;
            }
            case 5: {
                int id;
                cout << "Enter Product ID to Search: ";
                cin >> id;
                inventory.searchProduct(id);
                break;
            }
            case 0:
                cout << "Exiting Inventory System...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
