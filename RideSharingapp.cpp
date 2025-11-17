// ridesharing.cpp
// Compile: g++ -std=c++17 ridesharing.cpp -o ridesharing
// Run: ./ridesharing

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::cout;
using std::endl;
using std::fixed;
using std::setprecision;

// ---------- Base Ride (encapsulation + inheritance) ----------
class Ride {
protected:
    int rideID;
    string pickupLocation;
    string dropoffLocation;
    double distanceMiles; // distance in miles

public:
    Ride(int id, const string& pickup, const string& dropoff, double distance)
        : rideID(id), pickupLocation(pickup), dropoffLocation(dropoff), distanceMiles(distance) {}

    virtual ~Ride() = default;

    // Encapsulated getters
    int getRideID() const { return rideID; }
    string getPickup() const { return pickupLocation; }
    string getDropoff() const { return dropoffLocation; }
    double getDistance() const { return distanceMiles; }

    // Polymorphic fare calculation (overridden in subclasses)
    virtual double fare() const {
        // base default (could be abstract, but providing a default)
        const double baseRate = 1.5; // $1.50 per mile default
        return baseRate * distanceMiles;
    }

    // Polymorphic ride details
    virtual void rideDetails() const {
        cout << "Ride ID: " << rideID
             << " | Pickup: " << pickupLocation
             << " | Dropoff: " << dropoffLocation
             << " | Distance: " << distanceMiles << " miles"
             << " | Fare: $" << fixed << setprecision(2) << fare()
             << endl;
    }
};

// ---------- Derived Rides (inheritance + polymorphism) ----------
class StandardRide : public Ride {
public:
    StandardRide(int id, const string& pickup, const string& dropoff, double distance)
        : Ride(id, pickup, dropoff, distance) {}

    double fare() const override {
        const double baseFee = 2.0;     // flat base
        const double perMile = 1.8;     // $1.80 per mile
        return baseFee + perMile * distanceMiles;
    }

    void rideDetails() const override {
        cout << "[Standard] ";
        Ride::rideDetails();
    }
};

class PremiumRide : public Ride {
public:
    PremiumRide(int id, const string& pickup, const string& dropoff, double distance)
        : Ride(id, pickup, dropoff, distance) {}

    double fare() const override {
        const double baseFee = 5.0;     // higher base
        const double perMile = 3.5;     // $3.50 per mile
        const double premiumMultiplier = 1.15; // extra surge / premium
        return (baseFee + perMile * distanceMiles) * premiumMultiplier;
    }

    void rideDetails() const override {
        cout << "[Premium]  ";
        Ride::rideDetails();
    }
};

// ---------- Driver Class (encapsulation) ----------
class Driver {
private:
    int driverID;
    string name;
    double rating;
    vector<shared_ptr<Ride>> assignedRides; // private: encapsulated

public:
    Driver(int id, const string& name_, double rating_)
        : driverID(id), name(name_), rating(rating_) {}

    void addRide(const shared_ptr<Ride>& ride) {
        assignedRides.push_back(ride);
    }

    // Access assignedRides only through methods (encapsulation)
    void getDriverInfo() const {
        cout << "Driver ID: " << driverID << " | Name: " << name
             << " | Rating: " << fixed << setprecision(2) << rating << endl;
        cout << "Assigned rides: " << assignedRides.size() << endl;
        for (const auto& r : assignedRides) {
            r->rideDetails(); // polymorphic call
        }
    }

    double averageEarnings() const {
        double total = 0.0;
        for (const auto& r : assignedRides) total += r->fare();
        return assignedRides.empty() ? 0.0 : total / assignedRides.size();
    }
};

// ---------- Rider Class (encapsulation) ----------
class Rider {
private:
    int riderID;
    string name;
    vector<shared_ptr<Ride>> requestedRides; // private list

public:
    Rider(int id, const string& name_) : riderID(id), name(name_) {}

    void requestRide(const shared_ptr<Ride>& ride) {
        requestedRides.push_back(ride);
    }

    void viewRides() const {
        cout << "Rider ID: " << riderID << " | Name: " << name << endl;
        cout << "Requested rides: " << requestedRides.size() << endl;
        for (const auto& r : requestedRides) r->rideDetails(); // polymorphic
    }
};

// ---------- Demonstration (system functionality) ----------
int main() {
    // Create some rides of different types
    vector<shared_ptr<Ride>> allRides;
    allRides.push_back(make_shared<StandardRide>(101, "Downtown", "Airport", 12.3));
    allRides.push_back(make_shared<PremiumRide>(102, "Home", "Office", 5.5));
    allRides.push_back(make_shared<StandardRide>(103, "Mall", "Train Station", 3.2));
    allRides.push_back(make_shared<PremiumRide>(104, "Hotel", "Beach", 8.75));

    cout << "=== All rides (polymorphic fare calculation) ===" << endl;
    for (const auto& r : allRides) {
        r->rideDetails(); // calls overridden fare() and rideDetails()
    }

    // Create a driver and assign rides
    Driver driver1(1, "Asha Kumar", 4.92);
    driver1.addRide(allRides[0]);
    driver1.addRide(allRides[2]);

    cout << "\n=== Driver info and rides (encapsulated assignedRides) ===" << endl;
    driver1.getDriverInfo();
    cout << "Average earnings per ride: $" << fixed << setprecision(2) << driver1.averageEarnings() << endl;

    // Create a rider and request rides
    Rider rider1(5001, "Jordan Reddy");
    rider1.requestRide(allRides[1]);
    rider1.requestRide(allRides[3]);

    cout << "\n=== Rider ride history ===" << endl;
    rider1.viewRides();

    return 0;
}
