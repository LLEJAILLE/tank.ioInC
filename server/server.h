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


//------------Linked list for client----------------------------------//
typedef struct client_tank_s {
    int client_fd;

    int posX;
    int posY;

    int live;

    bool closed;

    struct client_tank_s *next;
} client_tank_t;


//------------Main struct of the project------------------------------//
typedef struct tank_s {
    int server_fd;
    fd_set readfds;
    struct sockaddr_in address;
    socklen_t addrlen;

    char buffer[1024];

    client_tank_t *client_tank;

    int numbertoremove;

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

#endif /* !SERVER_H_ */
