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
    std::cout << "\n=== Restaurant Order System ===\n";
    std::cout << "1. Create order\n";
    std::cout << "2. Modify order\n";
    std::cout << "3. Send order\n";
    std::cout << "4. Exit\n";
    std::cout << "Choice: ";
}

} // namespace

int main() {
    std::vector<Order> pending_orders;

    while (true) {
        printMenu();

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInputLine();
            std::cout << "Invalid input. Try again.\n";
            continue;
        }
        clearInputLine();

        if (choice == 1) {
            Order o{};
            inputOrder(o);
            if (!validateOrder(o)) {
                std::cout << "Invalid order: table and quantity must be > 0, "
                             "product must not be empty.\n";
                continue;
            }
            pending_orders.push_back(o);
            std::cout << "Order added to pending list.\n";
        } else if (choice == 2) {
            if (pending_orders.empty()) {
                std::cout << "No pending orders to modify.\n";
                continue;
            }
            std::cout << "Pending orders:\n";
            for (std::size_t i = 0; i < pending_orders.size(); ++i) {
                std::cout << "[" << i << "]\n";
                displayOrder(pending_orders[i]);
                std::cout << '\n';
            }
            std::cout << "Index to modify: ";
            std::size_t idx = 0;
            if (!(std::cin >> idx)) {
                clearInputLine();
                std::cout << "Invalid index.\n";
                continue;
            }
            clearInputLine();
            if (idx >= pending_orders.size()) {
                std::cout << "Invalid index.\n";
                continue;
            }
            modifyOrder(pending_orders[idx]);
            if (!validateOrder(pending_orders[idx])) {
                std::cout << "Warning: this order is still invalid until fixed.\n";
            }
        } else if (choice == 3) {
            if (pending_orders.empty()) {
                std::cout << "No pending orders to send.\n";
                continue;
            }
            std::cout << "Pending orders:\n";
            for (std::size_t i = 0; i < pending_orders.size(); ++i) {
                std::cout << "[" << i << "]\n";
                displayOrder(pending_orders[i]);
                std::cout << '\n';
            }
            std::cout << "Index to send: ";
            std::size_t idx = 0;
            if (!(std::cin >> idx)) {
                clearInputLine();
                std::cout << "Invalid index.\n";
                continue;
            }
            clearInputLine();
            if (idx >= pending_orders.size()) {
                std::cout << "Invalid index.\n";
                continue;
            }
            if (!validateOrder(pending_orders[idx])) {
                std::cout << "Cannot send: order is invalid. Modify it first.\n";
                continue;
            }
            const std::string payload = serializeOrder(pending_orders[idx]) + '\n';
            sendOrderAsync(payload);
            pending_orders.erase(pending_orders.begin() +
                                 static_cast<std::ptrdiff_t>(idx));
            std::cout << "Order queued for sending (async).\n";
        } else if (choice == 4) {
            std::cout << "Goodbye.\n";
            break;
        } else {
            std::cout << "Invalid choice. Enter 1–4.\n";
        }
    }

    waitForPendingSends();
    return 0;
}
