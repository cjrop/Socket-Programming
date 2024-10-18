#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <mutex>
#include <algorithm>

#define MAX_CLIENTS 30
#define BUFFER_SIZE 1024 // 1KB

// structure to hold client information
struct ClientInfo {
    int socket;
    struct sockaddr_in address;
};

// vector to store connected clients
std::vector<ClientInfo> clients;

// function to broadcast messages to all clients except the sender
void BroadcastMessage(int sender_socket, const char* message, int message_length) {
    for (const auto& client : clients) {
        if (client.socket != sender_socket) {
            send(client.socket, message, message_length, 0);
        }
    }
}

// function to handle client connections
void HandleClient(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while (true) {
        // receive message from client
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            // client disconnected or error occurred
            std::cout << "Client disconnected\n";
            
            // remove client from the vector
            clients.erase(std::remove_if(clients.begin(), clients.end(),
                [client_socket](const ClientInfo& client) { return client.socket == client_socket; }),
                clients.end());
            
            close(client_socket);
            break;
        }

        // broadcast the received message to all other clients
        BroadcastMessage(client_socket, buffer, bytes_received);
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    // create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    // set up server address structure
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    // bind socket to address and port
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Binding failed\n";
        return -1;
    }

    // start listening for incoming connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        std::cerr << "Listen failed\n";
        return -1;
    }

    std::cout << "Server is listening on port 8080\n";

    while (true) {
        // accept new client connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }

        std::cout << "New client connected\n";

        // add new client to the vector
        clients.push_back({client_socket, client_address});

        // create a new thread to handle the client
        std::thread(HandleClient, client_socket).detach();
    }

    return 0;
}
