#pragma once
#include <list>
#include <mutex>
#include <string>

enum class OrderStatus { PENDING, COMPLETE };

struct ServerOrder {
    int         id;
    int         table;
    std::string product;
    int         quantity;
    OrderStatus status;
};

class OrderStore {
public:
    void add(int table, const std::string& product, int quantity);
    void displayAll() const;
    void displayPending() const;
    bool markComplete(int id);  // retorna false si el id no existe

private:
    mutable std::mutex      mutex_;
    std::list<ServerOrder>  orders_;
    int                     next_id_ = 1;
};
