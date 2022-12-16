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
    std::string strMsg;
    if(sdMap.find(sd) != sdMap.end()) {
        strMsg = std::string("\n") + MSG_REJECT + std::string(" ") + CMD_HELLO + ERR_SD_TAKEN;
    } else if(clients.find(clientName) != clients.end() && clients.find(clientName)->second.isOpen()) {
        strMsg = std::string("\n") + MSG_REJECT + std::string(" ") + CMD_HELLO + ERR_CLIENT_CONNECTED;
    } else {
        sdMap[sd] = clientName;
        if(clients.find(clientName) != clients.end()) {
            clients.find(clientName)->second.updateSd(sd);
        } else {
            clients.insert(make_pair(clientName, Client(clientName, sd)));
        }
        strMsg = std::string("\n") + CMD_HELLO + std::string(" ") + clients.find(clientName)->second.viewClient() + "\n";
    }
    char *addMsg = &strMsg[0];
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
    if(cur == CMD_HELLO) {
        invalidInput = !std::getline(input, cur, ' ') || std::getline(input, cur, ' ');
        if(!invalidInput) addClient(sd, cur);
    } else if(cur == CMD_BUY || cur == CMD_SELL) {
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
        if(!invalidInput) addOrder(sd, cur == CMD_BUY, price, size);
    } else if(cur == CMD_VIEW) {
        invalidInput = std::getline(input, cur, ' ') ? true : false;
        if(!invalidInput) { 
            Client c = clients.find(sdMap[sd])->second;
            c.viewPositions();
        }
    } else if(cur == CMD_BOOK) {
        int size;
        if(std::getline(input, cur, ' ')) {
            size_t idx;
            size = stoi(cur, &idx);
            invalidInput = idx != cur.length() || std::getline(input, cur, ' ');
        }
        if(!invalidInput) viewExchange(sd, size);
    } else if(cur == CMD_CANCEL) {
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
    if(sdMap.find(sd) == sdMap.end()) return;
    clients.find(sdMap[sd])->second.close();
    sdMap.erase(sd);
}