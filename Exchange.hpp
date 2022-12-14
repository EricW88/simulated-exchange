#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string.h>         // strlen 
#include <sstream>          // std::istringstream
#include <sys/types.h>      // senc
#include <sys/socket.h>
#include <cassert> 
#include <errno.h> 



#include "Level.hpp"
#include "Client.hpp"
#include "Order.hpp"

#define CMD_HELLO "HELLO"
#define CMD_BUY "BUY"
#define CMD_SELL "SELL"
#define CMD_VIEW "VIEW"
#define CMD_BOOK "BOOK"
#define CMD_CANCEL "CANCEL"

#define MSG_REJECT "REJECT"
#define MSG_BOOK_EMPTY "\nOrderbook is empty\n"

#define ERR_INPUT "\nInvalid input\n"
#define ERR_NOT_CONNECTED "\nNot connected to exchange\n"
#define ERR_SD_TAKEN "\nReason: Already connected to exchange\n"
#define ERR_CLIENT_CONNECTED "\nReason: Client is already connected\n"


using std::unordered_map;
using std::unordered_set;
class Exchange {
	private:
        // const static char* ERR_INVALID_CMD;
        static constexpr double SNAPSHOT_CUTOFF = .2;

		map<double, Level> bids;
		map<double, Level> asks;
        unordered_map<std::string, Client> clients;
        unordered_map<unsigned int, std::string> sdMap;
		map<unsigned int, Order *> orders;
		int orderId;

        void viewExchange(int sd);
		void addOrder(int sd, bool isBuy, int price, int size);
		void cancelOrder(int sd, int orderId);
		void addClient(int sd, std::string clientId);

	public:
        void parse(void *buffer, int sd);
        void close(int sd);
};
