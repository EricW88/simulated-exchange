#include "Level.hpp"

Level::Level(double price) : amount(0), price(price), head(NULL), tail(NULL) {}

void Level::add(Order &order) {
    return;
}

void Level::remove(Order &order) {
    return;
}

std::string Level::viewLevel() {
    std::string price_str = std::to_string(price);
    price_str.erase(price_str.length() - 4);
    return price_str + " " + std::to_string(amount);
}