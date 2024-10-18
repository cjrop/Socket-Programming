# Socket Programming

Implemented in C++ using sockets and threading. It allows multiple clients to connect to a central server and exchange messages in real-time.

This is a simple implementation/example of socket programming in C++. I got the idea from a professor who showed an example in my Operating Systems class.

**Resources:**
[Socket Programming in C++](https://www.geeksforgeeks.org/socket-programming-cc/)

## Requirements

- C++11 or later
- g++ compiler

## Compilation

To compile the server and client, use the following commands:

```bash
bash
g++ -std=c++11 -pthread server.cpp -o server
g++ -std=c++11 -pthread client.cpp -o client
```

## Usage

Using separate terminals that are in the same directory follow the commands below.

### Starting the Server
Run the server executable:

```bash
./server
```

The server will start and listen for incoming connections on port 8080.

### Connecting a Client
Run the client executable:
```bash
./client
```

The client will attempt to connect to the server running on localhost (127.0.0.1) on port 8080.

To connect to a different IP address, modify the `SERVER_IP` constant in `client.cpp` before compiling.

## Files

- `server.cpp`: Contains the server-side code
- `client.cpp`: Contains the client-side code
- `README.md`: This file, containing usage instructions and project information