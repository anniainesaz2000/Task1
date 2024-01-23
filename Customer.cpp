#include <iostream>
#include "../include/Customer.h"

Customer::Customer(int id1, const string &name1, int locationDistance1, int maxOrders1): id(id1), name(name1),
        locationDistance(locationDistance1), maxOrders(maxOrders1), ordersId(vector<int>()){
        //ordersId = vector<int>();
}

const string& Customer::getName() const{
    return name;   
}

int Customer::getId() const{
    return id;
}

int Customer::getCustomerDistance() const{
    return locationDistance;
}

int Customer::getMaxOrders() const{
    return maxOrders;
} 

int Customer::getNumOrders() const{
    return ordersId.size();
} 

bool Customer::canMakeOrder() const{
    return ((int)ordersId.size()) < maxOrders;
} 

const vector<int> & Customer::getOrdersIds() const{
    return ordersId;

}

int Customer::addOrder(int orderId){
    if (canMakeOrder()){
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;

} 






