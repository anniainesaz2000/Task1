#include <iostream>
#include "../include/Action.h"
#include <typeinfo>

BaseAction::BaseAction():errorMsg(""),status(ActionStatus::ERROR){}

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){
    status(ActionStatus::COMPLETED);
}

void BaseAction::error(string errorMsg){
    errorMsg(errorMsg);
    status(ActionStatus::ERROR);
}

string BaseAction::getErrorMsg() const{
    return errorMsg;
}

//finished BaseAction------------------------------------------------------------------

SimulateStep::SimulateStep(int numOfSteps1):BaseAction(),numOfSteps(numOfSteps1){}

void SimulateStep::act(WareHouse &wareHouse){
    while(numOfSteps>0){
        actOneStep(wareHouse);
    }
}


void SimulateStep::actOneStep(WareHouse &wareHouse){

    vector<Order*> & pendingOrders = wareHouse.getPendingOrders();
    for (int i = 0; i < int(pendingOrders.size()); i++){//need to be checked
        Order order = *(pendingOrders.at(i));
        if (order.getStatus() == OrderStatus::PENDING){
            SimulateStep::orderToCollectorVolunteer(wareHouse, order, i);
        }

        else if (order.getStatus() == OrderStatus::COLLECTING){
            SimulateStep::orderToDriverVolunteer(wareHouse, order, i);
        }


    }

    vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();
    for (int i = 0; i < int(inProcessOrders.size()); i++){//need to be checked
        Order order = *(inProcessOrders.at(i));
        if (order.getStatus() == OrderStatus::COLLECTING){
            SimulateStep::checkCollectingOrderInProcess(wareHouse, order, i);
        }

        else if (order.getStatus() == OrderStatus::DELIVERING){
            SimulateStep::checkDeliveringOrderInProcess(wareHouse,order, i);
        }


    }

    
}

void SimulateStep::orderToCollectorVolunteer(WareHouse &wareHouse, Order& order, int pendIndex){
    vector<Volunteer*> & volunteers = wareHouse.getVolunteers();
    vector<Order*> & pendingOrders = wareHouse.getPendingOrders();
    vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();

    bool orderAccepted = false;

     for (int i = 0; !orderAccepted && i < int(volunteers.size()); i++){//need to be checked
            Volunteer volunteer = *(volunteers.at(i));

            if ((typeid(volunteer) == typeid(CollectorVolunteer) ||  typeid(volunteer) == typeid(LimitedCollectorVolunteer)) && !(*(volunteers.at(i)).canTakeOrder(order))){//maybe we don't need or?
                order.setStatus(OrderStatus::COLLECTING);
                order.setCollectorId(volunteer.getId());
                volunteer.acceptOrder(order);
                orderAccepted = true;
                inProcessOrders.push_back(&order);
                pendingOrders.erase(pendingOrders.begin() + pendIndex);

            }
        }

}


void SimulateStep::orderToDriverVolunteer(WareHouse &wareHouse, Order& order, int pendIndex){
    vector<Volunteer*> & volunteers = wareHouse.getVolunteers();
    vector<Order*> & pendingOrders = wareHouse.getPendingOrders();
    vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();

    bool orderAccepted = false;

     for (int i = 0; !orderAccepted && i < int(volunteers.size()); i++){//need to be checked
            Volunteer volunteer = *(volunteers.at(i));

            if ((typeid(volunteer) == typeid(DriverVolunteer) ||  typeid(volunteer) == typeid(LimitedDriverVolunteer)) && !(*(volunteers.at(i)).canTakeOrder(order))){//maybe we don't need or?
                order.setStatus(OrderStatus::DELIVERING);
                order.setCollectorId(volunteer.getId());
                volunteer.acceptOrder(order);
                orderAccepted = true;
                inProcessOrders.push_back(&order);
                pendingOrders.erase(pendingOrders.begin() + pendIndex);

            }
        }

}

void SimulateStep::checkCollectingOrderInProcess(WareHouse &wareHouse, Order& order, int inProIndex){
    int collectorId = order.getCollectorId();
    Volunteer collector = wareHouse.getVolunteer(collectorId);
    vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();
    vector<Order*> & pendingOrders = wareHouse.getPendingOrders();

    collector.step();

    if(collector.getTimeLeft()==0){
        order.setCollectorId(NO_VOLUNTEER);
        pendingOrders.push_back(&order);
        inProcessOrders.erase(inProcessOrders.begin() + inProIndex);
        if(typeid(collector) == typeid(LimitedCollectorVolunteer)){
            collector.decreaseOrdersLeft();
            if(collector.getNumOrdersLeft()==0){
                delete collector;
            }
        }

    }

}

void SimulateStep::checkDeliveringOrderInProcess(WareHouse &wareHouse, Order& order, int inProIndex){
    int driverId = order.getDriverId();
    Volunteer driver = wareHouse.getVolunteer(driverId);
    vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();
    vector<Order*> & completedOrders = wareHouse.getCompletedOrders();

    driver.step();

    if(driver.getDistanceLeft()==0){
        order.setStatus(OrderStatus::COMPLETED);
        order.setDriverId(NO_VOLUNTEER);
        completedOrders.push_back(&order);
        inProcessOrders.erase(inProcessOrders.begin() + inProIndex);
        if(typeid(driver) == typeid(LimitedDriverVolunteer)){
            driver.decreaseOrdersLeft();
            if(driver.getNumOrdersLeft()==0){
                delete driver;
            }
        }

    }

}

std::string SimulateStep::toString() const{
    //what do we need to print?

}

SimulateStep* SimulateStep::clone() const{
    return new SimulateStep(*this);

}



