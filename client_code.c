#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <socket_path>\n", argv[0]);
        exit(1);
    }

    int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_un client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strcpy(client_addr.sun_path, argv[1]);

    if (connect(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        perror("connect");
        close(client_socket);
        exit(1);
    }

    char buffer[256];
    int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read < 0) {
        perror("recv");
        close(client_socket);
        exit(1);
    }
    buffer[bytes_read] = '\0';

    if (strcmp(buffer, "HELLO") == 0) {
        printf("Server said: HELLO\n");
        send(client_socket, "THANKS", 6, 0);
    }

    while (1) {
        bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read < 0) {
            perror("recv");
            close(client_socket);
            exit(1);
        }
        buffer[bytes_read] = '\0';

        if (strcmp(buffer, "ENTERCMD") == 0) {
            printf("Enter command: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';

            send(client_socket, buffer, strlen(buffer), 0);

            if (strcmp(buffer, "quit") == 0) {
                printf("Quitting!\n");
                break;
            }
        }
    }

    close(client_socket);
    return 0;
}
