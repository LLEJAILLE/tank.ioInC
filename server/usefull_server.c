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

        for (Rooms_tank_t *tmp = tank->Rooms_tank; tmp; tmp = tmp->next) {
            for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
                if (tmp2->client_fd == list_tank->client_fd) {
                    tmp2->close = true;
                    tmp->client_room_tank = remove_room_client(tmp->client_room_tank);
                    dprintf(list_tank->client_fd, "You are out of the room %d\n", tmp->id_room);
                    break;
                }
            }
        }
        
        tank->numbertoremove++;
    } else {
        tank->buffer[bytes_received] = '\0';
        printf("Client: %d said: %s", list_tank->client_fd, tank->buffer);

        if (strcmp(tank->buffer, "clear\n") == 0) {
            system("clear");
            printf("terminal cleared!\n");
        }

        if (strncmp(tank->buffer, "hit", 3) == 0) {
            int checkClientIsInRoom = 0;

            for (Rooms_tank_t *tmp = tank->Rooms_tank; tmp; tmp = tmp->next) {
                for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
                    if (tmp2->client_fd == list_tank->client_fd && tmp->gameStarted == true) {
                        checkClientIsInRoom = 1;
                        parse_hit(tank->buffer, tank, list_tank->client_fd, tmp2, tmp);
                    }
                }
            }
            if (checkClientIsInRoom == 0) {
                dprintf(list_tank->client_fd, "You're not affected to a room");
            }
        }

        if (strncmp(tank->buffer, "setPos", 6) == 0) {
            for (Rooms_tank_t *tmp = tank->Rooms_tank; tmp; tmp = tmp->next) {
                for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
                    if (tmp2->client_fd == list_tank->client_fd && tmp->gameStarted == true) {
                        set(tank, list_tank->client_fd, tmp2, tmp, tank->buffer);
                    }
                }
            }
        }

        if (strcmp(tank->buffer, "rooms\n") == 0) {
            dprintf(list_tank->client_fd, "=====list of room=====\n");
            for (Rooms_tank_t *tmp = tank->Rooms_tank; tmp; tmp = tmp->next) {
                dprintf(list_tank->client_fd, "\troom: %d", tmp->id_room);
                if (tmp->gameStarted == true) {
                    dprintf(list_tank->client_fd, "\t[Game started]\n");
                } else {
                    dprintf(list_tank->client_fd, "\t[Game not started]\n");
                }
                for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
                    if (tmp->gameStarted == true) {
                        dprintf(list_tank->client_fd, "\t\tplayer: %d, lifes remaining: %d\n", tmp2->client_fd, tmp2->live);
                    } else {
                        dprintf(list_tank->client_fd, "\t\tplayer: %d\n", tmp2->client_fd);
                    }
                }
            }
            dprintf(list_tank->client_fd, "======================\n");
            dprintf(list_tank->client_fd, "There is %d rooms\n", tank->nbRoom);
        }

        if (strcmp(tank->buffer, "whoami\n") == 0) {
            dprintf(list_tank->client_fd, "You are the client number: %d\n", list_tank->client_fd);
            
            for (Rooms_tank_t *tmp = tank->Rooms_tank; tmp; tmp = tmp->next) {
                for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
                    if (tmp2->client_fd == list_tank->client_fd) {
                        dprintf(list_tank->client_fd, "You are in room %d, you have %d lives remaining, posX: %f, posY: %f, angle: %d\n", tmp->id_room, tmp2->live, tmp2->posX, tmp2->posY, tmp2->direction);
                    }
                }
            }
        }

        if (strncmp(tank->buffer, "gto", 3) == 0) {
            goToRoom(tank, list_tank->client_fd, tank->buffer);
        }

        if (strncmp(tank->buffer, "gout", 4) == 0) {
            for (Rooms_tank_t *tmp = tank->Rooms_tank; tmp; tmp = tmp->next) {
                for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
                    if (tmp2->client_fd == list_tank->client_fd) {
                        tmp2->close = true;
                        tmp->client_room_tank = remove_room_client(tmp->client_room_tank);
                        tmp->nb_client_in_room--;
                        dprintf(list_tank->client_fd, "You are out of the room %d\n", tmp->id_room);
                        break;
                    }
                }
            }
        }

        if (strcmp(tank->buffer, "start\n") == 0) {
            startGame(tank, list_tank->client_fd);
        }

        if (strcmp(tank->buffer, "help\n") == 0) {
            dprintf(list_tank->client_fd, "=====list of command=====\n");
            dprintf(list_tank->client_fd, "\trooms: list of room\n");
            dprintf(list_tank->client_fd, "\twhoami: give your id\n");
            dprintf(list_tank->client_fd, "\tgto: go to a room\n");
            dprintf(list_tank->client_fd, "\tgout: go out of a room\n");
            dprintf(list_tank->client_fd, "\thit: hit a client\n");
            dprintf(list_tank->client_fd, "\tsetPos: set your position\n");
            dprintf(list_tank->client_fd, "\texit: exit the server\n");
            dprintf(list_tank->client_fd, "======================\n");
        }


        // if (strcmp(tank->buffer, "exit\n") == 0) {
        //     printf("Server is shutting down.\n");
        //     // Fermez les connexions, libérez la mémoire, etc.
        //     exit(0);
        // }
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
        tank->client_room_tank = remove_room_client(tank->client_room_tank);
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