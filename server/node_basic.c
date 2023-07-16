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


client_room_tank_t *remove_room_client(client_room_tank_t *list_tank)
{
    client_room_tank_t *tmp = list_tank;
    if (list_tank == NULL)
        return NULL;
    if (list_tank->close == true) {
        list_tank = list_tank->next;
        printf("on remove\n");
        free(tmp);
        return list_tank;
    }
    for (client_room_tank_t *tmp2 = list_tank; tmp->next != NULL; tmp = tmp->next) {
        if (tmp->next->close == true) {
            tmp2 = tmp->next;
            tmp->next = tmp->next->next;
            printf("on remove\n");
            free(tmp2);
            return list_tank;
        }
    }
    return list_tank;
}

//------------Add nodes from the linked list--------------------------//

client_room_tank_t *add_node_client_room_tank(client_room_tank_t *client_room, int fd_cli)
{
    client_room_tank_t *new = malloc(sizeof(client_room_tank_t));
    new->client_fd = fd_cli;
    new->live = 3;
    new->posX = 0.0;
    new->posY = 0.0;
    new->close = false;
    new->next = NULL;

    if (client_room == NULL)
        return (new);
    client_room_tank_t *tmp = client_room;
    for (; tmp->next != NULL; tmp = tmp->next)
        ;
    tmp->next = new;
    return (client_room);
}

//------------Add nodes from the linked list--------------------------//

Rooms_tank_t *add_node_client_room(Rooms_tank_t *room, int id)
{
    Rooms_tank_t *new = malloc(sizeof(Rooms_tank_t));
    new->client_room_tank = NULL;
    new->id_room = id;
    new->nb_client_in_room = 0;
    new->gameStarted = false;
    new->next = NULL;

    if (room == NULL)
        return (new);
    Rooms_tank_t *tmp = room;
    for (; tmp->next != NULL; tmp = tmp->next)
        ;
    tmp->next = new;
    return (room);
}

//------------Add nodes from the linked list--------------------------//

client_tank_t *add_node_client_tank(client_tank_t *tank, int fd_cli)
{
    client_tank_t *new = malloc(sizeof(client_tank_t));
    new->client_fd = fd_cli;
    new->closed = false;
    new->next = NULL;

    if (tank == NULL)
        return (new);
    client_tank_t *tmp = tank;
    for (; tmp->next != NULL; tmp = tmp->next)
        ;
    tmp->next = new;
    return (tank);
}

//------------Remove nodes from the linked list--------------------------//

client_tank_t *remove_node_client_tank(client_tank_t *list_tank)
{
    client_tank_t *tmp = list_tank;
    if (list_tank == NULL)
        return NULL;
    if (list_tank->closed == true)
    {
        list_tank = list_tank->next;
        close(tmp->client_fd);
        free(tmp);
        return list_tank;
    }
    for (client_tank_t *tmp2 = list_tank; tmp->next != NULL; tmp = tmp->next)
    {
        if (tmp->next->closed == true)
        {
            tmp2 = tmp->next;
            tmp->next = tmp->next->next;
            close(tmp2->client_fd);
            free(tmp2);
            return list_tank;
        }
    }
    return list_tank;
}