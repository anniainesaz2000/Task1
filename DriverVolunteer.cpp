#include <iostream>
#include "Volunteer.h"

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep):Volunteer(id, name),*this.maxDistance(maxDistance),*this.distancePerStep(distancePerStep){
    distanceLeft = 0;
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
