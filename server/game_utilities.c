/*
** EPITECH PROJECT, 2023
** tank.ioInC
** File description:
** game_utilities
*/

#include "server.h"

void parse_hit(char *buffer, tank_t *tank, int client_fd_sender) {
    char **word = strToWordArray(buffer);

    int hittable_player = atoi(word[1]);

    client_room_tank_t *tmp = tank->client_room_tank;

    while (tmp != NULL)
    {
        if (tmp->client_fd == hittable_player)
        {
            tmp->live = tmp->live - 1;

            printf("Player %d has been hit.\n", tmp->client_fd);
            printf("Player %d has %d lives left.\n", tmp->client_fd, tmp->live);
            return;
        }
        tmp = tmp->next;
    }
    printf("The player you tried to hit doesn't exist.\n");
}

void goToRoom(tank_t *tank, int client_fd, char *buffer) {

    Rooms_tank_t *tmp = tank->Rooms_tank;
    char **word = strToWordArray(buffer);

    int room_id = 0;

    if (word[1] == NULL) {
        dprintf(client_fd, "You need to specify a room.\n");
        return;
    }

    room_id = atoi(word[1]);

    int sizeRoom = 2;

    while (tmp != NULL) {
        if (tmp->id_room == room_id) {
            
            for (Rooms_tank_t *tmp2 = tank->Rooms_tank; tmp2; tmp2 = tmp2->next) {
                for (client_room_tank_t *tmp3 = tmp2->client_room_tank; tmp3; tmp3 = tmp3->next) {
                    if (tmp3->client_fd == client_fd) {
                        dprintf(client_fd, "You are already in a room.\n");
                        return;
                    }
                }
            }

            if (tmp->nb_client_in_room >= sizeRoom) {
                dprintf(client_fd, "The room is full.\n");
                return;
            }

            tmp->client_room_tank = add_node_client_room_tank(tmp->client_room_tank, client_fd);
            tmp->nb_client_in_room++;
            return;
        }
        tmp = tmp->next;
    }
}

void exitRoom(tank_t *tank, int client_fd)
{
    Rooms_tank_t *tmp = tank->Rooms_tank;

    //free totalement la node en question
}

void startGame(tank_t *tank, int client_fd)
{
    Rooms_tank_t *tmp = tank->Rooms_tank;

    while (tmp != NULL) {
        for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
            if (tmp2->client_fd == client_fd && tmp->gameStarted == false && tmp->nb_client_in_room == 2) {
                tmp->gameStarted = true;
                for (client_room_tank_t *tmp3 = tmp->client_room_tank; tmp3; tmp3 = tmp3->next) {
                    dprintf(tmp3->client_fd, "The game is starting.\n");
                }
                return;
            }
        }
        tmp = tmp->next;
    }
}