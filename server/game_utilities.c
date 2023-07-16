/*
** EPITECH PROJECT, 2023
** tank.ioInC
** File description:
** game_utilities
*/

#include "server.h"

client_room_tank_t *reset_list_room(client_room_tank_t *list_client_room)
{
    client_room_tank_t *tmp = list_client_room;
    for (; tmp; tmp = tmp->next) {
        tmp->posX = 0.0;
        tmp->posY = 0.0;
        tmp->direction = 0;
        tmp->close = false;
        tmp->live = 3;
    }

    return (list_client_room);
}

void set(tank_t *tank, int client_fd_sender, client_room_tank_t *list_client_in_room, Rooms_tank_t *list_room, char *buffer)
{
    char **word = NULL;
    word = strToWordArray(buffer);

    for (Rooms_tank_t *tmp = tank->Rooms_tank; tmp; tmp = tmp->next) {
        for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
            if (tmp2->client_fd == client_fd_sender && tmp->gameStarted == true) {
                tmp2->posX = atof(word[1]);
                tmp2->posY = atof(word[2]);
                tmp2->direction = atoi(word[3]);

                dprintf(client_fd_sender, "Your position is now: (x: %f, y: %f, angle: %d)\n", tmp2->posX, tmp2->posY, tmp2->direction);
            }
        }
    }


    for (Rooms_tank_t *tmp = tank->Rooms_tank; tmp; tmp = tmp->next) {
        for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
            printf("client: %d\n", tmp2->client_fd);
        }
    }

}

void parse_hit(char *buffer, tank_t *tank, int client_fd_sender, client_room_tank_t *list_client_in_room, Rooms_tank_t *list_room) {
    char **word = strToWordArray(buffer);
    int hittable_player = atoi(word[1]);

    for (Rooms_tank_t *tmp = tank->Rooms_tank; tmp; tmp = tmp->next) {
        for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
            if (tmp2->client_fd == hittable_player) {
                dprintf(tmp2->client_fd, "You have been hit by %d\n", client_fd_sender);
                dprintf(client_fd_sender, "You hit %d\n", hittable_player);
                tmp2->live -= 1;
            }

            if (tmp2->live == 0) {
                dprintf(tmp2->client_fd, "You died. Player: %d has killed you.\n", client_fd_sender);
                dprintf(client_fd_sender, "Victory !, You killed player: %d\n", hittable_player);
                tmp2->close = true;
                tmp->gameStarted = false;
                tmp->nb_client_in_room = 2;
                tmp->client_room_tank = reset_list_room(tmp->client_room_tank);
            }
        }
    }

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

