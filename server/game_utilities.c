/*
** EPITECH PROJECT, 2023
** tank.ioInC
** File description:
** game_utilities
*/

#include "server.h"

//------------Explaination for this file------------------------------//
// In this file, there is all functions that are use to manage the game
// and the game loop. You can found the function to create a room, to
// join a room, to set the position of a player, to hit a player and
// to parse the buffer send by the client.


//------------Reset the list of client in a room----------------------//
client_room_tank_t *reset_list_room(client_room_tank_t *list_client_room)
{
    // init a new instance of the list and asign it to the list in parameter

    client_room_tank_t *tmp = list_client_room;
    for (; tmp; tmp = tmp->next) {

        // reset infos linked to the client in the room
        tmp->posX = 0.0;
        tmp->posY = 0.0;
        tmp->direction = 0;
        tmp->close = false;
        tmp->live = 3;
    }
    return (list_client_room);
}


//------------Set the position of a client in a room------------------//
void set(tank_t *tank, int client_fd_sender, client_room_tank_t *list_client_in_room, Rooms_tank_t *list_room, char *buffer)
{
    // transform the buffer in a word array to parse it
    char **word = strToWordArray(buffer);

    // browse the list of room and the list of client in the room to set position and direction of the client
    for (Rooms_tank_t *tmp = tank->Rooms_tank; tmp; tmp = tmp->next) {
        for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
            if (tmp2->client_fd == client_fd_sender && tmp->gameStarted == true) {

                // atof == transform char* to float

                tmp2->posX = atof(word[1]);
                tmp2->posY = atof(word[2]);
                tmp2->direction = atoi(word[3]);

                // send the new position to the client
                dprintf(client_fd_sender, "Your position is now: (x: %f, y: %f, angle: %d)\n", tmp2->posX, tmp2->posY, tmp2->direction);
            }
        }
    }
}

//------------Function to test if the system of win and lose works----//
void parse_hit(char *buffer, tank_t *tank, int client_fd_sender, client_room_tank_t *list_client_in_room, Rooms_tank_t *list_room)
{

    // transform the buffer in a word array to parse it
    char **word = strToWordArray(buffer);

    // get the id of the player that has been hit and transform it from char* to int
    int hittable_player = atoi(word[1]);


    // browse the list of room and the list of client in the room to hit the client
    for (Rooms_tank_t *tmp = tank->Rooms_tank; tmp; tmp = tmp->next) {
        for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
            if (tmp2->client_fd == hittable_player) {

                // send to the client that he has been hit and
                dprintf(tmp2->client_fd, "You have been hit by %d\n", client_fd_sender);
                dprintf(client_fd_sender, "You hit %d\n", hittable_player);
                tmp2->live -= 1;
            }

            if (tmp2->live == 0) {

                // send to the client that he has been killed and to the client that he has killed someone
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

//------------Function to go to a room thank to id room---------------//
void goToRoom(tank_t *tank, int client_fd, char *buffer)
{
    // init a new instance of the list and asign it to the list in parameter
    Rooms_tank_t *tmp = tank->Rooms_tank;

    // transform the buffer in a word array to parse it
    char **word = strToWordArray(buffer);

    // init variable to stock the id of the room
    int room_id = 0;


    // error management if the client doesn't specify a room
    if (word[1] == NULL) {
        dprintf(client_fd, "You need to specify a room.\n");
        return;
    }

    // fill the variable room_id with the id of the room
    room_id = atoi(word[1]);

    // max size of a room
    int sizeRoom = 2;


    // browse the list of room to find the room with the id in parameter (using a new instance of the list declared before and 
        // a while loop but it's exactly the same thing as if we use a for loop)

    while (tmp != NULL) {
        if (tmp->id_room == room_id) {

            //thoose 2 for loop are used to check if the client is already in a room
            for (Rooms_tank_t *tmp2 = tank->Rooms_tank; tmp2; tmp2 = tmp2->next) {
                for (client_room_tank_t *tmp3 = tmp2->client_room_tank; tmp3; tmp3 = tmp3->next) {
                    if (tmp3->client_fd == client_fd) {
                        dprintf(client_fd, "You are already in a room.\n");
                        return;
                    }
                }
            }

            // if the room is full, send a message to the client and return
            if (tmp->nb_client_in_room >= sizeRoom) {
                dprintf(client_fd, "The room is full.\n");
                return;
            }

            // if the room is not full, add the client to the room and increment the number of client in the room
            tmp->client_room_tank = add_node_client_room_tank(tmp->client_room_tank, client_fd);
            tmp->nb_client_in_room++;
            return;
        }
        tmp = tmp->next;
    }
}


//------------Start a game in a room----------------------------------//
void startGame(tank_t *tank, int client_fd)
{

    // init a new instance of the list and asign it to the list in parameter
    Rooms_tank_t *tmp = tank->Rooms_tank;


    // browse the list of room and the list of client in the room to start the game
    while (tmp != NULL) {
        for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {

            // check if the client is in the room and if the game is not already started and if there is 2 client in the room
            if (tmp2->client_fd == client_fd && tmp->gameStarted == false && tmp->nb_client_in_room == 2) {
                tmp->gameStarted = true;

                // send a message to all clientsin the room to tell him that the game is starting
                for (client_room_tank_t *tmp3 = tmp->client_room_tank; tmp3; tmp3 = tmp3->next) {
                    dprintf(tmp3->client_fd, "The game is starting.\n");
                }
                return;
            }
        }
        tmp = tmp->next;
    }
}

