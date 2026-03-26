#include "order.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <string>

namespace {

void clearInputLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

} // namespace

void inputOrder(Order &order) {
    std::cout << "Número de mesa: ";
    if (!(std::cin >> order.table)) {
        clearInputLine();
        return;
    }
    clearInputLine();

    std::cout << "Nombre del producto: ";
    std::getline(std::cin, order.product);

    std::cout << "Cantidad: ";
    if (!(std::cin >> order.quantity)) {
        clearInputLine();
        return;
    }
    clearInputLine();
}

bool validateOrder(const Order &order) {
    if (order.table <= 0) {
        return false;
    }
    if (order.quantity <= 0) {
        return false;
    }
    if (order.product.empty()) {
        return false;
    }
    return true;
}

std::string serializeOrder(const Order &order) {
    std::ostringstream out;
    out << order.table << '|' << order.product << '|' << order.quantity;
    return out.str();
}

void displayOrder(const Order &order) {
    std::cout << "  Mesa: " << order.table << '\n';
    std::cout << "  Producto: " << order.product << '\n';
    std::cout << "  Cantidad: " << order.quantity << '\n';
}

void modifyOrder(Order &order) {
    while (true) {
        std::cout << "\n--- Modificar pedido ---\n";
        displayOrder(order);
        std::cout << "1. Cambiar mesa\n";
        std::cout << "2. Cambiar producto\n";
        std::cout << "3. Cambiar cantidad\n";
        std::cout << "4. Terminar\n";
        std::cout << "Opción: ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInputLine();
            std::cout << "Entrada no válida.\n";
            continue;
        }
        clearInputLine();

        if (choice == 1) {
            std::cout << "Nuevo número de mesa: ";
            if (!(std::cin >> order.table)) {
                clearInputLine();
                std::cout << "Número no válido.\n";
                continue;
            }
            clearInputLine();
        } else if (choice == 2) {
            std::cout << "Nuevo nombre del producto: ";
            std::getline(std::cin, order.product);
        } else if (choice == 3) {
            std::cout << "Nueva cantidad: ";
            if (!(std::cin >> order.quantity)) {
                clearInputLine();
                std::cout << "Número no válido.\n";
                continue;
            }
            clearInputLine();
        } else if (choice == 4) {
            break;
        } else {
            std::cout << "Opción no válida.\n";
            continue;
        }

        if (!validateOrder(order)) {
            std::cout << "Validación incorrecta: la mesa y la cantidad deben ser "
                         "mayores que 0 y el producto no puede estar vacío.\n";
        }
    }
}
