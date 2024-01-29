#include <iostream>
#include "../include/WareHouse.h"


WareHouse::WareHouse(const string &configFilePath){
    //parse first
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

Customer& WareHouse::getCustomer(int customerId) const{
        for (Customer* cust : customers){
            if ((*cust).getId() == customerId){
                return *cust;
            }
        }
        SoldierCustomer customer (-1, "", -1, -1);
        return customer;
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const{
     for (Volunteer* vol : volunteers){
            if ((*vol).getId() == volunteerId){
                return *vol;
            }
        }
    
    CollectorVolunteer vol (-1, "", -1);
     return vol;
    
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

        Order fictOrder(-1,-1,-1);
        return fictOrder;
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
    delete this;//or copy the destructor?

}

void WareHouse::open(){
    isOpen=true;
}



 


