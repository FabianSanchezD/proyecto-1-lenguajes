#include "socket_client.h"

#include <arpa/inet.h>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <sstream>

namespace {

std::mutex g_socket_log_mutex;
std::atomic<int> g_pending_async_sends{0};

void socketLog(const char *msg) {
    std::lock_guard<std::mutex> lock(g_socket_log_mutex);
    std::cout << msg << std::flush;
}

void socketPerror(const char *s) {
    std::lock_guard<std::mutex> lock(g_socket_log_mutex);
    std::perror(s);
}

} // namespace

void sendOrder(const std::string &serialized) {
    // Crear socket TCP IPv4 (SOCK_STREAM).
    const int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        socketPerror("socket");
        socketLog("No se pudo conectar\n");
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(kServerPort));

    if (inet_pton(AF_INET, kServerHost, &addr.sin_addr) != 1) {
        {
            std::lock_guard<std::mutex> lock(g_socket_log_mutex);
            std::cerr << "Dirección no válida: " << kServerHost << '\n';
        }
        close(sock_fd);
        socketLog("No se pudo conectar\n");
        return;
    }

    // Intentar conexión TCP al servidor del restaurante.
    if (connect(sock_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        socketPerror("connect");
        close(sock_fd);
        socketLog("No se pudo conectar\n");
        return;
    }

    socketLog("Conectado al servidor\n");

    // send() puede escribir solo parte del buffer; iterar hasta enviar todos los bytes.
    const char *data = serialized.c_str();
    size_t remaining = serialized.size();
    while (remaining > 0) {
        const ssize_t sent = send(sock_fd, data, remaining, 0);
        if (sent < 0) {
            socketPerror("send");
            close(sock_fd);
            socketLog("No se pudo enviar el pedido\n");
            return;
        }
        if (sent == 0) {
            socketLog("No se pudo enviar el pedido\n");
            close(sock_fd);
            return;
        }
        data += static_cast<size_t>(sent);
        remaining -= static_cast<size_t>(sent);
    }

    close(sock_fd);
    socketLog("Pedido enviado correctamente\n");
}

void sendOrderAsync(const std::string &serialized) {
    // Contar antes del detach para que waitForPendingSends() no pierda un envío en curso.
    g_pending_async_sends.fetch_add(1, std::memory_order_acq_rel);
    std::thread([serialized]() {
        struct AsyncSendGuard {
            ~AsyncSendGuard() {
                g_pending_async_sends.fetch_sub(1, std::memory_order_acq_rel);
            }
        } guard;
        // Hilo separado: el menú sigue respondiendo; el guard siempre decrementa al salir.
        sendOrder(serialized);
    }).detach();
}

void waitForPendingSends() {
    if (g_pending_async_sends.load(std::memory_order_acquire) == 0) {
        return;
    }
    socketLog("Esperando a que terminen los envíos pendientes...\n");
    while (g_pending_async_sends.load(std::memory_order_acquire) > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int getMesasFromServer() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) return 0;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(kServerPort));

    if (inet_pton(AF_INET, kServerHost, &addr.sin_addr) != 1) {
        close(sock_fd);
        return 0;
    }

    if (connect(sock_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        close(sock_fd);
        return 0;
    }

    std::string msg = "GET_MESAS\n";
    send(sock_fd, msg.c_str(), msg.size(), 0);

    char buffer[64];
    ssize_t n = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);
    close(sock_fd);

    if (n <= 0) return 0;

    buffer[n] = '\0';
    return std::stoi(buffer);
}

std::vector<std::string> getProductosFromServer() {
    std::vector<std::string> productos;

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) return productos;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(kServerPort));

    if (inet_pton(AF_INET, kServerHost, &addr.sin_addr) != 1) {
        close(sock_fd);
        return productos;
    }

    if (connect(sock_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        close(sock_fd);
        return productos;
    }

    // pedir productos
    std::string msg = "GET_PRODUCTOS\n";
    send(sock_fd, msg.c_str(), msg.size(), 0);

    char buffer[512];
    ssize_t n = recv(sock_fd, buffer, sizeof(buffer) - 1, 0);

    if (n <= 0) {
        close(sock_fd);
        return productos;
    }

    buffer[n] = '\0';
    std::string total(buffer);

    close(sock_fd);

    // parsear
    std::istringstream ss(total);
    std::string linea;

    while (std::getline(ss, linea)) {
        if (linea.empty()) continue;

        std::istringstream lineaStream(linea);
        std::string idStr, nombre;

        if (std::getline(lineaStream, idStr, '|') &&
            std::getline(lineaStream, nombre)) {

            productos.push_back(nombre);
        }
    }

    return productos;
}