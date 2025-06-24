#include <iostream>
#include <vector>
#include <string>
using namespace std;

// CLASS: Car
// Demonstrates: Encapsulation, Composition
class Car {
private:
    // Encapsulated properties (only accessible via methods)
    string model;
    string plateNumber;
    bool isAvailable;

public:
    // Constructor
    Car(string m, string plate) : model(m), plateNumber(plate), isAvailable(true) {}

    // Public methods to access/modify private data (encapsulation)
    string getModel() { return model; }
    string getPlate() { return plateNumber; }
    bool available() { return isAvailable; }

    void rent() { isAvailable = false; }
    void returnCar() { isAvailable = true; }

    // Const method: doesn't modify object state
    void display() const {
        cout << "Model: " << model
             << ", Plate: " << plateNumber
             << ", Status: " << (isAvailable ? "Available" : "Rented") << endl;
    }
};

// ABSTRACT CLASS: User
// Demonstrates: Inheritance, Abstraction, Polymorphism
class User {
protected:
    // Encapsulated attributes
    string username;
    string password;

public:
    // Base constructor
    User(string uname, string pass) : username(uname), password(pass) {}

    // Pure virtual function (abstract method)
    virtual string getRole() = 0; // Enforces polymorphism

    // Accessor methods
    string getUsername() { return username; }
    string getPassword() { return password; }

    // Virtual destructor ensures proper cleanup in derived classes
    virtual ~User() {}
};

// CLASS: Admin
// Inherits: User
// Demonstrates: Inheritance and Polymorphism
class Admin : public User {
public:
    Admin(string uname, string pass) : User(uname, pass) {}

    // Polymorphic behavior (different implementation from Customer)
    string getRole() override { return "Admin"; }

    // Admin-specific behavior: adding cars
    void addCar(vector<Car>& cars) {
        string model, plate;
        cin.ignore();
        cout << "Enter car model: ";
        getline(cin, model);
        cout << "Enter plate number: ";
        getline(cin, plate);
        cars.push_back(Car(model, plate));
        cout << "✅ Car added successfully!\n";
    }

    // Admin-specific behavior: viewing all cars
    void viewCars(const vector<Car>& cars) {
        cout << "\n--- All Cars ---\n";
        for (const auto& car : cars)
            car.display();
    }
};

// CLASS: Customer
// Inherits: User
// Demonstrates: Inheritance and Polymorphism
class Customer : public User {
public:
    Customer(string uname, string pass) : User(uname, pass) {}

    // Customer-specific role implementation
    string getRole() override { return "Customer"; }

    // View only available cars
    void viewAvailableCars(vector<Car>& cars) {
        cout << "\n--- Available Cars ---\n";
        bool found = false;
        for (auto& car : cars) {
            if (car.available()) {
                car.display();
                found = true;
            }
        }
        if (!found)
            cout << "❌ No cars available.\n";
    }

    // Rent a car
    void rentCar(vector<Car>& cars) {
        string plate;
        cout << "Enter plate number to rent: ";
        cin >> plate;
        for (auto& car : cars) {
            if (car.getPlate() == plate && car.available()) {
                car.rent();
                cout << "✅ You rented: " << car.getModel() << "\n";
                return;
            }
        }
        cout << "❌ Car not found or already rented.\n";
    }

    // Return a rented car
    void returnCar(vector<Car>& cars) {
        string plate;
        cout << "Enter plate number to return: ";
        cin >> plate;
        for (auto& car : cars) {
            if (car.getPlate() == plate && !car.available()) {
                car.returnCar();
                cout << "✅ You returned: " << car.getModel() << "\n";
                return;
            }
        }
        cout << "❌ Car not found or not rented.\n";
    }
};
// CLASS: RentalSystem
// Demonstrates: Composition, Abstraction, Encapsulation
class RentalSystem {
private:
    vector<Car> cars;         // Composition: System "has-a" list of cars
    vector<User*> users;      // Composition: System "has-a" list of users

public:
    // Constructor seeds some initial data (for demo)
    RentalSystem() {
        users.push_back(new Admin("admin", "admin123"));
        users.push_back(new Customer("john", "1234"));
        users.push_back(new Customer("alice", "pass"));

        cars.push_back(Car("Toyota Corolla", "ABC123"));
        cars.push_back(Car("Honda Civic", "XYZ789"));
    }

    // Method to authenticate users
    User* login(string uname, string pass) {
        for (auto user : users) {
            if (user->getUsername() == uname && user->getPassword() == pass)
                return user; // Polymorphism in action
        }
        return nullptr;
    }

    // Main program loop
    void run() {
        string uname, pass;
        cout << "===== 🚗 Welcome to Car Rental System =====\n";
        cout << "Username: ";
        cin >> uname;
        cout << "Password: ";
        cin >> pass;

        User* user = login(uname, pass);

        if (!user) {
            cout << "❌ Invalid credentials! Exiting...\n";
            return;
        }

        cout << "\n✅ Login successful! Welcome " << uname << " (" << user->getRole() << ")\n";

        // Dynamic dispatch via polymorphism
        if (user->getRole() == "Admin") {
            Admin* admin = dynamic_cast<Admin*>(user);
            int choice;
            do {
                cout << "\n--- 🔧 Admin Menu ---\n";
                cout << "1. Add Car\n2. View All Cars\n3. Logout\nChoice: ";
                cin >> choice;

                if (choice == 1) admin->addCar(cars);
                else if (choice == 2) admin->viewCars(cars);
                else if (choice == 3) cout << "👋 Logging out...\n";
                else cout << "❌ Invalid choice!\n";

            } while (choice != 3);
        }

        else if (user->getRole() == "Customer") {
            Customer* cust = dynamic_cast<Customer*>(user);
            int choice;
            do {
                cout << "\n--- 👤 Customer Menu ---\n";
                cout << "1. View Available Cars\n2. Rent Car\n3. Return Car\n4. Logout\nChoice: ";
                cin >> choice;

                if (choice == 1) cust->viewAvailableCars(cars);
                else if (choice == 2) cust->rentCar(cars);
                else if (choice == 3) cust->returnCar(cars);
                else if (choice == 4) cout << "👋 Logging out...\n";
                else cout << "❌ Invalid choice!\n";

            } while (choice != 4);
        }
    }

    // Destructor releases memory (good OOP practice)
    ~RentalSystem() {
        for (auto user : users)
            delete user;
    }
};

// MAIN FUNCTION
// Entry point of the program

int main() {
    RentalSystem app;
    app.run(); // Start the system
    return 0;
}


