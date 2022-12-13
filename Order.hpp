#ifndef ORDER_H
#define ORDER_H

#include <iostream>

class Order {

    private:
        bool isBuy;
        unsigned int quantity;
        double price;
        unsigned int clientId;
        unsigned int orderId;
        Order *prev;
        Order *next;

    public:
        std::string viewOrder();
};

#endif