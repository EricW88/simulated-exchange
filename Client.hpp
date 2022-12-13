#ifndef CLIENT_H
#define CLIENT_H

#include "Order.hpp"
#include <vector>

using std::map;
class Client {

	private:
        int id;
        int fd;
        int balance;
        map<unsigned int, Order> positions;

    public:
        std::string viewClient();
};

#endif
