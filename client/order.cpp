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
    std::cout << "Table number: ";
    if (!(std::cin >> order.table)) {
        clearInputLine();
        return;
    }
    clearInputLine();

    std::cout << "Product name: ";
    std::getline(std::cin, order.product);

    std::cout << "Quantity: ";
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
    std::cout << "  Table: " << order.table << '\n';
    std::cout << "  Product: " << order.product << '\n';
    std::cout << "  Quantity: " << order.quantity << '\n';
}

void modifyOrder(Order &order) {
    while (true) {
        std::cout << "\n--- Modify order ---\n";
        displayOrder(order);
        std::cout << "1. Change table\n";
        std::cout << "2. Change product\n";
        std::cout << "3. Change quantity\n";
        std::cout << "4. Done\n";
        std::cout << "Choice: ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInputLine();
            std::cout << "Invalid input.\n";
            continue;
        }
        clearInputLine();

        if (choice == 1) {
            std::cout << "New table number: ";
            if (!(std::cin >> order.table)) {
                clearInputLine();
                std::cout << "Invalid number.\n";
                continue;
            }
            clearInputLine();
        } else if (choice == 2) {
            std::cout << "New product name: ";
            std::getline(std::cin, order.product);
        } else if (choice == 3) {
            std::cout << "New quantity: ";
            if (!(std::cin >> order.quantity)) {
                clearInputLine();
                std::cout << "Invalid number.\n";
                continue;
            }
            clearInputLine();
        } else if (choice == 4) {
            break;
        } else {
            std::cout << "Invalid choice.\n";
            continue;
        }

        if (!validateOrder(order)) {
            std::cout << "Validation failed: table and quantity must be > 0, "
                         "product must not be empty.\n";
        }
    }
}
