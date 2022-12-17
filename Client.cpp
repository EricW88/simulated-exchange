#include "Client.hpp"

Client::Client(std::string name, int sd) : name(name), open(true), sd(sd), balance(INIT_BALANCE) {}

bool Client::isOpen() {
    return open;
}

void Client::close() {
    open = false;
}

void Client::updateSd(int sd) {
    this->sd = sd;
    open = true;
}

void Client::viewPositions() {
    return;
}

std::string Client::viewClient() {
    std::string balance_str = std::to_string(balance);
    balance_str.erase(balance_str.length() - 4);
    std::string result = name + "\nBALANCE: " + std::to_string(balance) + "\nPOSITIONS: ";
    for(auto it = positions.begin(); it != positions.end(); ++it) {
        result += it->second.viewOrder() + ", ";
    }
    result.erase(result.length() - 2);
    result += "\n";
    return result;
}