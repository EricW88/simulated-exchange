#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include "Order.hpp"

class Level {
	private:
		unsigned long amount;
		Order *head;
		Order *tail;
    public:
        std::string viewLevel();
};

#endif