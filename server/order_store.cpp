#include "order_store.h"
#include <iostream>
#include <iomanip>

static const char* statusStr(OrderStatus s) {
    return s == OrderStatus::PENDING ? "Pendiente" : "Completa";
}

static void printHeader() {
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(8)  << "Mesa"
              << std::setw(20) << "Producto"
              << std::setw(10) << "Cantidad"
              << "Estado" << '\n';
    std::cout << std::string(54, '-') << '\n';
}

static void printOrder(const ServerOrder& o) {
    std::cout << std::left
              << std::setw(6)  << o.id
              << std::setw(8)  << o.table
              << std::setw(20) << o.product
              << std::setw(10) << o.quantity
              << statusStr(o.status) << '\n';
}

void OrderStore::add(int table, const std::string& product, int quantity) {
    std::lock_guard<std::mutex> lock(mutex_);
    orders_.push_back({next_id_++, table, product, quantity, OrderStatus::PENDING});
    std::cout << "[Servidor] Nueva orden recibida (ID "
              << orders_.back().id << "): Mesa " << table
              << " — " << product << " x" << quantity << '\n';
}

void OrderStore::displayAll() const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (orders_.empty()) {
        std::cout << "No hay ordenes registradas.\n";
        return;
    }
    printHeader();
    for (const auto& o : orders_) printOrder(o);
}

void OrderStore::displayPending() const {
    std::lock_guard<std::mutex> lock(mutex_);
    bool found = false;
    for (const auto& o : orders_) {
        if (o.status == OrderStatus::PENDING) {
            if (!found) {
                printHeader(); found = true;
            }
            printOrder(o);
        }
    }

    if (!found) {
        std::cout << "No hay ordenes pendientes.\n";
    }
}

bool OrderStore::markComplete(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& o : orders_) {
        if (o.id == id) {
            o.status = OrderStatus::COMPLETE;
            return true;
        }
    }
    return false;
}
