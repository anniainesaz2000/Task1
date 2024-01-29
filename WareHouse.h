#pragma once
#include <string>
#include <vector>
using namespace std;

#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        WareHouse(const WareHouse &other);//copy constructor
        WareHouse(WareHouse &&other);//move copt constructor//should be const? 
        ~WareHouse();//destructor
        WareHouse &operator=(const WareHouse &other);//copy assignment operator
        WareHouse &operator=(const WareHouse &&other);//move copy assignment operator
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addCustomer(Customer* customer);//we added it
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        vector<Order*> &getPendingOrders();//we added it
        vector<Order*> &getInProcessOrders();//we added it
        vector<Order*> &getCompletedOrders();//we added it
        vector<Volunteer*> &getVolunteers();//we added it
        int getCustomerCounter();//we added it

        void close();
        void open();

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter;//we added it
};
