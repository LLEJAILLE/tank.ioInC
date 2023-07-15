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
    if (signal == SIGINT) {
        printf("Server is shutting down.\n");
        exit(0);
    }
}


//------------Main function-------------------------------------------//

int main(int ac, char **av)
{
    if (ac != 2) {
        printf("Usage: ./server_tank <port>\n");
        return (84);
    }
    tank_t *tank = init_struct_tank(av[1]);
    signal(SIGINT, handle_signal);

    if (loop_server_tank(tank) == 84)
        return (84);

    return (0);
}

