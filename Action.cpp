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
    complete();
    wareHouse.addAction(SimulateStep::clone());
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

    if(customer.getId()==-1 ||  !customer.canMakeOrder()){
        error("Cannot place this order");
    }


    else{
        Order order (wareHouse.getCustomerCounter(), customerId,customer.getCustomerDistance());
        order.setStatus(OrderStatus::PENDING);
        wareHouse.addOrder(&order);
        complete();

    }

    wareHouse.addAction(AddOrder::clone());

}


AddOrder* AddOrder::clone() const{
        return new AddOrder(*this);
}

string AddOrder::toString() const{
    return ("Order " + std::to_string(customerId));
}

 //finished AddOrder--------------------------------------------------------------------
 //AddCustomer

AddCustomer::AddCustomer(string customerName1, string customerType1, int distance1, int maxOrders1):BaseAction(),customerName(customerName1),customerType((customerType1=="soldier") ? CustomerType::Soldier:CustomerType::Civilian),distance(distance1),maxOrders(maxOrders1){
  
}

void AddCustomer::act(WareHouse &wareHouse){
    int id = wareHouse.getCustomerCounter();
    if (customerType==CustomerType::Soldier){
        SoldierCustomer soldier (id, customerName, distance, maxOrders);
        wareHouse.addCustomer(&soldier);
   }

   else{
        CivilianCustomer civilian (id, customerName, distance, maxOrders);
        wareHouse.addCustomer(&civilian);
   }

   complete();
   wareHouse.addAction(AddCustomer::clone());

}

AddCustomer* AddCustomer::clone() const{
    return new AddCustomer(*this);
}

string AddCustomer::toString() const{
    return ("customer " + customerName + " "  + customerTypeToString(customerType) + " " + std::to_string(distance) +" " +  std::to_string(maxOrders));
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
    
    if (wareHouse.getOrder(orderId).getId() == -1){
        error("order does not exist");
    }
    else{
        Order order = wareHouse.getOrder(orderId);   
        cout << order.toString() << endl;
        complete();
    }

    wareHouse.addAction(PrintOrderStatus::clone());

}

PrintOrderStatus* PrintOrderStatus:: clone() const{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const{
    return ("PrintOrderStatus= " + std::to_string(orderId));
}
//finished PrintOrderStatus--------------------------------------------------------------------
//PrintCustomerStatus

PrintCustomerStatus::PrintCustomerStatus(int customerId1):BaseAction(),customerId(customerId1){
    
}

void PrintCustomerStatus::act(WareHouse &wareHouse){
    auto customer = wareHouse.getCustomer(customerId);
    if(customer.getId() == -1){
        error("Customer doesn't exist");
    }

    else{
        string output = "CustomerId: " + std::to_string(customerId);
        auto ordersIds = customer.getOrdersIds();
        for(int id:ordersIds){
            Order order = wareHouse.getOrder(id);
            string status = order.orderStatusToString(order.getStatus());
            output = output +"\nOrderId: " + std::to_string(id) +
            "\nOrderStatus: " + status;

        }

        output = output + "\n numOrdersLeft: " + std::to_string(customer.getOrdersLeft());
        cout << output << endl;
        complete();
    }

    wareHouse.addAction(PrintCustomerStatus::clone());
        

    }

PrintCustomerStatus* PrintCustomerStatus::clone() const{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const{
    return ("customerId:" + std::to_string(customerId));
}

//finished PrintCustomerStatus--------------------------------------------------------------------
//PrintVolunteerStatus

PrintVolunteerStatus::PrintVolunteerStatus(int id):BaseAction(), VolunteerId(id){
}

void PrintVolunteerStatus::act(WareHouse &wareHouse){
    auto vol = wareHouse.getVolunteer(VolunteerId);
    if(vol.getId()==-1){
        error("volunteer does not exist");
    }
    else{
         cout << vol.toString() << endl;
         complete();
    }

    wareHouse.addAction(PrintVolunteerStatus::clone());

}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const{
    return ("VolunteerId: " +  std::to_string(VolunteerId));
}

//finished PrintVolunteerStatus--------------------------------------------------------------------
//PrintActionsLog

PrintActionsLog::PrintActionsLog():BaseAction(){}

void PrintActionsLog::act(WareHouse &wareHouse){
    auto actionsLog = wareHouse.getActions();
    string output = "";
    for(auto action:actionsLog){
        output = output + "\n" +  actionTypeToString(action) +
        action->toString() + PrintActionsLog::actionStatusToString(action->getStatus());
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






