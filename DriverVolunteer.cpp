#include <iostream>
#include "../include/Volunteer.h"

DriverVolunteer::DriverVolunteer(int id1, string name1, int maxDistance1, int distancePerStep1):Volunteer(id1, name1),maxDistance(maxDistance1),distancePerStep(distancePerStep1),distanceLeft(0){
   
}

 DriverVolunteer* DriverVolunteer::clone() const{//we deleted override
    return new DriverVolunteer(*this);

 }

 int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
 }

 int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
 }

 int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
 }

  bool DriverVolunteer::decreaseDistanceLeft(){//if distanceLeft=0 in first place?
    distanceLeft = distanceLeft - distancePerStep;

    if(distanceLeft<=0){
        distanceLeft=0;
        return true;
    }

    else{
        return false;
    }

  }

   bool DriverVolunteer::hasOrdersLeft() const{
    return true;
   }

   bool DriverVolunteer::canTakeOrder(const Order &order) const{
      return (!isBusy() && order.getDistance()<=maxDistance);
   }

   void DriverVolunteer::acceptOrder(const Order &order){
      distanceLeft = order.getDistance();
   }

   void  DriverVolunteer::step(){
      distanceLeft = distanceLeft - distancePerStep;
   }

   string DriverVolunteer::toString() const{
   return ("id = " + std::to_string(getId()) + "\nVolunteer name = " + getName() +
    "\ncompletedOrderId= " + std::to_string(completedOrderId) + "\nactiveOrderId =" + std::to_string(activeOrderId) +"\nmaxDistance = " + std::to_string(maxDistance) +
    "\ndistancePerStep = " + std::to_string(distancePerStep) + "\ndistanceLeft = " + std::to_string(distanceLeft));
   }

   void DriverVolunteer::setDistanceLeft(int distanceLeft1){
      distanceLeft = distanceLeft1;
   }
