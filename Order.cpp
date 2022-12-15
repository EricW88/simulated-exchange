#include "Order.hpp"

std::string Order::viewOrder() {
    std::string result = "{" + std::to_string(orderId) + ", ";
    result += isBuy ? "BUY, " : "SELL, ";
    std::string price_str = std::to_string(price);
    // truncate to 2 decimal places
    price_str.erase(price_str.length() - 4);
    result += price_str + ", " + std::to_string(quantity) + "}";
    return result;
}