#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include "Order.hpp"

class Level {
	private:
		unsigned long amount;
        double price;
		Order *head;
		Order *tail;
    public:
        Level(double price);
        void add(Order& order);
        void remove(Order &order);
        std::string viewLevel();
};

#endif