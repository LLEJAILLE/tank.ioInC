/*
** EPITECH PROJECT, 2023
** tank_reseau
** File description:
** usefull_server
*/

//------------Explaination for this file------------------------------//
// This file contains all the functions that are used in the loop of
// the server. You can found the function to accept a client, to create
//a socket, read the content send by clients and reset fd set.


#include "server.h"


//------------Accept a client and store him to the linkedlist---------//
void accept_client(int select_ret, tank_t *tank)
{
    if (FD_ISSET(tank->server_fd, &tank->readfds)) {
        int new_socket = accept(tank->server_fd, \
        (struct sockaddr *)&tank->address, (socklen_t*)&tank->addrlen);
        if (new_socket < 0) {
            perror("accept");
            exit(84);
        }
        dprintf(new_socket, "WELCOME\n");
        printf("New connection, socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(tank->address.sin_addr), ntohs(tank->address.sin_port));
        tank->client_tank =
        add_node_client_tank(tank->client_tank, new_socket);
    }
}


//------------Create a socket and bind it to the port-----------------//
int create_socket(int port)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
    &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(84);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(socket_fd, (struct sockaddr *)
        &serv_addr, sizeof(serv_addr)) < 0) {
        exit(84);
    }
    if (listen(socket_fd, 1024) < 0) {
        exit(84);
    }
    return (socket_fd);
}


//------------Read the content send by clients------------------------//
void read_client(client_tank_t *list_tank, tank_t *tank)
{
    int bytes_received =
        recv(list_tank->client_fd, tank->buffer, 1024, 0);
    if (bytes_received == 0) {
        printf("the user %d DISCONNECT\n", list_tank->client_fd);
        list_tank->closed = true;
        tank->numbertoremove++;
    } else {
        tank->buffer[bytes_received] = '\0';
        printf("Client: %d said: %s", list_tank->client_fd, tank->buffer);

        if (strcmp(tank->buffer, "clear\n") == 0) {
            system("clear");
            printf("terminal cleared!\n");
        }


        if (strcmp(tank->buffer, "exit\n") == 0) {
            printf("Server is shutting down.\n");
            // Fermez les connexions, libérez la mémoire, etc.
            exit(0);
        }
    }
}


//------------Remove a client from the linkedlist or read client------//
void loop_server(tank_t *tank)
{
    client_tank_t *tank_list = tank->client_tank;

    while (tank_list != NULL) {
        if (FD_ISSET(tank_list->client_fd, &tank->readfds)) {
            read_client(tank_list, tank);
        }
        tank_list = tank_list->next;
    }
    while (tank->numbertoremove > 0) {
        tank->client_tank = remove_node_client_tank(tank->client_tank);
        tank->numbertoremove--;
    }
}


//------------Reset fd set--------------------------------------------//
void set_fd(tank_t *tank)
{
    FD_ZERO(&tank->readfds);
    FD_SET(tank->server_fd, &tank->readfds);

    for (client_tank_t *tmp =
    tank->client_tank; tmp; tmp = tmp->next) {
        if (tmp->client_fd != -1)
            FD_SET(tmp->client_fd, &tank->readfds);
    }
}


//------------Loop server----------------------------------------------//
int loop_server_tank(tank_t *tank)
{
    while (1) {
        set_fd(tank);
        int select_ret =
            select(FD_SETSIZE, &tank->readfds, NULL, NULL, NULL);
        if (select_ret < 0) {
            perror("select");
            return (84);
        }
        accept_client(select_ret, tank);
        loop_server(tank);
    }
}