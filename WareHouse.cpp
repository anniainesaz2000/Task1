#include <iostream>
#include "../include/WareHouse.h"


//rule of 5!!!!!!!!!!!!!!!!

 WareHouse::WareHouse(const string &configFilePath){
    //parse first
 }

 // copy constructor
WareHouse::WareHouse(const WareHouse &other):isOpen(other.isOpen),actionsLog(other.actionsLog),volunteers(other.volunteers),pendingOrders(other.pendingOrders),inProcessOrders(other.inProcessOrders),completedOrders(other.completedOrders),customers(other.customers),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter){
    for (BaseAction* action : other.actionsLog){
        actionsLog.push_back(&(*action.clone()));

    }

    for (Volunteer* vol : other.volunteers){
        actionsLog.push_back(&(*vol.clone()));

    }

    for (Order* penOrder : other.pendingOrders){
        actionsLog.push_back(&(*penOrder.clone()));

    }

    for (Order* inProOrder : other.inProcessOrders){
        actionsLog.push_back(&(*inProOrder).clone());

    }

    for (Order* compOrder : other.completedOrders){
        actionsLog.push_back(&(*compOrder).clone());

    }

     for (Customer* cust : other.customers){
        actionsLog.push_back(&(*cust).clone());

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

WareHouse::~Warehouse(){

    delete isOpen;

    for (int i = 0; i < int(actionsLog.size()); i++){
        delete actionsLog.at(i);
    }

    delete actionsLog;

    for (int i = 0; i < int(volunteers.size()); i++){
        delete volunteers.at(i);
    }

    delete volunteers;
        
    for (int i=0; i < int(pendingOrders.size()); i++){
        delete pendingOrders.at(i);
    }

    delete pendingOrders;

    for (int i=0; i < int(inProcessOrders.size()); i++){
        delete inProcessOrders.at(i);
    }

    delete inProcessOrders;

    for (int i=0; i < int(completedOrders.size()); i++){
        delete completedOrders.at(i);
    }

    delete completedOrders;

    for (int i=0; i < int(customers.size()); i++){
        delete customers.at(i);
    }

    delete customers;
    
    delete customerCounter;
    delete volunteerCounter;
    delete countIdOrder;
    }

//copy assignment operator

WareHouse& WareHouse::operator=(const WareHouse &other){
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
            actionsLog.push_back(&(*action.clone()));
        }

        for (Volunteer* vol : volunteers){
            delete vol;
        }

        volunteers.clear();

        for (Volunteer* vol : other.volunteers){
            actionsLog.push_back(&(*vol.clone()));
        }

        for (Order* penOrder : pendingOrders){
            delete penOrder;
        }

        pendingOrders.clear();

        for (Order* penOrder : other.pendingOrders){
            actionsLog.push_back(&(*penOrder.clone()));
        }

        for (Order* inProOrder : inProcessOrders){
            delete inProOrder;
        }

        inProcessOrders.clear();

        for (Order* inProOrder : other.inProcessOrders){
            actionsLog.push_back(&(*inProOrder.clone()));
        }

        for (Order* compOrder : completedOrders){
            delete compOrder;
        }

        completedOrders.clear();

        for (Order* compOrder : other.completedOrders){
            actionsLog.push_back(&(*compOrder.clone()));
        }

        for (Customer* cust : customers){
            delete cust;
        }

        customers.clear();

        for (Customer* cust : other.customers){
            actionsLog.push_back(*cust.clone());
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
 }

 void WareHouse::addAction(BaseAction* action){
    actionsLog.push_back(action);
 }

Customer& WareHouse::getCustomer(int customerId) const{
        for (Customer* cust : customers){
            if ((*cust).getId() == customerId){
                return *cust;
            }
        }

        return nullptr;
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const{
     for (Volunteer* vol : volunteers){
            if ((*vol).getId() == volunteerId){
                return *vol;
            }
        }

     return nullptr;
    
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

        return nullptr;
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

void WareHouse::increaseOrderCounter(){
    orderCounter = orderCounter + 1;
}

int WareHouse::getCustomerCounter(){
    return customerCounter;
}


void WareHouse::close(){
 //use action close - elaborate in assignment 1

    isOpen = false;
}

void WareHouse::open(){
    isOpen=true;
}



 


