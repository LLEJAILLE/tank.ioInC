/*
** EPITECH PROJECT, 2023
** tank_reseau
** File description:
** main
*/

#include "client.h"

void init_client(client_t *clients)
{
    clients->port = 8080;
    clients->ip = "127.0.0.1";
    clients->buffer[0] = '\0';
}

int init_cli(client_t *clients, int client_socket, struct sockaddr_in *server_address)
{
    memset(server_address, 0, sizeof(struct sockaddr_in));
    server_address->sin_family = AF_INET;
    server_address->sin_port = htons(clients->port);
    if (inet_pton(AF_INET, clients->ip, &(server_address->sin_addr)) <= 0) {
        printf("Error: Invalid IP Address.\n");
        return 84;
    }

    if (connect(client_socket, (struct sockaddr*)server_address, sizeof(struct sockaddr_in)) < 0) {
        printf("Error: Failed to connect to server.\n");
        return 84;
    }

    return 0;
}

int receive_data(client_t *clients, int client_socket, fd_set readfds)
{
    if (select(client_socket + 1, &readfds, NULL, NULL, NULL) == -1) {
        printf("Error: select failed.\n");
        return 84;
    }
    if (FD_ISSET(STDIN_FILENO, &readfds)) {
        fgets(clients->buffer, BUFFER_SIZE, stdin);
        send(client_socket, clients->buffer, strlen(clients->buffer), 0);
        memset(clients->buffer, 0, BUFFER_SIZE);
    }
    if (FD_ISSET(client_socket, &readfds)) {
        ssize_t bytes_received = recv(client_socket, clients->buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            printf("Error: Failed to receive data from server.\n");
            return 84;
        }
        clients->buffer[bytes_received] = '\0';
        printf("%s", clients->buffer);
        memset(clients->buffer, 0, BUFFER_SIZE);
    }

    return 0;
}

void boucle_cli(client_t *clients, int client_socket)
{
    fd_set readfds;
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(client_socket, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        if (receive_data(clients, client_socket, readfds) == 84)
            break;
    }
}

int main(int argc, char *argv[]) {

    client_t *clients = malloc(sizeof(client_t));
    init_client(clients);

    int client_socket;
    struct sockaddr_in server_address;
    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Error: Failed to create socket.\n");
        return 1;
    }

    if (init_cli(clients, client_socket, &server_address) == 84)
        return 84;

    boucle_cli(clients, client_socket);

    close(client_socket);
    return 0;
}
