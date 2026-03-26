#ifndef ORDER_H
#define ORDER_H

#include <string>

/**
 * Represents a single restaurant order entered by a waiter (mesero).
 */
struct Order {
    int table = 0;
    std::string product;
    int quantity = 0;
};

/**
 * Reads table number, product name, and quantity from standard input.
 */
void inputOrder(Order &order);

/**
 * Validates: table > 0, quantity > 0, product non-empty.
 */
bool validateOrder(const Order &order);

/**
 * Serializes order to wire format: table|product|quantity
 */
std::string serializeOrder(const Order &order);

/** Prints order fields for review in the console. */
void displayOrder(const Order &order);

/**
 * Lets the user edit table, product, or quantity, then re-validates.
 */
void modifyOrder(Order &order);

#endif
