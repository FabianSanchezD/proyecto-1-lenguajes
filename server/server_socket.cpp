#include "server_socket.h"
#include "producto.h"
#include "config.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

// Parsea mesa producto cantidad y lo guarda
static bool parseAndStore(const std::string& msg, OrderStore& store) {
    std::istringstream ss(msg);
    std::string part;
    std::vector<std::string> parts;

    while (std::getline(ss, part, '|'))
        parts.push_back(part);

    if (parts.size() != 3){
        return false;
    }
    try {
        int table = std::stoi(parts[0]);
        int quantity = std::stoi(parts[2]);

        if (table <= 0 || quantity <= 0 || parts[1].empty()) {
            return false;
        }

        std::string nombre = parts[1];

        store.add(table, nombre, quantity);
        return true;
    } catch (...) {
        return false;
    }
}

// Maneja la conexión de un cliente
static void handleClient(int client_fd, OrderStore& store) {
    std::string buffer;
    char chunk[256];

    while (true) {
        ssize_t n = recv(client_fd, chunk, sizeof(chunk) - 1, 0);
        if (n <= 0) break; // se cerró la conexión
        chunk[n] = '\0';
        buffer += chunk;

        // procesar todos los mensajes completos que hayan llegado
        size_t pos;
        while ((pos = buffer.find('\n')) != std::string::npos) {
            std::string msg = buffer.substr(0, pos);
            buffer.erase(0, pos + 1);

            if (msg.empty()) continue;

            // comandos especiales
            if (msg == "GET_MESAS") {
                int mesas = cargarMesas();
                std::string res = std::to_string(mesas) + "\n";
                send(client_fd, res.c_str(), res.size(), 0);
                continue;
            }

            if (msg == "GET_PRODUCTOS") {
                std::vector<producto> lista;
                cargarProductos(lista);

                std::string res;
                for (size_t i = 0; i < lista.size(); i++) {
                    res += std::to_string(i) + "|" + lista[i].nombre + "\n";
                }

                send(client_fd, res.c_str(), res.size(), 0);
                continue;
            }

            if (parseAndStore(msg, store)) {
                send(client_fd, "OK\n", 3, 0);
            } else {
                std::cerr << "Mensaje invalido ignorado: " << msg << '\n';
                send(client_fd, "ERROR\n", 6, 0);
            }
        }
    }
    close(client_fd);
}

void runServer(OrderStore& store, int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Error al crear socket\n";
        return;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(static_cast<uint16_t>(port));

    if (bind(server_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::cerr << "Error en bind (puerto " << port << ")\n";
        close(server_fd);
        return;
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "Error en listen\n";
        close(server_fd);
        return;
    }

    std::cout << "Escuchando en puerto " << port << "...\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t   client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);

        if (client_fd < 0) {
            std::cerr << "Error en accept\n";
            continue;
        }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
        //std::cout << "Cliente conectado: " << ip << '\n'

        // un hilo por cliente para manejar conexiones en paralelo
        std::thread(handleClient, client_fd, std::ref(store)).detach();
    }

    close(server_fd);
}
