# Peer-to-Peer Client-Server Program

This project implements a simple peer-to-peer communication system using Unix domain sockets. It consists of two programs: a server (`p2p_server`) and a client (`p2p_client`).

## Files Included
- `server.c`: Source code for the server program.
- `client.c`: Source code for the client program.
- `README.md`: This file, containing instructions on how to compile and run the programs.

## Compilation Instructions

### Server Compilation
To compile the server program, use the following commands:
```bash
gcc -o p2p_server server.c -Wall
```
This command compiles `server.c` into an executable named `p2p_server`. Make sure you are in the directory containing `server.c` before running this command.

### Client Compilation
To compile the client program, use the following commands:
```bash
gcc -o p2p_client client.c -Wall
```
This command compiles `client.c` into an executable named `p2p_client`. Make sure you are in the directory containing `client.c` before running this command.

## Running the Programs

### Starting the Server
1. Open a terminal.
2. Navigate to the directory where `p2p_server` is located.
3. Start the server by running the following command:
   ```bash
   ./p2p_server /tmp/mysocket
   ```
   Replace `/tmp/mysocket` with the desired socket file path. The server will start listening on this socket.

### Connecting with the Client
1. Open another terminal or a new instance of your terminal application.
2. Navigate to the directory where `p2p_client` is located.
3. Connect to the server by running the following command:
   ```bash
   ./p2p_client /tmp/mysocket
   ```
   Replace `/tmp/mysocket` with the same socket file path used for starting the server.

### Client-Server Interaction
- Once connected, the client will receive a "HELLO" message from the server.
- The client can then input commands (e.g., "world", "quit") which will be sent to the server.
- The server will process these commands and respond accordingly.
- To terminate the client, enter "quit". This will also terminate the server gracefully.

## Notes
- Ensure that the socket file path (`/tmp/mysocket` in the examples) is the same for both server and client.
- The server and client should be compiled and run on the same system or virtual machine.
- If there are issues with socket binding (e.g., "bind: Address already in use"), make sure no previous instances of the server are running and that the socket file path is not in use.
