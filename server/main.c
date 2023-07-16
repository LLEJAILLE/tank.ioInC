/*
** EPITECH PROJECT, 2023
** tank_reseau
** File description:
** main
*/

//------------Explaination for this file----------------------------------------//
// This file contains the main function of the server. It is used to initialise
// the server and to launch the loop of the server.

#include "server.h"

//------------Functions to catch ctrl+C-------------------------------//

void handle_signal(int signal)
{
    // if ctrl+C is pressed, the server is shutting down and all the code below is executed
    if (signal == SIGINT) {
        printf("Server is shutting down.\n");
        exit(0);
    }
}


//------------Main function-------------------------------------------//

int main(int ac, char **av)
{

    // check if the number of arguments is correct (./server_tank <port>)
    if (ac != 2) {
        printf("Usage: ./server_tank <port>\n");
        return (84);
    }

    // initialise the struct tank_t
    tank_t *tank = init_struct_tank(av[1]);

    // catch ctrl+C
    signal(SIGINT, handle_signal);


    // loop of the server
    if (loop_server_tank(tank) == 84)
        return (84);

    return (0);
}

