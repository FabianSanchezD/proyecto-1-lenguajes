#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include "producto.h"

using namespace std;

void agregarProducto(vector<producto>& lista) {
    producto p;

    cout << "Nombre del producto: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    getline(cin, p.nombre);

    while (true) {
        cout << "Precio: ";
        if (cin >> p.precio && p.precio > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break; 
        } else {
            cout << "Entrada invalida. Ingrese un numero positivo.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
    }

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
        if (!(cin >> id)) {
            cout << "Entrada invalida. Debe ingresar un numero.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            if (id >= 0 && id < lista.size()) {
                lista.erase(lista.begin() + id);
                cout << "Producto eliminado.\n";
            } else {
                cout << "ID invalido.\n";
            }
        }
}

void modificarProducto(vector<producto>& lista) {
    if (lista.empty()) {
        cout << "No hay productos para modificar.\n";
        return;
    }

    int id;

    while (true) {
        cout << "Ingrese el ID del producto a modificar: ";

        if (!(cin >> id)) {
            cout << "Entrada invalida. Debe ingresar un numero.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (id < 0 || id >= lista.size()) {
            cout << "ID invalido. Intente nuevamente.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            break; 
        }
    }

    cout << "Nombre actual: " << lista[id].nombre << endl;
    cout << "Nuevo nombre: ";
    getline(cin, lista[id].nombre);

    while (true) {
        cout << "Precio actual: " << lista[id].precio << endl;
        cout << "Nuevo precio: ";
        if (cin >> lista[id].precio && lista[id].precio > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break; // precio válido
        } else {
            cout << "Entrada invalida. Ingrese un numero positivo.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    cout << "Producto modificado.\n";
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
    int opcion;

    while(true) {
        cout << "\n=== Configuracion ===\n";
        cout << "1. Agregar producto\n";
        cout << "2. Listar productos\n";
        cout << "3. Eliminar producto\n";
        cout << "4. Modificar producto\n";
        cout << "5. Configurar mesas\n";
        cout << "6. Regresar\n";
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) {
            // entrada no numérica
            cout << "Entrada invalida. Debe ingresar un numero.\n";
            cin.clear(); // limpiar error
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue; 
        }

        switch(opcion) {
            case 1:
                agregarProducto(lista);
                guardarProductos(lista);
                break;
            case 2:
                listarProductos(lista);
                break;
            case 3:
                listarProductos(lista);
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
                if (!(cin >> cantidadMesas)) {
                    cout << "Entrada invalida. Debe ingresar un numero.\n";
                    cin.clear(); // limpiar error
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue; 
                }
                else if (cantidadMesas <= 0) {
                    cout << "La cantidad de mesas debe ser mayor a 0.\n";
                    continue;
                }
                else {
                    guardarMesas(cantidadMesas);
                    cout << "Cantidad de mesas actualizada.\n";
                }
                break;
            }
            case 6:
                return;
            default:
                cout << "Opcion invalida.\n";
        }

    } 

}