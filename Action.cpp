#include <iostream>
#include "../include/Action.h"

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

//finished BaseAction

SimulateStep::SimulateStep(int numOfSteps1):BaseAction(),numOfSteps(numOfSteps1){}

void SimulateStep::act(WareHouse &wareHouse){

}
