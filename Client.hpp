#ifndef CLIENT_H
#define CLIENT_H

#include "Order.hpp"
#include <vector>
#include <map>

using std::map;
class Client {

	private:
        static constexpr double INIT_BALANCE = 10000;
        std::string name;
        bool open;
        int sd;
        double balance;
        map<unsigned int, Order> positions;

    public:
        Client(std::string name, int sd);
        bool isOpen();
        void close();
        void updateSd(int sd);
        void viewPositions();
        std::string viewClient();
};

#endif
