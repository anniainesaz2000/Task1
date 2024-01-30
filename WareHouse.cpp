#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "../include/WareHouse.h"

// Split a string into a vector of substrings based on a delimiter
std::vector<std::string> WareHouse::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream ss(s);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

void WareHouse::parseCustomer(const std::vector<std::string>& tokens) {
        std::string name = tokens[1];
        std::string customerType = tokens[2];
        int distance = std::stoi(tokens[3]);
        int maxOrders = std::stoi(tokens[4]);

        if(customerType =="soldier"){
            customers.push_back(new SoldierCustomer(customerCounter, name, distance, maxOrders));
        }

        else if (customerType =="civilian"){
            customers.push_back(new CivilianCustomer(customerCounter, name, distance, maxOrders));
        }

        customerCounter = customerCounter +1;
        //customers.emplace_back(name, customerType, distance, maxOrders);
    }

void WareHouse::parseVolunteer(const std::vector<std::string>& tokens) {
        std::string name = tokens[1];
        std::string role = tokens[2];
        int maxDistance, distancePerStep, maxOrders, coolDown;

        if (role == "driver") {
            maxDistance = std::stoi(tokens[3]);
            distancePerStep = std::stoi(tokens[4]);
            volunteers.push_back(new DriverVolunteer(volunteerCounter,name, maxDistance, distancePerStep));
        } else if(role == "limited_driver"){
            maxDistance = std::stoi(tokens[3]);
            distancePerStep = std::stoi(tokens[4]);
            maxOrders = std::stoi(tokens[5]);
            volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter,name, maxDistance, distancePerStep, maxOrders));
         }else if(role == "collector"){
            coolDown = std::stoi(tokens[3]);
            volunteers.push_back(new CollectorVolunteer(volunteerCounter,name, coolDown));
         }
         else if(role == "limited_collector"){
            coolDown = std::stoi(tokens[3]);
            maxOrders = std::stoi(tokens[4]);
            volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter,name, coolDown, maxOrders));

         }

         volunteerCounter = volunteerCounter +1;

        //volunteers.emplace_back(name, role, coolDown, maxDistance, distancePerStep, maxOrders);
    }

WareHouse::WareHouse(const string &configFilePath):isOpen(false),actionsLog(vector<BaseAction*>()), volunteers(vector<Volunteer*>()),pendingOrders(vector<Order*>()),inProcessOrders(vector<Order*>()),completedOrders(vector<Order*>()), customers(vector<Customer*>()) ,customerCounter(0), volunteerCounter(0), orderCounter(0){
    start();
     std::ifstream file(configFilePath);
     std::string line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                // Skip empty lines and comments
                continue;
            }

            std::vector<std::string> tokens = split(line, ' ');

            if (tokens[0] == "customer") {
                parseCustomer(tokens);
            } else if (tokens[0] == "volunteer") {
                parseVolunteer(tokens);
            } else {
                std::cerr << "Unknown type: " << tokens[0] << std::endl;
            }
        }

}

//rule of 5:
// copy constructor
WareHouse::WareHouse(const WareHouse &other):isOpen(other.isOpen),actionsLog(other.actionsLog),volunteers(other.volunteers),pendingOrders(other.pendingOrders),inProcessOrders(other.inProcessOrders),completedOrders(other.completedOrders),customers(other.customers),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter){
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }

    for (Volunteer* vol : other.volunteers) {
        volunteers.push_back(vol->clone());
    }

    for (Order* penOrder : other.pendingOrders) {
        pendingOrders.push_back(penOrder->clone());
    }

    for (Order* inProOrder : other.inProcessOrders) {
        inProcessOrders.push_back(inProOrder->clone());
    }

    for (Order* compOrder : other.completedOrders) {
        completedOrders.push_back(compOrder->clone());
    }

    for (Customer* cust : other.customers) {
        customers.push_back(cust->clone());
    }


}

// move copy constructor //is the parameter sould be const?
WareHouse::WareHouse(WareHouse &&other):isOpen(other.isOpen),actionsLog(other.actionsLog),volunteers(other.volunteers),pendingOrders(other.pendingOrders),inProcessOrders(other.inProcessOrders),completedOrders(other.completedOrders),customers(other.customers),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter){
        
    for (BaseAction* action : other.actionsLog){
        actionsLog.push_back(action);
        action = nullptr;

    }

    for (Volunteer* vol : other.volunteers){
        volunteers.push_back(vol);
        vol = nullptr;

    }

    for (Order* penOrder : other.pendingOrders){
        pendingOrders.push_back(penOrder);
        penOrder = nullptr;

    }

    for (Order* inProOrder : other.inProcessOrders){
        inProcessOrders.push_back(inProOrder);
        inProOrder = nullptr;

    }

    for (Order* compOrder : other.completedOrders){
        completedOrders.push_back(compOrder);
        compOrder= nullptr;

    }

     for (Customer* cust : other.customers){
        customers.push_back(cust);

    }
    }

 //Destructor

WareHouse::~WareHouse(){

    for(BaseAction* act: actionsLog){
        delete act;
    }
    actionsLog.clear();

    for(Volunteer* vol: volunteers ){
        delete vol;
    }
    volunteers.clear();

    for(Order* penOrd: pendingOrders ){
        delete penOrd;
    }
    pendingOrders.clear();

    for(Order* inPOrd: inProcessOrders ){
        delete inPOrd;
    }
    inProcessOrders.clear();

    for(Order* comOrd: completedOrders ){
        delete comOrd;
    }
    completedOrders.clear();

    for(Customer* cust: customers ){
        delete cust;
    }
    customers.clear();

    }

//copy assignment operator

WareHouse& WareHouse::operator=(const WareHouse &other){
     if (this != &other) {
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        // Clear existing data
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        for (Volunteer* vol : volunteers) {
            delete vol;
        }
        volunteers.clear();

        for (Order* penOrder : pendingOrders) {
            delete penOrder;
        }
        pendingOrders.clear();

        for (Order* inProOrder : inProcessOrders) {
            delete inProOrder;
        }
        inProcessOrders.clear();

        for (Order* compOrder : completedOrders) {
            delete compOrder;
        }
        completedOrders.clear();

        for (Customer* cust : customers) {
            delete cust;
        }
        customers.clear();

        // Copy new data
        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }

        for (Volunteer* vol : other.volunteers) {
            volunteers.push_back(vol->clone());
        }

        for (Order* penOrder : other.pendingOrders) {
            pendingOrders.push_back(penOrder->clone());
        }

        for (Order* inProOrder : other.inProcessOrders) {
            inProcessOrders.push_back(inProOrder->clone());
        }

        for (Order* compOrder : other.completedOrders) {
            completedOrders.push_back(compOrder->clone());
        }

        for (Customer* cust : other.customers) {
            customers.push_back(cust->clone());
        }
    }

    return *this;

}

//move copy assignment operator

WareHouse& WareHouse::operator=(const WareHouse &&other){
    if (this != &other){
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;

        for (BaseAction* action : actionsLog){
            delete action;
        }

        actionsLog.clear();

        for (BaseAction* action : other.actionsLog){
            actionsLog.push_back(action);
            action = nullptr;
        }

         for (Volunteer* vol : volunteers){
            delete vol;
        }

        volunteers.clear();

        for (Volunteer* vol : other.volunteers){
            volunteers.push_back(vol);
            vol = nullptr;
        }

        for (Order* penOrder : pendingOrders){
            delete penOrder;
        }

        pendingOrders.clear();

        for (Order* penOrder : other.pendingOrders){
            pendingOrders.push_back(penOrder);
            penOrder = nullptr;
        }

         for (Order* inProOrder : inProcessOrders){
            delete inProOrder;
        }

        inProcessOrders.clear();

        for (Order* inProOrder : other.inProcessOrders){
            inProcessOrders.push_back(inProOrder);
            inProOrder = nullptr;
        }

        for (Order* compOrder : completedOrders){
            delete compOrder;
        }

        completedOrders.clear();

        for (Order* compOrder : other.completedOrders){
            completedOrders.push_back(compOrder);
            compOrder = nullptr;
        }

        for (Customer* cust : customers){
            delete cust;
        }

        customers.clear();

        for (Customer* cust : other.customers){
            customers.push_back(cust);
            cust = nullptr;
        }


    }

    return *this;

}





 void WareHouse::start(){
    open();//check if correct
    std::cout << "WareHouse is open!" << std::endl;
 }

 void WareHouse::addOrder(Order* order){
    pendingOrders.push_back(order);
    orderCounter = orderCounter +1;
 }

 void WareHouse::addAction(BaseAction* action){
    actionsLog.push_back(action);
 }

 void WareHouse::addCustomer(Customer* customer){
    customers.push_back(customer);
    customerCounter = customerCounter +1;


 }

 bool WareHouse::customerExist(int customerId) const{
    if(0<customerId  && customerId<customerCounter){
        return true;
    }

    return false;
 }

Customer& WareHouse::getCustomer(int customerId) const{
        for (Customer* cust : customers){
            if ((*cust).getId() == customerId){
                return *cust;
            }
        }

        return *customers.at(0);
}

//  bool WareHouse::volunteerExist(int volunteerId) const{
//     for (Volunteer* vol : volunteers){
//             if ((*vol).getId() == volunteerId){
//                 return true;
//             }
//         }

//     return false;
//  }


Volunteer& WareHouse::getVolunteer(int volunteerId) const{
     for (Volunteer* vol : volunteers){
            if ((*vol).getId() == volunteerId){
                return *vol;
            }
        }

     return *volunteers.at(0);
    
    }

bool WareHouse::orderExist(int orderId) const{
         for (Order* penOrder : pendingOrders){
            if ((*penOrder).getId() == orderId){
                return true;
            }
        }

         for (Order* inProOrder : inProcessOrders){
            if ((*inProOrder).getId() == orderId){
                return true;
            }
        }

         for (Order* compOrder : completedOrders){
            if ((*compOrder).getId() == orderId){
                return true;
            }
        }

        return false;
}

 Order& WareHouse::getOrder(int orderId) const{
        for (Order* penOrder : pendingOrders){
            if ((*penOrder).getId() == orderId){
                return *penOrder;
            }
        }

         for (Order* inProOrder : inProcessOrders){
            if ((*inProOrder).getId() == orderId){
                return *inProOrder;
            }
        }

         for (Order* compOrder : completedOrders){
            if ((*compOrder).getId() == orderId){
                return *compOrder;
            }
        }

        return *pendingOrders.at(0);
    }

const vector<BaseAction*>& WareHouse::getActions() const{
    return actionsLog;
    }

vector<Order*>& WareHouse::getPendingOrders(){
    return pendingOrders;
}

vector<Order*> & WareHouse::getInProcessOrders(){
     return inProcessOrders;
}

vector<Order*> & WareHouse::getCompletedOrders(){
    return completedOrders;
}


vector<Volunteer*> & WareHouse::getVolunteers(){
    return volunteers;
}

int WareHouse::getCustomerCounter(){
    return customerCounter;
}


void WareHouse::close(){
    isOpen = false;

}

void WareHouse::open(){
    isOpen=true;
}



 


