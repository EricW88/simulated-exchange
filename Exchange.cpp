#include "Exchange.hpp"

void Exchange::viewExchange(int sd, int size) {
    send(sd, "viewExchange\n\n", strlen("viewExchange\n\n"), 0);
    return;
}

void Exchange::addOrder(int sd, bool isBuy, int price, int size) {
    send(sd, "addOrder\n\n", strlen("addOrder\n\n"), 0);
    return;
}

void Exchange::cancelOrder(int sd, int orderId) {
    send(sd, "cancelOrder\n\n", strlen("cancelOrder\n\n"), 0);
    return;
}

void Exchange::addClient(int sd, std::string clientName) {
    char *addMsg;
    
    if(clients.find(clientName) != clients.end() && clients.find(clientName)->second.isOpen()) {
        std::cout << "test1" << std::endl;
        addMsg = ERR_CLIENT_CONNECTED;
    } else {
        std::cout << "test2" << std::endl;
        sdMap[sd] = clientName;
        if(clients.find(clientName) != clients.end()) {
            clients.find(clientName)->second.updateSd(sd);
        } else {
            clients.insert(make_pair(clientName, Client(clientName, sd)));
        }
        std::string strMsg = "HELLO " + clients.find(clientName)->second.viewClient() + "\n";
        addMsg = &strMsg[0];
    }

    send(sd, addMsg, strlen(addMsg), 0);
    return;
}

void Exchange::parse(void *buffer, int sd) {
    std::string line(static_cast<char *>(buffer));
    std::istringstream input(line);
    std::string cur;
    std::getline(input, cur, ' ');
    const char *errMessage = ERR_INPUT;

    bool invalidInput = true;
    if(cur == "HELLO") {
        invalidInput = !std::getline(input, cur, ' ') || std::getline(input, cur, ' ');
        if(!invalidInput) addClient(sd, cur);
    } else if(cur == "BUY" || cur == "SELL") {
        int price;
        int size;
        if(std::getline(input, cur, ' ')) {
            size_t idx;
            price = stoi(cur, &idx);
            invalidInput = idx != cur.length() || !std::getline(input, cur, ' ');
            if(!invalidInput) {
                size = stoi(cur, &idx);
                invalidInput = idx != cur.length() || std::getline(input, cur, ' ');
            }
        }
        if(!invalidInput) addOrder(sd, cur == "BUY", price, size);
    } else if(cur == "VIEW") {
        invalidInput = std::getline(input, cur, ' ') ? true : false;
        if(!invalidInput) { 
            Client c = clients.find(sdMap[sd])->second;
            c.viewPositions();
        }
    } else if(cur == "BOOK") {
        int size;
        if(std::getline(input, cur, ' ')) {
            size_t idx;
            size = stoi(cur, &idx);
            invalidInput = idx != cur.length() || std::getline(input, cur, ' ');
        }
        if(!invalidInput) viewExchange(sd, size);
    } else if(cur == "CANCEL") {
        int id;
        if(std::getline(input, cur, ' ')) {
            size_t idx;
            id = stoi(cur, &idx);
            invalidInput = idx != cur.length() || std::getline(input, cur, ' ');
        }
        if(!invalidInput) cancelOrder(sd, id);        
    }

    if(invalidInput) {
        send(sd, errMessage, strlen(errMessage), 0);
    }
    return;
}

void Exchange::close(int sd) {
    clients.find(sdMap[sd])->second.close();
    sdMap.erase(sd);
    return;
}