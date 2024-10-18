#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

// constants for server IP and port
const char* SERVER_IP = "127.0.0.1";  // or the correct IP if not on localhost
const int SERVER_PORT = 8080;

// function to receive messages from the server
void ReceiveMessage(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while (true) {
        // receive message from server
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            // server disconnected or error occurred
            std::cout << "Disconnected from server\n";
            break;
        }

        // display the received message
        buffer[bytes_received] = '\0';
        std::cout << buffer << std::endl;
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in server_address;

    // create a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error: " << strerror(errno) << std::endl;
        return -1;
    }

    // set up server address structure
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    // convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported: " << strerror(errno) << std::endl;
        return -1;
    }

    // connect to the server
    if(connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection failed: " << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << "Connected to server\n";

    // create a thread to receive messages from the server
    std::thread receive_thread(ReceiveMessage, sock);

    char message[BUFFER_SIZE];
    while (true) {
        // read user input
        std::cin.getline(message, BUFFER_SIZE);

        // send the message to the server
        send(sock, message, strlen(message), 0);

        // check if user wants to quit
        if (strcmp(message, "quit") == 0) {
            break;
        }
    }

    // close the socket and end the program
    close(sock);
    receive_thread.join();

    return 0;
}
