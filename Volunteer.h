#pragma once
#include <string>
#include <vector>
#include "../include/Order.h"
using std::string;
using std::vector;

#define NO_ORDER -1

class Volunteer {
    public:
        Volunteer(int id, const string &name);
        int getId() const;
        const string &getName() const;
        int getActiveOrderId() const;
        int getCompletedOrderId() const;
        bool isBusy() const; // Signal whether the volunteer is currently processing an order   
        virtual ~Volunteer()=default; // we added it 
        virtual bool hasOrdersLeft() const = 0; // Signal whether the volunteer didn't reach orders limit,Always true for CollectorVolunteer and DriverVolunteer
        virtual bool canTakeOrder(const Order &order) const = 0; // Signal if the volunteer can take the order.      
        virtual void acceptOrder(const Order &order) = 0; // Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)  
        virtual void step() = 0; //Simulate volunteer step,if the volunteer finished the order, transfer activeOrderId to completedOrderId

        virtual string toString() const = 0;
        virtual Volunteer* clone() const = 0; //Return a copy of the volunteer

    protected:
        int completedOrderId; //Initialized to NO_ORDER if no order has been completed yet
        int activeOrderId; //Initialized to NO_ORDER if no order is being processed
    
    private:
        const int id;
        const string name;

};


class CollectorVolunteer: public Volunteer {

    public:
        CollectorVolunteer(int id, string name, int coolDown);
        CollectorVolunteer *clone() const override;
        virtual ~CollectorVolunteer()=default;//we added it
        void step() override;
        int getCoolDown() const;
        int getTimeLeft() const;
        bool decreaseCoolDown();//Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
        bool hasOrdersLeft() const override;
        bool canTakeOrder(const Order &order) const override;
        void acceptOrder(const Order &order) override;
        string toString() const override;
    
    private:
        const int coolDown; // The time it takes the volunteer to process an order
        int timeLeft; // Time left until the volunteer finishes his current order
};

class LimitedCollectorVolunteer: public CollectorVolunteer {

    public:
        LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders);
        LimitedCollectorVolunteer *clone() const override;
        ~LimitedCollectorVolunteer() override;//we added it
        bool hasOrdersLeft() const override;
        bool canTakeOrder(const Order &order) const override;
        void acceptOrder(const Order &order) override;
        int getMaxOrders() const;
        int getNumOrdersLeft() const;
        void decreaseOrdersLeft();//we added it
        string toString() const override;
    
    private:
        const int maxOrders; // The number of orders the volunteer can process in the whole simulation
        int ordersLeft; // The number of orders the volunteer can still take
};

class DriverVolunteer: public Volunteer {

    public:
        DriverVolunteer(int id, string name, int maxDistance, int distancePerStep);
        DriverVolunteer *clone() const override;
        virtual ~DriverVolunteer()=default;// we added it
        int getDistanceLeft() const;
        int getMaxDistance() const;
        int getDistancePerStep() const;  
        bool decreaseDistanceLeft(); //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
        bool hasOrdersLeft() const override;
        bool canTakeOrder(const Order &order) const override; // Signal if the volunteer is not busy and the order is within the maxDistance
        void acceptOrder(const Order &order) override; // Assign distanceLeft to order's distance
        void step() override; // Decrease distanceLeft by distancePerStep
        string toString() const override;

    private:
        const int maxDistance; // The maximum distance of ANY order the volunteer can take
        const int distancePerStep; // The distance the volunteer does in one step
        int distanceLeft; // Distance left until the volunteer finishes his current order
};

class LimitedDriverVolunteer: public DriverVolunteer {

    public:
        LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders);
        LimitedDriverVolunteer *clone() const override;
        ~LimitedDriverVolunteer() override;//we added it
        int getMaxOrders() const;
        int getNumOrdersLeft() const;
        bool hasOrdersLeft() const override;
        bool canTakeOrder(const Order &order) const override; // Signal if the volunteer is not busy, the order is within the maxDistance.
        void acceptOrder(const Order &order) override; // Assign distanceLeft to order's distance and decrease ordersLeft
        void decreaseOrdersLeft();//we added it
        string toString() const override;

    private:
        const int maxOrders; // The number of orders the volunteer can process in the whole simulation
        int ordersLeft; // The number of orders the volunteer can still take
};
