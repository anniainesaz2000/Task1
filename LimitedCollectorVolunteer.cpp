#include <iostream>
#include "../include/Volunteer.h"

 LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id1, string name1, int coolDown1 ,int maxOrders1):CollectorVolunteer(id1, name1, coolDown1),maxOrders(maxOrders1),ordersLeft(maxOrders){

 }

 LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const{//we deleted "override"
    return new LimitedCollectorVolunteer(*this);

 }

 LimitedCollectorVolunteer::~LimitedCollectorVolunteer(){
    delete this;
 }

 bool  LimitedCollectorVolunteer::hasOrdersLeft() const{//we dfeleted override
    if (ordersLeft>0){
        return true;
    }

    else{
        return false;
    }

 }

 bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    //how to use the paraneter order?
    if(!isBusy() && hasOrdersLeft()){
        return true;
    }

    else{
        return false;
    }
 }

void LimitedCollectorVolunteer::acceptOrder(const Order &order){
    if(canTakeOrder(order)){
        CollectorVolunteer::acceptOrder(order);
        ordersLeft = ordersLeft-1;
    }
    
    }

int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const{
    return (CollectorVolunteer::toString() + "\n maxOrders = " + std::to_string(maxOrders) +"\nordersLeft = " + std::to_string(ordersLeft));
}









