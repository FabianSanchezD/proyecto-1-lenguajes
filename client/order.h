#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>

/**
 * Represents a single restaurant order entered by a waiter (mesero).
 */
struct Order {
    int table = 0;
    std::string product;
    int productId;
    int quantity = 0;
};


/**
 * Reads table number, product name, and quantity from standard input.
 */
void inputOrder(Order &order, const std::vector<std::string>& productos);

/**
 * Validates: table > 0 && table <= maxMesas, quantity > 0, product non-empty.
 */
bool validateOrder(const Order &order, int maxMesas, const std::vector<std::string>& productos);

/**
 * Serializes order to wire format: table|product|quantity
 */
std::string serializeOrder(const Order &order, const std::vector<std::string>& productos);

/** Prints order fields for review in the console. */
void displayOrder(const Order &order, const std::vector<std::string>& productos);

/**
 * Lets the user edit table, product, or quantity, then re-validates.
 */
void modifyOrder(Order &order, int maxMesas, const std::vector<std::string>& productos);

#endif
