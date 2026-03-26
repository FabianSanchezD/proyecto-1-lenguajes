CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

.PHONY: all client server clean

all: client server

client:
	$(CXX) $(CXXFLAGS) client/main.cpp client/order.cpp client/socket_client.cpp \
		-o client/client

server:
	$(CXX) $(CXXFLAGS) server/main.cpp server/order_store.cpp server/server_socket.cpp \
		-o server/server

clean:
	rm -f client/client server/server
