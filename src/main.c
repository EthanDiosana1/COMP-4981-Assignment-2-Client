#include "arguments.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stringTools.h>
#include <sys/socket.h>
#include <unistd.h>

// #define LOCALHOST_IP "127.0.0.1\0"
// #define DEFAULT_PORT 3000
// #define MINIMUM_PORT 3000
// #define MAXIMUM_PORT 10000
#define PORT_BASE 10

/**
 * @brief displays the usage of this app.
 */
void display_usage(void);

/**
 * @brief Creates a socket connection to the given address.
 * @param ip the ip to connect to
 * @param port the port to connect to
 */
int socket_connect_to(const char *ip, uint16_t port);

/**
 * @brief converts a port from char to uint16_t
 * @param portStr the port string
 */
uint16_t convert_port(const char *portStr);

int main(int argc, char *argv[])
{
    struct arguments arguments;    // Holds the arguments info.
    uint16_t         port;         // the port to connect to.
    const char      *ip;           // the IP to connect to.

    arguments.argc = argc;
    arguments.argv = argv;

    if(arguments.argc < 3)
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
    return EXIT_SUCCESS;
}

uint16_t convert_port(const char *portStr)
{
    char         *endptr;
    unsigned long portUlong;

    portUlong = strtoul(portStr,
                        &endptr,
                        PORT_BASE    // Base 10
    );

    if(*endptr != '\0')
    {
        fprintf(stderr, "Error: Invalid port number format.\n");
        return EXIT_FAILURE;
    }

    if(portUlong == UINT16_MAX)
    {
        fprintf(stderr, "Error: Port number out of range.\n");
        return EXIT_FAILURE;
    }

    return (uint16_t)portUlong;
}
