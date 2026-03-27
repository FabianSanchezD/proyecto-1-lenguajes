#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <string>
#include <vector>

/** Server address used by the waiter client (matches typical local dev setup). */
constexpr const char *kServerHost = "127.0.0.1";
constexpr int kServerPort = 8080;

/**
 * Opens a TCP connection, sends one serialized order string, closes the socket.
 * Uses Linux/Berkeley sockets: socket(), connect(), send(), close().
 */
void sendOrder(const std::string &serialized);

/**
 * Sends an order in a detached thread so the main thread can register more orders
 * without waiting for the network round-trip.
 */
void sendOrderAsync(const std::string &serialized);

/**
 * Blocks until all queued async sends have finished (e.g. before process exit).
 */
void waitForPendingSends();

// obtiene la cantidad de mesas desde el servidor
int getMesasFromServer();

// obtiene la lista de productos desde el servidor
std::vector<std::string> getProductosFromServer();

#endif
