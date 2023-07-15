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

