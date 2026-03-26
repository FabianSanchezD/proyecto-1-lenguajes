#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include "producto.h"

using namespace std;

void menuConfiguracion(vector<producto>& lista);
void agregarProducto(vector<producto>& lista);
void listarProductos(const vector<producto>& lista);
void modificarProducto(vector<producto>& lista);
void eliminarProducto(vector<producto>& lista);
void guardarProductos(const vector<producto>& lista);
void cargarProductos(vector<producto>& lista);
void guardarMesas(int cantidadMesas);
int cargarMesas();

#endif