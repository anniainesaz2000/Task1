#include <iostream>
#include "../include/Customer.h"

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders):Customer(id, name, locationDistance, maxOrders){

}

CivilianCustomer* CivilianCustomer::clone() const{
    return new CivilianCustomer(*this);
}
