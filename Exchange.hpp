#include <map>

#include "Level.hpp"
#include "Client.hpp"
#include "Order.hpp"

class Exchange {
	private:
		map<double, Level> bids;
		map<double, Level> asks;
		map<unsigned int, Client> clients;
		map<unsigned int, Order *> orders;
		int orderId;

	public:
		char* viewExchange(int size);
		char* addOrder(bool isBuy, int price, int size);
		char* cancelOrder(int clientId, int orderId);
		char* addClient(int clientId);
};
