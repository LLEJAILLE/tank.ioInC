/*
** EPITECH PROJECT, 2023
** tank_reseau
** File description:
** server
*/

//------------Explaination for this file------------------------------//
// This file contains all include and prototypes for the server.
// But also structs for the server and prototypes of linked list.

#ifndef SERVER_H_
#define SERVER_H_

    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <errno.h>
    #include <string.h>
    #include <sys/types.h>
    #include <time.h>
    #include <signal.h>
    #include <dirent.h>
    #include <sys/stat.h>
    #include <string.h>
    #include <sys/select.h>
    #include <stdbool.h>
    #include <dlfcn.h>
    #include <fcntl.h>


//------------Linked list for clients in room ------------------------//
//this linked list is used to store the clients in a room, it is used to
//send the positions of the tanks to the clients in the room and usually
//to manage players in the room
typedef struct client_room_tank_s {
    int client_fd;
    int live;

    float posX;
    float posY;
    int direction;
    bool close;

    struct client_room_tank_s *next;
} client_room_tank_t;


//------------Linked list for room------------------------------------//
//this linked list is used to store the rooms.
typedef struct Rooms_tank_s {
    int id_room;
    struct client_room_tank_s *client_room_tank;
    int nb_client_in_room;
    bool gameStarted;

    struct Rooms_tank_s *next;
} Rooms_tank_t;


//------------Linked list for client----------------------------------//
//this linked list stores the clients connected to the server
//there is no link between the gameplay, rooms and player, there is only
//clients connected to the server
typedef struct client_tank_s {
    int client_fd;

    bool closed;

    struct client_tank_s *next;
} client_tank_t;


//------------Main struct of the project------------------------------//
//this struct is used to store all the variables of the server
//there are used to manage the server, the clients, the rooms and the gameplay
typedef struct tank_s {
    int server_fd;
    fd_set readfds;
    struct sockaddr_in address;
    socklen_t addrlen;

    char buffer[1024];

    client_tank_t *client_tank;
    client_room_tank_t *client_room_tank;
    Rooms_tank_t *Rooms_tank;

    int numbertoremove;

    float MAP_WIDTH;
    float MAP_HEIGHT;

    int nbRoom;

} tank_t;


//------------Prototypes of server------------------------------------//
void read_client(client_tank_t *list_tank, tank_t *tank);
void loop_server(tank_t *tank);
void set_fd(tank_t *tank);
int loop_server_tank(tank_t *tank);
client_tank_t *add_node_client_tank(client_tank_t *tank, int fd_cli);
client_tank_t *remove_node_client_tank(client_tank_t *list_tank);
tank_t *init_struct_tank(char *port);
int create_socket(int port);
void parse_hit(char *buffer, tank_t *tank, int client_fd_sender, client_room_tank_t *list_client_in_room, Rooms_tank_t *list_room);
char **strToWordArray(char *str);
Rooms_tank_t *add_node_client_room(Rooms_tank_t *room, int id);
client_room_tank_t *add_node_client_room_tank(client_room_tank_t *client_room, int fd_cli);
void goToRoom(tank_t *tank, int client_fd, char *buffer);
client_room_tank_t *remove_room_client(client_room_tank_t *list_tank);
void startGame(tank_t *tank, int client_fd);
void set(tank_t *tank, int client_fd_sender, client_room_tank_t *list_client_in_room, Rooms_tank_t *list_room, char *buffer);

#endif /* !SERVER_H_ */
