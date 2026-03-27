#include "order.h"
#include "socket_client.h"

#include <cstddef>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace {

void clearInputLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printMenu() {
    std::cout << "\n=== Sistema de pedidos del restaurante ===\n";
    std::cout << "1. Crear pedido\n";
    std::cout << "2. Modificar pedido\n";
    std::cout << "3. Enviar pedido\n";
    std::cout << "4. Salir\n";
    std::cout << "Opción: ";
}

} // namespace

int main() {
    std::vector<Order> pending_orders;
    int maxMesas = getMesasFromServer();
    std::vector<std::string> productos = getProductosFromServer();

    while (true) {
        printMenu();

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInputLine();
            std::cout << "Entrada no válida. Intenta de nuevo.\n";
            continue;
        }
        clearInputLine();

        if (choice == 1) {
            Order o{};
            maxMesas = getMesasFromServer();
            productos = getProductosFromServer();
            inputOrder(o, productos);
            if (!validateOrder(o, maxMesas, productos)) {
                std::cout << "Pedido no válido:\n";
                std::cout << "- La mesa debe existir (1 a " << maxMesas << ")\n";
                std::cout << "- El producto debe existir\n";
                std::cout << "- La cantidad debe ser mayor a 0\n";
                continue;
            }
            pending_orders.push_back(o);
            std::cout << "Pedido añadido a la lista pendiente.\n";
        } else if (choice == 2) {
            if (pending_orders.empty()) {
                std::cout << "No hay pedidos pendientes para modificar.\n";
                continue;
            }
            std::cout << "Pedidos pendientes:\n";
            for (std::size_t i = 0; i < pending_orders.size(); ++i) {
                std::cout << "[" << i << "]\n";
                displayOrder(pending_orders[i], productos);
                std::cout << '\n';
            }
            std::cout << "Índice a modificar: ";
            std::size_t idx = 0;
            if (!(std::cin >> idx)) {
                clearInputLine();
                std::cout << "Índice no válido.\n";
                continue;
            }
            clearInputLine();
            if (idx >= pending_orders.size()) {
                std::cout << "Índice no válido.\n";
                continue;
            }
            maxMesas = getMesasFromServer();
            productos = getProductosFromServer();
            modifyOrder(pending_orders[idx], maxMesas, productos);
            if (!validateOrder(pending_orders[idx], maxMesas, productos)) {
                std::cout << "Atención: este pedido sigue siendo inválido hasta "
                             "que lo corrijas.\n";
            }
        } else if (choice == 3) {
            if (pending_orders.empty()) {
                std::cout << "No hay pedidos pendientes para enviar.\n";
                continue;
            }
            std::cout << "Pedidos pendientes:\n";
            for (std::size_t i = 0; i < pending_orders.size(); ++i) {
                std::cout << "[" << i << "]\n";
                displayOrder(pending_orders[i], productos);
                std::cout << '\n';
            }
            std::cout << "Índice a enviar: ";
            std::size_t idx = 0;
            if (!(std::cin >> idx)) {
                clearInputLine();
                std::cout << "Índice no válido.\n";
                continue;
            }
            clearInputLine();
            if (idx >= pending_orders.size()) {
                std::cout << "Índice no válido.\n";
                continue;
            }
            if (!validateOrder(pending_orders[idx], maxMesas, productos)) {
                std::cout << "No se puede enviar: el pedido no es válido. "
                             "Modifícalo primero.\n";
                continue;
            }
            const std::string payload = serializeOrder(pending_orders[idx], productos) + '\n';
            sendOrderAsync(payload);
            pending_orders.erase(pending_orders.begin() +
                                 static_cast<std::ptrdiff_t>(idx));
            std::cout << "Pedido encolado para envío (asíncrono).\n";
        } else if (choice == 4) {
            std::cout << "Hasta luego.\n";
            break;
        } else {
            std::cout << "Opción no válida. Elige un número del 1 al 4.\n";
        }
    }

    waitForPendingSends();
    return 0;
}
