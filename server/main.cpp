#include "order_store.h"
#include "server_socket.h"
#include "config.h"

#include <iostream>
#include <limits>
#include <thread>
#include <vector>


static void clearInputLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void adminMenu(OrderStore& store) {
    vector<producto> lista;
    cargarProductos(lista);
    int cantidadMesas = cargarMesas();
    int option = 0;
    while (true) {
        std::cout << "\n=== Menu Administrador ===\n"
                  << "1. Ver todas las ordenes\n"
                  << "2. Ver ordenes pendientes\n"
                  << "3. Marcar orden como completada\n"
                  << "4. Configuracion\n"
                  << "5. Salir\n"
                  << "Opcion: ";

        if (!(std::cin >> option)) {
            clearInputLine();
            std::cout << "Entrada invalida.\n";
            continue;
        }
        clearInputLine();

        switch (option) {
        case 1:
            store.displayAll();
            break;
        case 2:
            store.displayPending();
            break;
        case 3: {
            store.displayPending();
            std::cout << "ID de la orden a completar: ";
            int id;
            if (!(std::cin >> id)) {
                clearInputLine();
                std::cout << "ID invalido.\n";
                break;
            }
            clearInputLine();
            if (store.markComplete(id))
                std::cout << "Orden " << id << " marcada como completada.\n";
            else
                std::cout << "No se encontro la orden con ID " << id << ".\n";
            break;
        }
        case 4:
            menuConfiguracion(lista);
            break;
        case 5:
            std::cout << "Cerrando servidor...\n";
            return;
        default:
            std::cout << "Opcion no valida.\n";
        }
    }
}

int main() {
    const int kPort = 8080; // está hardcodeado
    OrderStore store;

    std::thread server_thread(runServer, std::ref(store), kPort);
    server_thread.detach();

    adminMenu(store);
    return 0;
}
