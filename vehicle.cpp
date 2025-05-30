#include <iostream> // Include input/output stream library
#include <string> // Include string handling library
#include <vector> // Include vector container library
using namespace std; // Use standard namespace

// Base Person class - parent for Admin, Customer, RentalAgent
class Person {
protected:
    string personId; // Unique identifier for person
    string name; // Person's full name
    string email; // Person's email address
    string phone; // Person's phone number
    
public:
    Person(string id, string n, string e, string p) : personId(id), name(n), email(e), phone(p) {} // Constructor with initialization list
    virtual ~Person() = default; // Virtual destructor for proper inheritance
    string getName() const { return name; } // Get person's name
    virtual void display() { cout << "Name: " << name << ", Email: " << email << endl; } // Display person info
};

// Car class - represents rental vehicles
class Car {
private:
    string carId; // Unique car identifier
    string make; // Car manufacturer (Toyota, Honda, etc.)
    string model; // Car model (Camry, Civic, etc.)
    bool available; // Car availability status
    
public:
    Car(string id, string mk, string md) : carId(id), make(mk), model(md), available(true) {} // Constructor sets car as available
    string getCarId() const { return carId; } // Get car ID
    bool isAvailable() const { return available; } // Check if car is available
    void setAvailable(bool status) { available = status; } // Set car availability
    void display() { cout << "Car: " << make << " " << model << " [" << (available ? "Available" : "Rented") << "]" << endl; } // Display car info
};

// Admin class - inherits from Person, manages system
class Admin : public Person {
private:
    string adminLevel; // Admin privilege level
    
public:
    Admin(string id, string name, string email, string phone, string level) : Person(id, name, email, phone), adminLevel(level) {} // Initialize admin with level
    void manageSystem() { cout << "Admin " << name << " managing system" << endl; } // System management function
    void display() override { cout << "Admin - "; Person::display(); } // Override display for admin info
};

// Customer class - inherits from Person, can rent cars
class Customer : public Person {
private:
    string licenseNumber; // Driver's license number
    vector<string> rentedCars; // List of currently rented car IDs
    
public:
    Customer(string id, string name, string email, string phone, string license) : Person(id, name, email, phone), licenseNumber(license) {} // Initialize customer
    bool rentCar(Car& car) { // Rent a car function
        if (car.isAvailable()) { // Check if car is available
            car.setAvailable(false); // Mark car as rented
            rentedCars.push_back(car.getCarId()); // Add to rented cars list
            cout << name << " rented car " << car.getCarId() << endl; // Confirm rental
            return true; // Return success
        }
        return false; // Return failure if car not available
    }
    void display() override { cout << "Customer - "; Person::display(); cout << "Rented cars: " << rentedCars.size() << endl; } // Display customer info
};

// RentalAgent class - inherits from Person, processes rentals
class RentalAgent : public Person {
private:
    string employeeId; // Employee identification number
    
public:
    RentalAgent(string id, string name, string email, string phone, string empId) : Person(id, name, email, phone), employeeId(empId) {} // Initialize agent
    void processRental(Customer& customer, Car& car) { // Process rental transaction
        cout << "Agent " << name << " processing rental..." << endl; // Announce processing
        customer.rentCar(car); // Execute the rental
    }
    void display() override { cout << "Agent - "; Person::display(); } // Display agent info
};

// CarOwner class - independent class, owns cars
class CarOwner {
private:
    string ownerId; // Owner's unique ID
    string name; // Owner's name
    vector<Car> cars; // List of owned cars
    
public:
    CarOwner(string id, string n) : ownerId(id), name(n) {} // Initialize car owner
    void addCar(string carId, string make, string model) { // Add new car to fleet
        cars.emplace_back(carId, make, model); // Create and add car to vector
        cout << "Owner " << name << " added " << make << " " << model << endl; // Confirm addition
    }
    Car* findCar(string carId) { // Find car by ID
        for (auto& car : cars) { // Loop through owned cars
            if (car.getCarId() == carId) return &car; // Return car if found
        }
        return nullptr; // Return null if not found
    }
    void displayCars() { // Display all owned cars
        cout << "Owner " << name << " cars:" << endl; // Print header
        for (auto& car : cars) car.display(); // Display each car
    }
};

// Main function - demonstrates the system
int main() {
    cout << "=== Car Rental System ===" << endl; // Print system title
    
    CarOwner owner("O1", "John Smith"); // Create car owner
    owner.addCar("C1", "Toyota", "Camry"); // Add first car
    owner.addCar("C2", "Honda", "Civic"); // Add second car
    
    Customer customer("P1", "Alice", "alice@email.com", "555-1234", "DL123"); // Create customer
    RentalAgent agent("P2", "Bob", "bob@rental.com", "555-5678", "EMP001"); // Create rental agent
    Admin admin("P3", "Carol", "carol@admin.com", "555-9999", "Super"); // Create admin
    
    cout << "\n=== Users ===" << endl; // Print users section header
    customer.display(); // Show customer info
    agent.display(); // Show agent info
    admin.display(); // Show admin info
    
    cout << "\n=== Cars ===" << endl; // Print cars section header
    owner.displayCars(); // Show all cars
    
    cout << "\n=== Rental Process ===" << endl; // Print rental section header
    Car* car = owner.findCar("C1"); // Find specific car
    if (car) agent.processRental(customer, *car); // Process rental if car exists
    
    cout << "\n=== After Rental ===" << endl; // Print after rental header
    owner.displayCars(); // Show updated car status
    customer.display(); // Show updated customer info
    
    return 0; // End program successfully
}