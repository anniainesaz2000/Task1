#include <iostream>
#include "../include/WareHouse.h"


//rule of 5!!!!!!!!!!!!!!!!

 WareHouse::WareHouse(const string &configFilePath){
    //parse first
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
        for (int i = 0; i < int(customers.size()); i++){//need to br checked
            if (*customers.at(i).getId() == customerId){
                return *customers.at(i);
            }
        }
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const{
     for (int i = 0; i < int(volunteers.size()); i++){//need to br checked
            if (*volunteers.at(i).getId() == volunteerId){
                return *volunteers.at(i);
            }
        }
    
    }

 Order& WareHouse::getOrder(int orderId) const{
        for (int i = 0; i < int(pendingOrders.size()); i++){//need to br checked
            if (*pendingOrders.at(i).getId() == orderId){
                return *pendingOrders.at(i);
            }
        }

         for (int i = 0; i < int(inProcessOrders.size()); i++){//need to br checked
            if (*inProcessOrders.at(i).getId() == orderId){
                return *inProcessOrders.at(i);
            }
        }

         for (int i = 0; i < int(completedOrders.size()); i++){//need to br checked
            if (*completedOrders.at(i).getId() == orderId){
                return *completedOrders.at(i);
            }
        }
    }

const vector<BaseAction*>& WareHouse::getActions() const{
    return actionsLog;
    }

void WareHouse::close(){
 //use action close - elaborate in assignment 1

    isOpen = false;
}

void WareHouse::open(){
    isOpen=true;
}



 


