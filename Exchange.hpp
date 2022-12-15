#include <map>
#include <string.h>   // strlen 
#include <sstream>      // std::istringstream
#include <sys/types.h>  // senc
#include <sys/socket.h>
#include <cassert> 

#include "Level.hpp"
#include "Client.hpp"
#include "Order.hpp"

#define CMD_HELLO "HELLO"
#define CMD_BUY "BUY"
#define CMD_SELL "SELL"
#define CMD_VIEW "VIEW"
#define CMD_BOOK "BOOK"
#define CMD_CANCEL "CANCEL"

// #define ERR_INVALID_CMD "Invalid command"


// using std::vector;
class Exchange {
	private:
        // const static std::string ERR_INVALID_CMD;

		map<double, Level> bids;
		map<double, Level> asks;
        map<unsigned int, Client> clients;
        map<unsigned int, unsigned int> sdMap;
		map<unsigned int, Order *> orders;
		int orderId;

        void viewExchange(int sd, int size);
		void addOrder(int sd, bool isBuy, int price, int size);
		void cancelOrder(int sd, int orderId);
		void addClient(int sd, int clientId);

	public:
        void parse(void *buffer, int sd);
};
