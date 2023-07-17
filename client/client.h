/*
** EPITECH PROJECT, 2023
** tank.ioInC
** File description:
** client
*/

#ifndef CLIENT_H_
#define CLIENT_H_

    #include <SFML/Graphics.h>
    #include <SFML/Network.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <arpa/inet.h>

typedef struct client_s {
    int port;
    char *ip;
    char buffer[1024];
    
} client_t;


#define BUFFER_SIZE 1024

#endif /* !CLIENT_H_ */
