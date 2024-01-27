#include <iostream>
#include "../include/Action.h"
#include <typeinfo>

//BaseAction

BaseAction::BaseAction():errorMsg(""),status(ActionStatus::ERROR){}

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg1){
    errorMsg = errorMsg1;
    status = ActionStatus::ERROR;
}

string BaseAction::getErrorMsg() const{
    return errorMsg;
}

//finished BaseAction------------------------------------------------------------------
//SimulateStep

SimulateStep::SimulateStep(int numOfSteps1):BaseAction(),numOfSteps(numOfSteps1){}

void SimulateStep::act(WareHouse &wareHouse){
    for(int i=0; i<numOfSteps; i++){
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
            auto volunteer = *(volunteers.at(i));

            if ((typeid(volunteer) == typeid(CollectorVolunteer) ||  typeid(volunteer) == typeid(LimitedCollectorVolunteer)) && (*(volunteers.at(i)).canTakeOrder(order))){//maybe we don't need or?
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
            auto volunteer = *(volunteers.at(i));

            if ((typeid(volunteer) == typeid(DriverVolunteer) ||  typeid(volunteer) == typeid(LimitedDriverVolunteer)) && (*(volunteers.at(i)).canTakeOrder(order))){//maybe we don't need or?
                order.setStatus(OrderStatus::DELIVERING);
                order.setDriverId(volunteer.getId());
                volunteer.acceptOrder(order);
                orderAccepted = true;
                inProcessOrders.push_back(&order);
                pendingOrders.erase(pendingOrders.begin() + pendIndex);

            }
        }

}

void SimulateStep::checkCollectingOrderInProcess(WareHouse &wareHouse, Order& order, int inProIndex){
    int collectorId = order.getCollectorId();
    auto collector = wareHouse.getVolunteer(collectorId);
    vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();
    vector<Order*> & pendingOrders = wareHouse.getPendingOrders();

    collector.step();

    if(collector.getTimeLeft()==0){
        order.setCollectorId(NO_VOLUNTEER);
        pendingOrders.push_back(&order);
        inProcessOrders.erase(inProcessOrders.begin() + inProIndex);
        // if(typeid(collector) == typeid(LimitedCollectorVolunteer)){
        //     if(collector.getNumOrdersLeft()==0){
        //         delete (&collector);
        //     }
        // }

    }

}

void SimulateStep::checkDeliveringOrderInProcess(WareHouse &wareHouse, Order& order, int inProIndex){
    int driverId = order.getDriverId();
    auto driver = wareHouse.getVolunteer(driverId);
    vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();
    vector<Order*> & completedOrders = wareHouse.getCompletedOrders();

    driver.step();

    if(driver.getDistanceLeft()==0){
        order.setStatus(OrderStatus::COMPLETED);
        order.setDriverId(NO_VOLUNTEER);
        completedOrders.push_back(&order);
        inProcessOrders.erase(inProcessOrders.begin() + inProIndex);
        // if(typeid(driver) == typeid(LimitedDriverVolunteer)){
        //     if(driver.getNumOrdersLeft()==0){
        //         delete driver;
        //     }
        // }

    }

}

std::string SimulateStep::toString() const{
    return ("step = " + std::to_string(numOfSteps));

}

SimulateStep* SimulateStep::clone() const{
    return new SimulateStep(*this);

}

//finished SimulateStep------------------------------------------------------------------
//AddOrder

AddOrder:: AddOrder(int id):BaseAction(),customerId(id){
      
}


void AddOrder::act(WareHouse &wareHouse){
    auto customer = wareHouse.getCustomer(customerId);

    if(customer==nullptr ||  !customer.canMakeOrder()){
        error("Cannot place this order");
    }


    else{
        Order order (wareHouse.getCustomerCounter(), customerId,customer.getCustomerDistance());
        order.setStatus(OrderStatus::PENDING);
        wareHouse.addOrder(&order);
        wareHouse.increaseOrderCounter();

    }

}


AddOrder* AddOrder::clone() const{
        return new AddOrder(*this);
}

string AddOrder::toString() const{
    return ("Order " + std::to_string(customerId));
}

 //finished AddOrder--------------------------------------------------------------------
 //AddCustomer















//////////////////////////////////////////////////////////////////////////////////


// PrintOrderStatus::PrintOrderStatus(int id):orderId(id){

// }
// PrintOrderStatus::act(WareHouse &wareHouse){
    
//     if (wareHouse.getOrder(orderId) == nullptr){
//         return error("order does not exist");
//     }
//     else{
//         Order order = *(wareHouse.getOrder(orderId));   
//         OrderStatus orderStatus = wareHouse.getOrder(orderId).getStatus(); //in the stack, suppose to be deleted at the end of AF
//         cout << "Order " + std::to_string(orderId) +
//          "\nOrderStatus: " orderStatusToString(orderStatus) +
//          "\nCustomerId:" + std::to_string(order.getCustomerId()) +
//          "\nCollector" + std::to_string(order.getCollectorId()) +  // it is written the suppose to print None if there is no collector etc
//          "\nDriver" + std::to_string(order.getDriverId()) +
//          << endl;
//          status(ActionStatus::COMPLETED);
//     }

// }
// PrintOrderStatus::PrintOrderStatus *clone() const{
//     return new PrintOrderStatus(*this);
// }
// PrintOrderStatus::string toString() const{
//     return "PrintOrderStatus " + std::to_string(orderId);
}
//////////////////////////////////////////////////////////////////////////////














///////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////
// PrintVolunteerStatus::PrintVolunteerStatus(int id):volunteerId(id){
// }
// PrintVolunteerStatus::act(WareHouse &wareHouse){
//     if(wareHouse.getVolunteer(volunteerId) == nullptr){
//         return error("volunteer does not exist");
//     }
//     else{
//         Volunteer volunteer = *(wareHouse.getVolunteer(volunteerId));

//         //if volunteer is a LimitedCollector
//         if(typeid(volunteer) == typeid(CollectorVolunteer)){
//             if( typeid(volunteer) == typeid(LimitedCollectorVolunteer)){
//                 if(volunteer.isBusy() == false){
//                       cout << 
//                        "Volunteer " + std::to_string(volunteerId) +
//                         "\nisBusy: " + "False" +
//                         "\nOrderId: " + "None" +
//                         "\nTimeLeft: " + "None" +
//                         "\nOrdersLeft: " + std::to_string(volunteer.getNumOrders()) +
//                         << endl;
//                 }
//                 else{ // isBusy() == true
//                     cout << 
//                       "Volunteer " + std::to_string(volunteerId) +
//                        "\nisBusy: " + "True" +
//                        "\nOrderId: " + std::to_string(volunteer.getActiveOrderId()) +
//                         "\nTimeLeft: " + std::to_string(volunteer.getTimeLeft()) +
//                        "\nOrdersLeft: " + std::to_string(volunteer.getNumOrders()) +
//                     << endl;
//                 }
//             }
//             // volunteer is a Collector
//             else if (volunteer.isBusy() == false) {
//                 cout << 
//                        "Volunteer " + std::to_string(volunteerId) +
//                         "\nisBusy: " + "False" +
//                         "\nOrderId: " + "None" +
//                         "\nTimeLeft: " + "None" +
//                         "\nOrdersLeft: " + "No Limit" +
//                         << endl;
//                 }
//                 else{ // isBusy() == true
//                 cout << 
//                       "Volunteer " + std::to_string(volunteerId) +
//                        "\nisBusy: " + "True" +
//                        "\nOrderId: " + std::to_string(volunteer.getActiveOrderId()) +
//                         "\nTimeLeft: " + std::to_string(volunteer.getTimeLeft()) +
//                        "\nOrdersLeft: " + "No Limit" +
//                     << endl;
//             }
//         }
//         //volunteer is driver
//         else{ //LimitedDriverVolunteer
//             if(typeid(volunteer) == typeid(LimitedDriverVolunteer)){
//                 if (volunteer.isBusy() == false){
//                     cout << 
//                         "Volunteer " + std::to_string(volunteerId) +
//                         "\nisBusy: " + "False" +
//                         "\nOrderId: " + "None" +
//                         "\nDistanceLeft: " + "None" +
//                         "\nOrdersLeft: " + std::to_string(volunteer.getNumOrders()) +
//                         << endl;
//              }
//                 else{ // isBusy() == true
//                         cout << 
//                             "Volunteer " + std::to_string(volunteerId) +
//                             "\nisBusy: " + "True" +
//                             "\nOrderId: " + std::to_string(volunteer.getActiveOrderId()) +
//                             "\nDistanceLeft: " + std::to_string(volunteer.getDistanceLeft()) +
//                             "\nOrdersLeft: " + std::to_string(volunteer.getNumOrders()) +
//                         << endl;
//                 }
//              }
//              else{ //DriverVolunteer
//                     if (volunteer.isBusy() == false){
//                         cout << 
//                             "Volunteer " + std::to_string(volunteerId) +
//                             "\nisBusy: " + "False" +
//                             "\nOrderId: " + "None" +
//                             "\nDistanceLeft: " + "None" +
//                             "\nOrdersLeft: " + "No Limit" +
//                             << endl;
//                 }
//                     else{ // isBusy() == true
//                             cout << 
//                                 "Volunteer " + std::to_string(volunteerId) +
//                                 "\nisBusy: " + "True" +
//                                 "\nOrderId: " + std::to_string(volunteer.getActiveOrderId()) +
//                                 "\nDistanceLeft: " + std::to_string(volunteer.getDistanceLeft()) +
//                                 "\nOrdersLeft: " + "No Limit" +
//                             << endl;
//                     }
//              }
//             }   
//     }
// }


///////////////////////////////////////////////////////////////////////////////////////////////////////////




