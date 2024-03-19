#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// My libraries
#include "serverTools.h"

// Colors
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_BG_BLACK "\x1b[40m"

#define MIN_PORT 3000
#define MAX_PORT 10000
#define CLOSE_CONNECTION_KEYWORD "exit"
#define DEFAULT_BUFFER_SIZE 1024
#define NUM_ARGS 3

/**
 * @brief Displays the usage of this app.
 */
void display_usage(void);

/**
 * @brief Connects the client to a given domain.
 * @param ip the ip to connect to
 * @param port the port to connect to
 */
int socket_connect_to(const char *ip, uint16_t port);

int main(int argc, char *argv[])
{
    uint16_t    port;    // the port to connect to.
    const char *ip;      // the IP to connect to.

    if(argc < NUM_ARGS)
    {
        display_usage();
        return EXIT_FAILURE;
    }

    ip   = argv[1];
    port = convert_port(argv[2]);

    // If the ip is not a valid ip...
    if(!is_valid_ip(ip))
    {
        fprintf(stderr, "invalid IPv4 address: %s\n", ip);
        display_usage();
        return EXIT_FAILURE;
    }

    // If the port is less than the min...
    if(port < MIN_PORT || port > MAX_PORT)
    {
        fprintf(stderr, "port must be between 3000 and 10000 inclusive.\n");
        display_usage();
        return EXIT_FAILURE;
    }

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
    serveraddr.sin_addr.s_addr = inet_addr(ip);
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
        char   *message      = NULL;
        ssize_t message_size = 0;
        int     send_result;

        printf(ANSI_BG_BLACK ANSI_COLOR_GREEN "> " ANSI_COLOR_RESET);

        fgets(buffer, sizeof(buffer), stdin);

        // Check for exit
        if(strcmp(buffer, CLOSE_CONNECTION_KEYWORD) == 0)
        {
            printf("Exiting...\n");
            break;
        }

        // Send the message to the server
        send_result = send_message(sockfd, buffer);

        // If send encountered an error, continue
        if(send_result == EXIT_FAILURE)
        {
            continue;
        }

        // Receive the server response
        if(receive_message(sockfd, &message, message_size) == EXIT_FAILURE)
        {
            fprintf(stderr, "receive_message() failed\n");
        }

        printf("Received: %s\n", message);

        free(message);
    }

    close(sockfd);

    return EXIT_SUCCESS;
}
