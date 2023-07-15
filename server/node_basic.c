/*
** EPITECH PROJECT, 2023
** tank_reseau
** File description:
** node_basic
*/

#include "server.h"

//------------Explaination for this file----------------------------------------//
// This file contains the functions to add and remove nodes from the linked list
// of client_tank_t. The linked list is used to store the clients connected to
// the server.

//------------Add nodes from the linked list--------------------------//

client_tank_t *add_node_client_tank(client_tank_t *tank, int fd_cli)
{
    client_tank_t *new = malloc(sizeof(client_tank_t));
    new->client_fd = fd_cli;
    new->closed = false;
    new->live = 3;
    new->posX = 0;
    new->posY = 0;
    new->next = NULL;

    if (tank == NULL)
        return (new);
    client_tank_t *tmp = tank;
    for (; tmp->next != NULL; tmp = tmp->next);
    tmp->next = new;
    return (tank);
}

//------------Remove nodes from the linked list--------------------------//

client_tank_t *remove_node_client_tank(client_tank_t *list_tank)
{
    client_tank_t *tmp = list_tank;
    if (list_tank == NULL)
        return (NULL);
    if (list_tank->closed == true) {
        list_tank = list_tank->next;
        close(tmp->client_fd);
        free(tmp);
        return (list_tank);
    }
    for (client_tank_t *tmp2 = list_tank;
        tmp->next != NULL; tmp = tmp->next) {
        if (tmp->next->closed == true) {
            tmp2 = tmp->next;
            tmp->next = tmp->next->next;
            close(tmp2->client_fd);
            free(tmp2);
            return (list_tank);
        }
    }
    return (list_tank);
}