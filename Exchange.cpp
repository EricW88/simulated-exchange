#include "Exchange.hpp"

void Exchange::viewExchange(int sd, int size) {
    send(sd, "viewExchange\n", strlen("viewExchange\n"), 0);
    return;
}

void Exchange::addOrder(int sd, bool isBuy, int price, int size) {
    send(sd, "addOrder\n", strlen("addOrder\n"), 0);
    return;
}

void Exchange::cancelOrder(int sd, int orderId) {
    send(sd, "cancelOrder\n", strlen("cancelOrder\n"), 0);
    return;
}

void Exchange::addClient(int sd, int clientId) {
    send(sd, "addClient\n", strlen("addClient\n"), 0);
    return;
}

void Exchange::parse(void *buffer, int sd) {
    std::string line(static_cast<char *>(buffer));
    std::istringstream input(line);
    std::string cur;
    std::getline(input, cur, ' ');
    const char *errMessage = "Invalid input\n";

    bool invalidInput = true;
    if(cur == "HELLO") {
        int id;
        if(std::getline(input, cur, ' ')) {
            size_t idx;
            id = stoi(cur, &idx);
            invalidInput = idx != cur.length() || std::getline(input, cur, ' ');

        }
        // std::cout << invalidInput << std::endl;
        if(!invalidInput) addClient(sd, id);
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
            Client c = clients[sdMap[sd]];
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