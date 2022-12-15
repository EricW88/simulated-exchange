#ifndef CLIENT_H
#define CLIENT_H

#include "Order.hpp"
#include <vector>
#include <map>

using std::map;
class Client {

	private:
        // int id;
        int sd;
        int balance;
        map<unsigned int, Order> positions;

    public:
        void viewPositions();
        void viewClient();
};

#endif
