/*
** EPITECH PROJECT, 2023
** tank_reseau
** File description:
** initialisation
*/

#include "server.h"

//------------Explaination for this file----------------------------------------//
// This file contains the functions to initialise the server and the main struct
// tank_t.

tank_t *init_struct_tank(char *port)
{
    tank_t *tank = malloc(sizeof(tank_t));
    if (tank == NULL) {
        perror("malloc");
        exit(84);
    }

    tank->server_fd = -1;
    FD_ZERO(&tank->readfds);
    memset(&tank->address, 0, sizeof(struct sockaddr_in));

    tank->server_fd = create_socket(atoi(port));
    tank->addrlen = sizeof(tank->address);
    tank->numbertoremove = 0;
    tank->client_tank = NULL;

    return tank;
}