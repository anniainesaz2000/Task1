#include <iostream>
#include "Volunteer.h"

LimitedDriverVolunteer::LimitedDriverVolunteer(int id1, const string &name1, int maxDistance1, int distancePerStep1,int maxOrders1):DriverVolunteer(id1, name1, maxDistance1, distancePerStep1),maxOrders(maxOrders1){
    ordersLeft = maxOrders;
}

LimitedDriverVolunteer LimitedDriverVolunteer::*clone() const{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}

 int  LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
 }

bool LimitedDriverVolunteer::hasOrdersLeft() const{
    return (ordersLeft>0);
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const{ //do we need to consider hasOrdersLeft?
    return (!isBusy() && order.getDistance()<=getMaxDistance() && hasOrdersLeft());
}

void LimitedDriverVolunteer::acceptOrder(const Order &order){
    setDistanceLeft(order.getDistance());
    ordersLeft = ordersLeft - 1;
}

string LimitedDriverVolunteer::toString() const{
    return ("id = " + getId() + "\nVolunteer name = " + getName() +
    "\ncompletedOrderId= " + completedOrderId + "\nactiveOrderId =" + activeOrderId +"\nmaxDistance = " + getMaxDistance() +
    "\ndistancePerStep = " + getDistancePerStep() + "\ndistanceLeft = " + getDistanceLeft() + "\nmaxOrders = " + maxOrders + "\nordersLeft = " + ordersLeft);
}
