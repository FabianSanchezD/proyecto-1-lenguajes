#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <string>
#include <vector>

/** Dirección del servidor usada por el cliente mesero (configuración típica de desarrollo local). */
constexpr const char *kServerHost = "127.0.0.1";
constexpr int kServerPort = 8080;

/**
 * Abre una conexión TCP, envía una cadena de pedido serializada y cierra el socket.
 * Usa sockets Linux/Berkeley: socket(), connect(), send(), close().
 */
void sendOrder(const std::string &serialized);

/**
 * Envía un pedido en un hilo separado para que el hilo principal pueda registrar
 * más pedidos sin esperar el viaje de ida y vuelta por la red.
 */
void sendOrderAsync(const std::string &serialized);

/**
 * Bloquea hasta que todos los envíos asíncronos en cola hayan finalizado (ej. antes de salir del proceso).
 */
void waitForPendingSends();

// obtiene la cantidad de mesas desde el servidor
int getMesasFromServer();

// obtiene la lista de productos desde el servidor
std::vector<std::string> getProductosFromServer();

#endif
