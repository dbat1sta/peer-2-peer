#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

int server_socket;

void handle_signal(int sig) {
    unlink("/home/smarz1/courses/ece553/p2p_socket/myserver");
    close(server_socket);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <socket_path>\n", argv[0]);
        exit(1);
    }

    signal(SIGINT, handle_signal);

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_un server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, argv[1]);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_socket);
        exit(1);
    }

    if (listen(server_socket, 5) < 0) {
        perror("listen");
        close(server_socket);
        exit(1);
    }

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        send(client_socket, "HELLO", 5, 0);

        while (1) {
            char buffer[256];
            int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_read < 0) {
                perror("recv");
                close(client_socket);
                break;
            }
            buffer[bytes_read] = '\0';

            if (strcmp(buffer, "quit") == 0) {
                close(client_socket);
                break;
            }

            printf("Client says: %s\n", buffer);
            send(client_socket, "ENTERCMD", 8, 0);
        }

        close(client_socket);
    }

    close(server_socket);
    unlink(argv[1]);
    return 0;
}
