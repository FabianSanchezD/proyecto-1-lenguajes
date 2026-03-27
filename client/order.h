#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>

/**
 * Representa un pedido individual del restaurante ingresado por un mesero
 */
struct Order {
    int table = 0;
    std::string product;
    int productId;
    int quantity = 0;
};


/**
 * Lee el número de mesa, nombre del producto y cantidad desde la entrada
 */
void inputOrder(Order &order, const std::vector<std::string>& productos);

/**
 * Valida: mesa > 0 && mesa <= maxMesas, cantidad > 0
 */
bool validateOrder(const Order &order, int maxMesas, const std::vector<std::string>& productos);

/**
 * Serializa el pedido al formato de red: mesa|producto|cantidad
 */
std::string serializeOrder(const Order &order, const std::vector<std::string>& productos);

/** Imprime los campos del pedido para revisión en la consola */
void displayOrder(const Order &order, const std::vector<std::string>& productos);

/**
 * Permite al usuario editar mesa, producto o cantidad, luego revalid
 */
void modifyOrder(Order &order, int maxMesas, const std::vector<std::string>& productos);

#endif
