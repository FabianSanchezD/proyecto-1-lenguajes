#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "producto.h"

using namespace std;

void agregarProducto(vector<producto>& lista) {
    producto p;

    cout << "Nombre del producto: ";
    cin.ignore();
    getline(cin, p.nombre);

    cout << "Precio: ";
    cin >> p.precio;

    lista.push_back(p);

    cout << "Producto agregado.\n";
}

void listarProductos(const vector<producto>& lista) {
    if (lista.empty()) {
        cout << "No hay productos.\n";
        return;
    }
    cout << "ID. Nombre - Precio\n";
    for (int i = 0; i < lista.size(); i++) {
        cout << i << ". " << lista[i].nombre 
             << " - " << lista[i].precio << endl;
    }
}

void eliminarProducto(vector<producto>& lista) {
    int id;

    cout << "Ingrese el ID del producto a eliminar: ";
    cin >> id;

    if (id >= 0 && id < lista.size()) {
        lista.erase(lista.begin() + id);
        cout << "Producto eliminado.\n";
    } else {
        cout << "ID invalido.\n";
    }
}

void modificarProducto(vector<producto>& lista) {
    int id;

    cout << "Ingrese el ID del producto a modificar: ";
    cin >> id;

    if (id >= 0 && id < lista.size()) {
        cout << "Nombre actual: " << lista[id].nombre << endl;
        cout << "Nuevo nombre: ";
        cin.ignore();
        getline(cin, lista[id].nombre);

        cout << "Precio actual: " << lista[id].precio << endl;
        cout << "Nuevo precio: ";
        cin >> lista[id].precio;

        cout << "Producto modificado.\n";
    } else {
        cout << "ID invalido.\n";
    }
}

void guardarProductos(const vector<producto>& lista) {
    ofstream archivo("Datos/productos.txt");

    for (int i = 0; i < lista.size(); i++) {
        archivo << lista[i].nombre << "," << lista[i].precio << endl;
    }

    archivo.close();
}

void cargarProductos(vector<producto>& lista) {
    ifstream archivo("Datos/productos.txt");

    if (!archivo.is_open()) return;

    producto p;
    string linea;

    while (getline(archivo, linea)) {
        int coma = linea.find(",");

        p.nombre = linea.substr(0, coma);
        p.precio = stod(linea.substr(coma + 1));

        lista.push_back(p);
    }

    archivo.close();
}

void guardarMesas(int cantidadMesas) {
    ofstream archivo("Datos/mesas.txt");
    if (!archivo.is_open()) return;

    archivo << cantidadMesas << endl;
    archivo.close();
}

int cargarMesas() {
    ifstream archivo("Datos/mesas.txt");
    int cantidadMesas = 0;
    if (!archivo.is_open()) return cantidadMesas;

    archivo >> cantidadMesas;
    archivo.close();
    return cantidadMesas;
}

void menuConfiguracion(vector<producto>& lista){
    int opcion = -1;

    while(opcion != 0) {
        cout << "\n=== Configuracion ===\n";
        cout << "1. Agregar producto\n";
        cout << "2. Listar productos\n";
        cout << "3. Eliminar producto\n";
        cout << "4. Modificar producto\n";
        cout << "5. Configurar mesas\n";
        cout << "6. Regresar\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1:
                agregarProducto(lista);
                guardarProductos(lista);
                break;
            case 2:
                listarProductos(lista);
                break;
            case 3:
                eliminarProducto(lista);
                guardarProductos(lista);
                break;
            case 4:
                listarProductos(lista);
                modificarProducto(lista);
                guardarProductos(lista);
                break;
            case 5: {
                int cantidadMesas;
                cout << "Mesas actuales: " << cargarMesas() << endl;
                cout << "Ingrese la nueva cantidad de mesas: ";
                cin >> cantidadMesas;
                guardarMesas(cantidadMesas);
                break;
            }
            case 6:
                return;
            default:
                cout << "Opcion invalida.\n";
        }

        opcion = -1; 

    } 

}