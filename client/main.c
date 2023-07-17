/*
** EPITECH PROJECT, 2023
** tank_reseau
** File description:
** main
*/

#include "client.h"

#include "client.h"

void init_client(client_t *clients)
{
    clients->port = 8080;
    clients->ip = "127.0.0.1";
    clients->buffer[0] = '\0';
}

int init_cli(client_t *clients, int client_socket, struct sockaddr_in *server_address)
{
    memset(server_address, 0, sizeof(struct sockaddr_in));
    server_address->sin_family = AF_INET;
    server_address->sin_port = htons(clients->port);
    if (inet_pton(AF_INET, clients->ip, &(server_address->sin_addr)) <= 0) {
        printf("Error: Invalid IP Address.\n");
        return 84;
    }

    if (connect(client_socket, (struct sockaddr*)server_address, sizeof(struct sockaddr_in)) < 0) {
        printf("Error: Failed to connect to server.\n");
        return 84;
    }

    return 0;
}

void receive_data(client_t *clients, int client_socket, fd_set readfds)
{
    if (FD_ISSET(client_socket, &readfds)) {
        ssize_t bytes_received = recv(client_socket, clients->buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            printf("Error: Failed to receive data from server.\n");
            return;
        }
        clients->buffer[bytes_received] = '\0';
        printf("%s", clients->buffer);

        memset(clients->buffer, 0, BUFFER_SIZE);
    }
}

void handle_events(sfRenderWindow* window, int client_socket)
{
    sfEvent event;
    while (sfRenderWindow_pollEvent(window, &event)) {
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(window);
    }
}

void draw_text(sfRenderWindow* window, sfText* text)
{
    sfRenderWindow_clear(window, sfBlack);
    sfRenderWindow_drawText(window, text, NULL);
    sfRenderWindow_display(window);
}

void boucle_cli(client_t *clients, int client_socket)
{
    sfVideoMode mode = {1920, 1080, 32};
    sfRenderWindow* window;
    sfFont* font;
    sfText* text;

    window = sfRenderWindow_create(mode, "tank.io", sfResize | sfClose, NULL);
    if (!window) {
        printf("Error: Failed to create SFML window.\n");
        return;
    }

    // Charger l'image en tant que texture
    sfTexture* texture = sfTexture_createFromFile("/home/Louis/delivery/projets_en_cours/tank.ioInC/client/media/background_menu.png", NULL);
    if (!texture) {
        printf("Error: Failed to load image texture.\n");
        sfRenderWindow_destroy(window);
        return;
    }

    // Créer un sprite à partir de la texture
    sfSprite* sprite = sfSprite_create();
    sfSprite_setTexture(sprite, texture, sfTrue);

    fd_set readfds;
    while (sfRenderWindow_isOpen(window)) {
        FD_ZERO(&readfds);
        FD_SET(client_socket, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        if (select(client_socket + 1, &readfds, NULL, NULL, NULL) == -1) {
            printf("Error: select failed.\n");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            fgets(clients->buffer, BUFFER_SIZE, stdin);
            send(client_socket, clients->buffer, strlen(clients->buffer), 0);
            memset(clients->buffer, 0, BUFFER_SIZE);
        }

        receive_data(clients, client_socket, readfds);

        handle_events(window, client_socket);

        sfRenderWindow_clear(window, sfBlack);

        // Dessiner le sprite en arrière-plan
        sfRenderWindow_drawSprite(window, sprite, NULL);

        // Dessiner les autres éléments

        sfRenderWindow_display(window);
    }

    // Libérer la mémoire
    sfSprite_destroy(sprite);
    sfTexture_destroy(texture);

    sfText_destroy(text);
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);
}


int main(int argc, char *argv[])
{
    client_t *clients = malloc(sizeof(client_t));
    init_client(clients);

    int client_socket;
    struct sockaddr_in server_address;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Error: Failed to create socket.\n");
        return 1;
    }

    if (init_cli(clients, client_socket, &server_address) == 84)
        return 84;

    boucle_cli(clients, client_socket);

    close(client_socket);
    return 0;
}
