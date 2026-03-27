#include "order.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace {

void clearInputLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

} // namespace

void inputOrder(Order &order, const std::vector<std::string>& productos) {
    std::cout << "Número de mesa: ";
    if (!(std::cin >> order.table)) {
        clearInputLine();
        return;
    }
    clearInputLine();

    // mostrar productos disponibles
    std::cout << "\nID. Nombre del platillo\n";
    for (size_t i = 0; i < productos.size(); i++) {
        std::cout << i << ". " << productos[i] << "\n";
    }

    // pedir ID
    std::cout << "ID del producto: ";
    if (!(std::cin >> order.productId)) {
        clearInputLine();
        return;
    }
    clearInputLine();

    std::cout << "Cantidad: ";
    if (!(std::cin >> order.quantity)) {
        clearInputLine();
        return;
    }
    clearInputLine();
}

bool validateOrder(const Order &order, int maxMesas, const std::vector<std::string>& productos) {
    if (order.table <= 0 || order.table > maxMesas)
        return false;

    if (order.quantity <= 0)
        return false;

    if (order.productId < 0 || order.productId >= productos.size())
        return false;

    return true;
}


std::string serializeOrder(const Order &order, const std::vector<std::string>& productos) {
    std::ostringstream out;

    std::string nombreProducto = "[desconocido]";
    if (order.productId >= 0 && order.productId < productos.size()) {
        nombreProducto = productos[order.productId];
    }

    out << order.table << '|' << nombreProducto << '|' << order.quantity;
    return out.str();
}

void displayOrder(const Order &order, const std::vector<std::string>& productos) {
    std::cout << "  Mesa: " << order.table << '\n';

    if (order.productId >= 0 && order.productId < productos.size()) {
        std::cout << "  Producto: " << productos[order.productId] << '\n';
    } else {
        std::cout << "  Producto: [ID inválido]\n";
    }

    std::cout << "  Cantidad: " << order.quantity << '\n';
}

void modifyOrder(Order &order, int maxMesas, const std::vector<std::string>& productos) {
    while (true) {
        std::cout << "\n--- Modificar pedido ---\n";
        displayOrder(order, productos);

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
            // mostrar productos
            std::cout << "\nID. Nombre del platillo\n";
            for (size_t i = 0; i < productos.size(); i++) {
                std::cout << i << ". " << productos[i] << "\n";
            }

            std::cout << "ID del nuevo producto: ";
            int id;
            if (!(std::cin >> id)) {
                clearInputLine();
                return;
            }
            clearInputLine();
            if (id >= 0 && id < productos.size()) {
                order.productId = id;
            } else {
                std::cout << "ID inválido\n";
            }
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

        if (!validateOrder(order, maxMesas, productos)) {
            std::cout << "Pedido no válido:\n";
            std::cout << "- La mesa debe existir (1 a " << maxMesas << ")\n";
            std::cout << "- El producto debe existir\n";
            std::cout << "- La cantidad debe ser mayor a 0\n";
        } else {
            std::cout << "Pedido actualizado correctamente.\n";
        }
    }
}