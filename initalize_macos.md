# Restaurant Order System - Cliente (Meseros)

Este proyecto forma parte de un sistema cliente-servidor para la gestión de órdenes de un restaurante.

El módulo actual corresponde al **Cliente (Meseros)**, encargado de:

- Registrar órdenes (mesa, producto, cantidad)
- Modificar órdenes antes de enviarlas
- Validar datos ingresados
- Enviar órdenes al servidor mediante sockets TCP
- Manejar múltiples envíos sin bloquear (uso de threads)

---

# Tecnologías

- Lenguaje: C++
- Comunicación: Sockets TCP
- Concurrencia: std::thread
- Entorno de ejecución: Linux (Docker)
- Desarrollo: macOS (usando Docker como runtime)

---

# Estructura del Proyecto

client/
├── main.cpp
├── order.h
├── order.cpp
├── socket_client.h
├── socket_client.cpp

---

# Requisitos

- Tener Docker instalado en macOS
- Tener Cursor o cualquier editor para modificar el código

---

# Cómo ejecutar en Mac (usando Docker)

## 1. Abrir terminal en la carpeta del proyecto

cd ruta/del/proyecto/client

---

## 2. Crear y ejecutar contenedor Linux

docker run -it --name cpp-client \
-v $(pwd):/app \
-w /app \
ubuntu:22.04

---

## 3. Instalar compilador (solo la primera vez)

Dentro del contenedor:

apt update  
apt install build-essential -y

---

## 4. Compilar el proyecto

g++ main.cpp order.cpp socket_client.cpp -o client -pthread

---

## 5. Ejecutar el cliente

./client

---

# Ejecuciones posteriores

docker start -ai cpp-client

Luego:

g++ main.cpp order.cpp socket_client.cpp -o client -pthread  
./client

---

# Configuración de red

El cliente intenta conectarse a:

- IP: 127.0.0.1
- Puerto: 8080

**Nota:** Es necesario que el servidor esté corriendo para que el envío funcione correctamente.

---

# Uso del programa

1. Create order  
2. Modify order  
3. Send order  
4. Exit  

---

# Problemas comunes

**g++: command not found**  
Solución:  
apt install build-essential -y

**Permission denied**  
Estás intentando ejecutar un archivo .cpp  
Usa: ./client

**No conecta al servidor**  
Verifica que el servidor esté activo y puerto 8080 abierto

---

# Notas técnicas

- El envío de órdenes es asíncrono
- Cada orden se envía en un thread independiente
- Arquitectura cliente-servidor

---

# Autor

Santiago Villarreal
Sistema Cliente-Servidor en C++
