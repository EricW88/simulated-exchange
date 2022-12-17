#include "Exchange.hpp"

void Exchange::viewExchange(int sd) {
    bids.insert(std::make_pair(1.9, Level(1.9)));
    asks.insert(std::make_pair(2.1, Level(2.1)));


    if(bids.empty() || asks.empty()) {
        send(sd, MSG_BOOK_EMPTY, strlen(MSG_BOOK_EMPTY), 0);
        return;
    }

    double mid = (bids.rbegin()->first + asks.begin()->first) / 2.0;
    double upper = mid * (1 + SNAPSHOT_CUTOFF);
    double lower = mid * (1 - SNAPSHOT_CUTOFF);
    std::string strMsg = "\nASKS:\n";

    // concatenate asks in descending order
    auto askIt = asks.begin();
    while(askIt != asks.end() && askIt->first <= upper) {
        ++askIt;
    }
    --askIt;
    for(; askIt != asks.begin(); --askIt) {
        strMsg += askIt->second.viewLevel();
    }
    if(askIt->first <= upper) strMsg += askIt->second.viewLevel();
    strMsg += "\nBIDS:\n";

    // concatenate bids in increasing order
    auto bidIt = bids.rbegin();
    while(bidIt != bids.rend() && bidIt->first >= lower) {
        strMsg += bidIt->second.viewLevel();
        ++bidIt;
    }
    strMsg += "\n";

    char *viewMsg = &strMsg[0];
    send(sd, viewMsg, strlen(viewMsg), 0);
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
        invalidInput = std::getline(input, cur, ' ') ? true : false;
        if(!invalidInput) viewExchange(sd);
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