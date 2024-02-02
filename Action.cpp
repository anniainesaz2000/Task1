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


// SimulateStep::SimulateStep(int numOfSteps1):BaseAction(),numOfSteps(numOfSteps1){}

// void SimulateStep::act(WareHouse &wareHouse){
//     for(int i=0; i<numOfSteps; i++){
//         actOneStep(wareHouse);
//     }
//     complete();
//     wareHouse.addAction(SimulateStep::clone());
// }


// void SimulateStep::actOneStep(WareHouse &wareHouse){

//     vector<Order*> & pendingOrders = wareHouse.getPendingOrders();
//     for (Order* order : pendingOrders){
//         if (order->getStatus() == OrderStatus::PENDING){
//             SimulateStep::orderToCollectorVolunteer(wareHouse, order);
//         }

//         else if (order->getStatus() == OrderStatus::COLLECTING){
//             SimulateStep::orderToDriverVolunteer(wareHouse, order);
//         }


//     }

//     vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();
//     for (Order* order : inProcessOrders){

//         if (order->getStatus() == OrderStatus::COLLECTING){
//             SimulateStep::checkCollectingOrderInProcess(wareHouse, order);
//         }

//         else if (order->getStatus() == OrderStatus::DELIVERING){
//             SimulateStep::checkDeliveringOrderInProcess(wareHouse,order);
//         }


//     }

    
// }

// void SimulateStep::orderToCollectorVolunteer(WareHouse &wareHouse, Order* order){
//     vector<Volunteer*> & volunteers = wareHouse.getVolunteers();
//     vector<Order*> & pendingOrders = wareHouse.getPendingOrders();
//     vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();

//      for (Volunteer* vol : volunteers){

//             if ((typeid(*vol) == typeid(CollectorVolunteer) ||  typeid(*vol) == typeid(LimitedCollectorVolunteer)) && ((*vol).canTakeOrder(*order))){
//                 order->setStatus(OrderStatus::COLLECTING);
//                 order->setCollectorId(vol->getId());
//                 vol->acceptOrder(*order);
//                 inProcessOrders.push_back(order);
//                 int pendIndex = wareHouse.getIndexInPendingOrder(order->getId());
//                 pendingOrders.erase(pendingOrders.begin() + pendIndex);
//                 break;

//             }
//         }

// }


// void SimulateStep::orderToDriverVolunteer(WareHouse &wareHouse, Order* order){
//     vector<Volunteer*> & volunteers = wareHouse.getVolunteers();
//     vector<Order*> & pendingOrders = wareHouse.getPendingOrders();
//     vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();

//      for (Volunteer* vol : volunteers){

//             if ((typeid(*vol) == typeid(DriverVolunteer) ||  typeid(*vol) == typeid(LimitedDriverVolunteer)) && (vol->canTakeOrder(*order))){
//                 order->setStatus(OrderStatus::DELIVERING);
//                 order->setDriverId((*vol).getId());
//                 vol->acceptOrder(*order);
//                 inProcessOrders.push_back(order);
//                 int pendIndex = wareHouse.getIndexInPendingOrder(order->getId());
//                 pendingOrders.erase(pendingOrders.begin() + pendIndex);

//             }
//         }

// }

// void SimulateStep::checkCollectingOrderInProcess(WareHouse &wareHouse, Order* order){
//     int collectorId = order->getCollectorId();
//     auto collector = &(wareHouse.getVolunteer(collectorId));
//     vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();
//     vector<Order*> & pendingOrders = wareHouse.getPendingOrders();

//     static_cast<CollectorVolunteer*>(collector)->step();

//     if(static_cast<CollectorVolunteer*>(collector)->getTimeLeft()==0){
//         order->setCollectorId(NO_VOLUNTEER);
//         pendingOrders.push_back(order);
//         int inProIndex = wareHouse.getIndexInProcessOrder(order->getId());
//         inProcessOrders.erase(inProcessOrders.begin() + inProIndex);
//         if(typeid(collector) == typeid(LimitedCollectorVolunteer)){
//             if(static_cast<LimitedCollectorVolunteer*>(collector)->getNumOrdersLeft()==0){
//                 delete (collector);
//             }
//         }

//     }

// }

// void SimulateStep::checkDeliveringOrderInProcess(WareHouse &wareHouse, Order* order){
//     int driverId = order->getDriverId();
//     auto driver = &(wareHouse.getVolunteer(driverId));
//     vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();
//     vector<Order*> & completedOrders = wareHouse.getCompletedOrders();

//     static_cast<DriverVolunteer*>(driver)->step();

//     if(static_cast<DriverVolunteer*>(driver)->getDistanceLeft()==0){
//         order->setStatus(OrderStatus::COMPLETED);
//         order->setDriverId(NO_VOLUNTEER);
//         completedOrders.push_back(order);
//         int inProIndex = wareHouse.getIndexInProcessOrder(order->getId());
//         inProcessOrders.erase(inProcessOrders.begin() + inProIndex);
//         if(typeid(driver) == typeid(LimitedDriverVolunteer)){
//             if(static_cast<LimitedDriverVolunteer*>(driver)->getNumOrdersLeft()==0){
//                 delete (driver);
//             }
//         }

//     }

// }

// std::string SimulateStep::toString() const{
//     return ("step = " + std::to_string(numOfSteps));

// }

// SimulateStep* SimulateStep::clone() const{
//     return new SimulateStep(*this);

// }

//finished SimulateStep------------------------------------------------------------------





















SimulateStep::SimulateStep(int numOfSteps1):BaseAction(),numOfSteps(numOfSteps1){}

void SimulateStep::act(WareHouse &wareHouse){
    for(int i=0; i<numOfSteps; i++){
        actOneStep(wareHouse);
    }
    complete();
    wareHouse.addAction(SimulateStep::clone());
}


void SimulateStep::actOneStep(WareHouse &wareHouse){

    vector<Volunteer*> & volunteers= wareHouse.getVolunteers();

    Order order(-1, -1, -1);
    for (Volunteer* vol : volunteers) {
        if(vol->canTakeOrder(order)){
            if(typeid(*vol) == typeid(CollectorVolunteer) ||  typeid(*vol) == typeid(LimitedCollectorVolunteer)){
                orderToCollectorVolunteer(wareHouse, vol);
                
            }

            else{
                orderToDriverVolunteer(wareHouse, vol);

            }
        }

        else{
            if(typeid(*vol) == typeid(CollectorVolunteer) ||  typeid(*vol) == typeid(LimitedCollectorVolunteer)){
                checkCollectorInProcess(wareHouse, vol);
                
            }

            else{
                checkDriverInProcess(wareHouse, vol);

            }
            
        }
    }

    
}

void SimulateStep::orderToCollectorVolunteer(WareHouse &wareHouse, Volunteer* collector){
    vector<Order*> & pendingOrders = wareHouse.getPendingOrders();
    vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();
    int i = 0;

    for(Order* order : pendingOrders){
        if(order->getStatus() == OrderStatus::PENDING){
            static_cast<CollectorVolunteer*>(collector)->acceptOrder(*order);
            order->setStatus(OrderStatus::COLLECTING);
            order->setCollectorId(collector->getId());
            inProcessOrders.push_back(order);
            pendingOrders.erase(pendingOrders.begin() + i);

            break;
                    }

            }
            i+=1;

}


void SimulateStep::orderToDriverVolunteer(WareHouse &wareHouse, Volunteer* driver){
    vector<Order*> & pendingOrders = wareHouse.getPendingOrders();
    vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();
 
    int i = 0;

     for(Order* order : pendingOrders){
        if(order->getStatus() == OrderStatus::COLLECTING){
            static_cast<DriverVolunteer*>(driver)->acceptOrder(*order);
            order->setStatus(OrderStatus::DELIVERING);
            order->setDriverId(driver->getId());
            inProcessOrders.push_back(order);
            pendingOrders.erase(pendingOrders.begin() + i);

            break;
                    }

            }
            i+=1;
}

void SimulateStep::checkCollectorInProcess(WareHouse &wareHouse, Volunteer* collector){
    vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();
    vector<Order*> & pendingOrders = wareHouse.getPendingOrders();

    static_cast<CollectorVolunteer*>(collector)->step();

    if(static_cast<CollectorVolunteer*>(collector)->getTimeLeft()==0){
        int orderId = collector->getActiveOrderId();
        Order* order = &(wareHouse.getOrder(orderId));
        order->setCollectorId(NO_VOLUNTEER);
        pendingOrders.push_back(order);
        int inProIndex = wareHouse.getIndexInProcessOrder(orderId);
        inProcessOrders.erase(inProcessOrders.begin() + inProIndex);
        if(typeid(collector) == typeid(LimitedCollectorVolunteer)){
            if(static_cast<LimitedCollectorVolunteer*>(collector)->getNumOrdersLeft()==0){
                delete collector;
            }
        }

    }

}

void SimulateStep::checkDriverInProcess(WareHouse &wareHouse, Volunteer* driver){
    vector<Order*> & inProcessOrders = wareHouse.getInProcessOrders();
    vector<Order*> & completedOrders = wareHouse.getCompletedOrders();
   
    static_cast<DriverVolunteer*>(driver)->step();

    if(static_cast<DriverVolunteer*>(driver)->getDistanceLeft()==0){
        int orderId = driver->getActiveOrderId();
        Order* order = &(wareHouse.getOrder(orderId));
        order->setStatus(OrderStatus::COMPLETED);
        order->setDriverId(NO_VOLUNTEER);
        completedOrders.push_back(order);
        int inProIndex = wareHouse.getIndexInProcessOrder(orderId);
        inProcessOrders.erase(inProcessOrders.begin() + inProIndex);
        if(typeid(driver) == typeid(LimitedDriverVolunteer)){
            if(static_cast<LimitedDriverVolunteer*>(driver)->getNumOrdersLeft()==0){
                delete driver;
            }
        }

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

    if(wareHouse.customerExist(customerId)){
        auto customer = &(wareHouse.getCustomer(customerId));

        if( !(*customer).canMakeOrder()){
            error("Cannot place this order");
        }

        else{
        Order* order = new Order (wareHouse.getOrderCounter(), customerId,(*customer).getCustomerDistance());
        (*order).setStatus(OrderStatus::PENDING);
        wareHouse.addOrder(order);
        (*customer).addOrder((*order).getId());
        complete();

    }

    }

    else{
        error("Cannot place this order");
    }

    wareHouse.addAction(AddOrder::clone());

}


AddOrder* AddOrder::clone() const{
        return new AddOrder(*this);
}

string AddOrder::toString() const{
    return (" Order " + std::to_string(customerId));
}

 //finished AddOrder--------------------------------------------------------------------
 //AddCustomer

AddCustomer::AddCustomer(string customerName1, string customerType1, int distance1, int maxOrders1):BaseAction(),customerName(customerName1),customerType((customerType1=="soldier") ? CustomerType::Soldier:CustomerType::Civilian),distance(distance1),maxOrders(maxOrders1){
  
}

void AddCustomer::act(WareHouse &wareHouse){
    int id = wareHouse.getCustomerCounter();
    if (customerType==CustomerType::Soldier){
        SoldierCustomer* soldier = new  SoldierCustomer(id, customerName, distance, maxOrders);
        wareHouse.addCustomer(soldier);
   }

   else{
        CivilianCustomer* civilian = new CivilianCustomer(id, customerName, distance, maxOrders);
        wareHouse.addCustomer(civilian);
   }

   complete();
   wareHouse.addAction(AddCustomer::clone());

}

AddCustomer* AddCustomer::clone() const{
    return new AddCustomer(*this);
}

string AddCustomer::toString() const{
    return (" customer " + customerName + " "  + customerTypeToString(customerType) + " " + std::to_string(distance) +" " +  std::to_string(maxOrders));
}

string AddCustomer::customerTypeToString(CustomerType customerType)const{
    if(customerType == CustomerType::Soldier){
        return "soldier";
    }

    else{
        return "civilian";
    }

}

//finished AddCustomer--------------------------------------------------------------------
//PrintOrderStatus

PrintOrderStatus::PrintOrderStatus(int id):BaseAction(), orderId(id){

}

void PrintOrderStatus::act(WareHouse &wareHouse){
    
    if (!(wareHouse.orderExist(orderId))){
        error("order does not exist");
    }
    else{
        Order order = wareHouse.getOrder(orderId); 
        cout<< order.toString()<<endl;
        complete();
    }

    wareHouse.addAction(PrintOrderStatus::clone());

}

PrintOrderStatus* PrintOrderStatus:: clone() const{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const{
    return (" PrintOrderStatus= " + std::to_string(orderId));
}
//finished PrintOrderStatus--------------------------------------------------------------------
//PrintCustomerStatus

PrintCustomerStatus::PrintCustomerStatus(int customerId1):BaseAction(),customerId(customerId1){
    
}

void PrintCustomerStatus::act(WareHouse &wareHouse){

    if(!(wareHouse.customerExist(customerId))){
         error("Customer doesn't exist");
    }
    else{
        auto customer = &(wareHouse.getCustomer(customerId));
        string output = "--------------------- \nCustomerId: " + std::to_string(customerId);
        auto ordersIds = (*customer).getOrdersIds();
        for(int id:ordersIds){
            Order order = wareHouse.getOrder(id);
            string status = order.orderStatusToString(order.getStatus());
            output = output +"\nOrderId: " + std::to_string(id) +
            "\nOrderStatus: " + status;

        }

        output = output + "\n numOrdersLeft: " + std::to_string((*customer).getOrdersLeft()) + + "\n--------------------- ";
        cout << output << endl;
        complete();
    }

    wareHouse.addAction(PrintCustomerStatus::clone());
        

    }

PrintCustomerStatus* PrintCustomerStatus::clone() const{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const{
    return (" customerId: " + std::to_string(customerId));
}

//finished PrintCustomerStatus--------------------------------------------------------------------
//PrintVolunteerStatus

PrintVolunteerStatus::PrintVolunteerStatus(int id):BaseAction(), VolunteerId(id){
}

void PrintVolunteerStatus::act(WareHouse &wareHouse){
    if(!(wareHouse.volunteerExist(VolunteerId))){
         error("Customer doesn't exist");
    }

    else{
        auto vol = &(wareHouse.getVolunteer(VolunteerId));
        cout << (*vol).toString() << endl;
        complete();

    }

    wareHouse.addAction(PrintVolunteerStatus::clone());

}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const{
    return (" VolunteerId: " +  std::to_string(VolunteerId));
}

//finished PrintVolunteerStatus--------------------------------------------------------------------
//PrintActionsLog

PrintActionsLog::PrintActionsLog():BaseAction(){}

void PrintActionsLog::act(WareHouse &wareHouse){
    auto actionsLog = wareHouse.getActions();
    string output = "";
    for(auto action:actionsLog){
        output = output + "\n" +  actionTypeToString(action) +
        action->toString() + " " + PrintActionsLog::actionStatusToString(action->getStatus());
    }

    cout << output << endl;

}

string PrintActionsLog::actionTypeToString(BaseAction* action)const{
    if(typeid(*action) == typeid(SimulateStep)){
        return "SimulateStep";
    }

    else if(typeid(*action) == typeid(AddOrder)){
        return "AddOrder";
    }

    else if(typeid(*action) == typeid(AddCustomer)){
        return "AddCustomer";
    }

    else if(typeid(*action) == typeid(PrintOrderStatus)){
        return "PrintOrderStatus";
    }

    else if(typeid(*action) == typeid(PrintCustomerStatus)){
        return "PrintCustomerStatus";
    }

     else if(typeid(*action) == typeid(PrintVolunteerStatus)){
        return "PrintVolunteerStatus";
    }

     else if(typeid(*action) == typeid(BackupWareHouse)){
        return "BackupWareHouse";
    }

    else if(typeid(*action) == typeid(BackupWareHouse)){
        return "RestoreWareHouse";
    }

    return "";
}

std::string PrintActionsLog::actionStatusToString(ActionStatus status)const {
    switch (status) {
        case ActionStatus::COMPLETED:
            return "COMPLETED";
        case ActionStatus::ERROR:
            return "ERROR";
        default:
            return "UNDEFINED";
    }
}

PrintActionsLog* PrintActionsLog::clone() const{
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const{
    return "";
}

//finished PrintActionsLog--------------------------------------------------------------------
//Close

Close::Close():BaseAction(){}

void Close::act(WareHouse &wareHouse){
    for (Order* penOrder : wareHouse.getPendingOrders()){
         cout << penOrder->closeToString() << endl;
    }
    for (Order* ProOrder : wareHouse.getInProcessOrders()){
         cout << ProOrder->closeToString() << endl;
    }
    for (Order* compOrder : wareHouse.getCompletedOrders()){
         cout << compOrder->closeToString() << endl;
    }

    wareHouse.close();
    complete();

}

Close* Close::clone() const{
    return new Close(*this);
}

string Close::toString() const{
    return "";
}

//finished Close--------------------------------------------------------------------
//BackupWareHouse

BackupWareHouse::BackupWareHouse():BaseAction(){}

void BackupWareHouse::act(WareHouse &wareHouse){
    if(backup==nullptr){
        backup = new WareHouse(wareHouse);
    }

    else{
        *backup = wareHouse;
    }

    complete();
}

 BackupWareHouse* BackupWareHouse::clone() const{
    return new BackupWareHouse(*this);
 }

 string BackupWareHouse::toString() const{
    return "";
 }

//finished BackupWareHouse--------------------------------------------------------------------
//RestoreWareHouse

RestoreWareHouse::RestoreWareHouse():BaseAction(){}

void RestoreWareHouse::act(WareHouse &wareHouse){
    if(backup==nullptr){
        error("No backup available");
    }

    else{
         wareHouse = *backup;
         complete();
    }
}

 RestoreWareHouse* RestoreWareHouse::clone() const{
    return new RestoreWareHouse(*this);
 }

 string RestoreWareHouse::toString() const{
     return "";
 }
//finished RestoreWareHouse--------------------------------------------------------------------




