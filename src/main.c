#include "arguments.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stringTools.h>
#include <sys/socket.h>
#include <unistd.h>

// My libraries
#include "serverTools.h"

// Colors
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_BG_BLACK "\x1b[40m"

// #define LOCALHOST_IP "127.0.0.1\0"
// #define DEFAULT_PORT 3000
// #define MINIMUM_PORT 3000
// #define MAXIMUM_PORT 10000
#define CLOSE_CONNECTION_KEYWORD "exit"
#define DEFAULT_BUFFER_SIZE 1024
#define NUM_ARGS 3

/**
 * @brief displays the usage of this app.
 */
void display_usage(void);

/**
 * @brief Sends the size of the message to the server.
 * @param message_size the size of the message in bytes
 *
 */
int send_message_size(int client_fd, size_t message_size);

/**
 * @brief Sends the message to the server.
 * @param client_fd the client file descriptor
 * @param buffer the buffer containing the message
 * @param message_size the size of the message
 */
int send_message(int client_fd, const char *buffer, size_t message_size);

int socket_connect_to(const char *ip, uint16_t port);

int main(int argc, char *argv[])
{
    struct arguments arguments;    // Holds the arguments info.
    uint16_t         port;         // the port to connect to.
    const char      *ip;           // the IP to connect to.

    arguments.argc = argc;
    arguments.argv = argv;

    if(arguments.argc < NUM_ARGS)
    {
        display_usage();
        return EXIT_FAILURE;
    }

    // Goal: Create a remote shell app.
    // Create a server that is able to accept multiple clients.
    // The clients should be able to run commands through the server.
    // It should read and execute the commands from the client socket
    // dup2 -> redirects the stdout to the socket
    // use execv();
    // "Confirm that it exists" using access();

    // On the client side:
    // You don't need much to the client because it's just writing the commands
    // to the server and awaiting a response.

    // Make sure to go through D'Arcy's examples on Google Drive.

    ip   = arguments.argv[1];
    port = convert_port(arguments.argv[2]);

    printf("Client options: ip: %s, port: %hu\n", ip, port);

    socket_connect_to(ip, port);

    return EXIT_SUCCESS;
}

void display_usage(void)
{
    printf("Usage: ./main <ip> <port>\n");
}

int socket_connect_to(const char *ip, uint16_t port)
{
    int                sockfd;    // The client socket.
    char               buffer[DEFAULT_BUFFER_SIZE];
    struct sockaddr_in serveraddr;

    if(!ip)
    {
        perror("ip cannot be null\n");
        return EXIT_FAILURE;
    }

    if(!port)
    {
        perror("port cannot be null\n");
        return EXIT_FAILURE;
    }

    // Create the socket.
    sockfd = socket(AF_INET,        // int domain
                    SOCK_STREAM,    // int type
                    0               // int protocol
    );

    // Check if socket failed.
    if(sockfd == -1)
    {
        perror("socket creation failed\n");
        return EXIT_FAILURE;
    }

    // Assign IP, port.
    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port        = htons(port);

    if(connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) != 0)
    {
        perror("connection to server failed\n");
        return EXIT_FAILURE;
    }

    printf("Connecting to:"
           "\n\tip: %s"
           "\n\tport: %hu"
           "\n",
           ip,
           port);

    // Write to the socket.
    while(1)
    {
        size_t buffer_size;

        printf(ANSI_BG_BLACK ANSI_COLOR_GREEN "> " ANSI_COLOR_RESET);
        fgets(buffer, sizeof(buffer), stdin);

        // Remove newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        buffer_size = strlen(buffer);

        // Send message size to the server.
        send_message_size(sockfd, buffer_size);

        // Send the message
        send_message(sockfd, buffer, buffer_size);

        // Check for exit
        if(strcmp(buffer, CLOSE_CONNECTION_KEYWORD) == 0)
        {
            printf("Exiting...\n");
            break;
        }
    }

    close(sockfd);

    return EXIT_SUCCESS;
}

int send_message_size(int client_fd, size_t message_size)
{
    if(send(client_fd, &message_size, sizeof(size_t), 0) == -1)
    {
        fprintf(stderr, "send() failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int send_message(int client_fd, const char *buffer, size_t message_size)
{
    if(buffer == NULL)
    {
        fprintf(stderr, "buffer is null\n");
        return EXIT_FAILURE;
    }

    if(send(client_fd, buffer, message_size, 0) == -1)
    {
        fprintf(stderr, "send() failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
