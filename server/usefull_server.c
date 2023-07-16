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
// in this function we check if the server_fd is set in the fd_set
// if it's okey we accept the client and add him to the linked list
// of the server
void accept_client(int select_ret, tank_t *tank)
{
    // if we have a new connection on the server_fd
    if (FD_ISSET(tank->server_fd, &tank->readfds)) {

        // initialise a new socket for the client, and accept it
        int new_socket = accept(tank->server_fd, \
        (struct sockaddr *)&tank->address, (socklen_t*)&tank->addrlen);

        // check if accept failed
        if (new_socket < 0) {
            perror("accept");
            exit(84);
        }

        // if accept is okey, we send a message to the client
        dprintf(new_socket, "WELCOME\n");

        // this is just to print the ip and port of the client, for the server log
        printf("New connection, socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(tank->address.sin_addr), ntohs(tank->address.sin_port));

        // add the client to the linked list of the server
        tank->client_tank = add_node_client_tank(tank->client_tank, new_socket);
    }
}


//------------Create a socket and bind it to the port-----------------//
// in this function we create a socket and bind it to the port
// we also set the socket to listen
// this is necessary for a server, because it's the socket that
// will be used to accept clients
int create_socket(int port)
{

    // create the socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // call the struct sockaddr_in
    struct sockaddr_in serv_addr;
    int opt = 1;

    // use setsockopt to set the socket to reuse the address, (thanks to this, we can restart the server without waiting that the port is free)
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
    &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(84);
    }

    // assign bind informations
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    // error management for bind and listen
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

    // initialise a buffer to store the content of the client
    int bytes_received =
        recv(list_tank->client_fd, tank->buffer, 1024, 0);

    // if the client is disconnected, we remove him from the linked list
    if (bytes_received == 0) {
        printf("the user %d DISCONNECT\n", list_tank->client_fd);
        list_tank->closed = true;

        // here, we call the function to remove the client from a room, it's for the function gout
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

        // if the client send something, we print it
        tank->buffer[bytes_received] = '\0';
        printf("Client: %d said: %s", list_tank->client_fd, tank->buffer);


        ////////////////////////////COMMANDS////////////////////////////

        // if the client send clear, we clear the terminal of the server (it's for me to debug, you can remove it)
        if (strcmp(tank->buffer, "clear\n") == 0) {
            system("clear");
            printf("terminal cleared!\n");
        }

        // if the client send hit <id>, we call the function to parse the command and to hit the client, (normally, this will not be used in the final version, it's just for me to debug)
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

            // if the client is not in a room, we send him a message
            if (checkClientIsInRoom == 0) {
                dprintf(list_tank->client_fd, "You're not affected to a room");
            }
        }


        // if the client send setPos <x> <y> <direction (0 - 360)>, we call the function to parse the command and to set the position of the client
        if (strncmp(tank->buffer, "setPos", 6) == 0) {
            for (Rooms_tank_t *tmp = tank->Rooms_tank; tmp; tmp = tmp->next) {
                for (client_room_tank_t *tmp2 = tmp->client_room_tank; tmp2; tmp2 = tmp2->next) {
                    if (tmp2->client_fd == list_tank->client_fd && tmp->gameStarted == true) {
                        set(tank, list_tank->client_fd, tmp2, tmp, tank->buffer);
                    }
                }
            }
        }

        // if the client send rooms, we send him the list of rooms, with the status of player in the room (if the game is started or not)
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


        // this command is important to debug if you wanna edit my code and to understand how it works. Here, you can add all
        // informations about clients in room, out rooms, and just client connected to the server (the common point is the
        // client_fd, you can use it to find the client you want to find in the linked list of player, and clients connected to the server)
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


        // just to go to a room, you can't go to a room if the game is started, and if you are already in a room
        if (strncmp(tank->buffer, "gto", 3) == 0) {
            goToRoom(tank, list_tank->client_fd, tank->buffer);
        }


        // just to go out of a room, you can't go out of a room if you are not in a room
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
        
        // if the client send start, we call the function to start the game in the room where the client is
        if (strcmp(tank->buffer, "start\n") == 0) {
            startGame(tank, list_tank->client_fd);
        }


        // if the client send help, we send him the list of command
        if (strcmp(tank->buffer, "help\n") == 0) {
            dprintf(list_tank->client_fd, "=====list of command=====\n");
            dprintf(list_tank->client_fd, "\trooms: list of room\n");
            dprintf(list_tank->client_fd, "\twhoami: give your id\n");
            dprintf(list_tank->client_fd, "\tgto: go to a room\n");
            dprintf(list_tank->client_fd, "\tgout: go out of a room\n");
            dprintf(list_tank->client_fd, "\tstart: start the game in the room where you are\n");
            dprintf(list_tank->client_fd, "\thit: hit a client\n");
            dprintf(list_tank->client_fd, "\tsetPos: set your position\n");
            dprintf(list_tank->client_fd, "\texit: exit the server\n");
            dprintf(list_tank->client_fd, "======================\n");
        }
    }
}

//------------Remove a client from the linkedlist or read client------//
// this is the loop of the server
void loop_server(tank_t *tank)
{
    client_tank_t *tank_list = tank->client_tank;

    while (tank_list != NULL) {
        if (FD_ISSET(tank_list->client_fd, &tank->readfds)) {
            // read information from client
            read_client(tank_list, tank);
        }
        tank_list = tank_list->next;
    }

    // remove client from linked list if they are disconnected
    while (tank->numbertoremove > 0) {
        tank->client_tank = remove_node_client_tank(tank->client_tank);
        tank->client_room_tank = remove_room_client(tank->client_room_tank);
        tank->numbertoremove--;
    }
}


//------------Reset fd set--------------------------------------------//
// this function is important to reset the fd_set, because if you don't reset it, the select will not work
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
// this is the MAIN loop of the server, it's the loop of the select
// select is a function that wait for an event on a fd, and if there is an event, it will return the fd where the event is
// if there is no event, it will wait until there is an event
// so, in this loop, we wait for an event on a fd, and if there is an event, we call the function to read the client
// and we call the function to accept a client
int loop_server_tank(tank_t *tank)
{
    // so we put a while(1) to loop the select (while(true) is the same thing, but it's not C standard (C99))
    while (1) {

        // we call the function to reset the fd_set at each loop
        set_fd(tank);

        // we call the select, and we wait for an event on a fd
        int select_ret = select(FD_SETSIZE, &tank->readfds, NULL, NULL, NULL);

        // check if there is an error on the select
        if (select_ret < 0) {
            perror("select");
            return (84);
        }

        // if there is an event on the server fd, we call the function to accept a client
        accept_client(select_ret, tank);

        // and we call the function to read the client
        loop_server(tank);
    }
}