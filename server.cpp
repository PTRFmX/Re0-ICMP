/**
 * @file server.cpp
 * @author Mingxuan Fan
*/

#include "server.h"

/**
 * @brief Initialize TCP socket / SOCK_STREAM on the server 
 * @param port          The port which the socket is bind with
 * @param max_client    Max client connections
 * 
 * @return none
*/
static void initializeServer(unsigned int port, unsigned int max_client) {

    // Initialize sockets
    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sock < 0) {
        die(__LINE__, "Unable to create server socket");
    }

    // Initialize server data
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(LOCALHOST);
    server_addr.sin_port = htons(port);

    // Bind sockets with port
    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        die(__LINE__, "Unable to bine socket on port %s", port);
    }

    // Listen on the socket
    if (listen(server_sock, max_client) < 0) {
        die(__LINE__, "Unable to listen on socket on port %s", port);
    }

    // Accept client requests
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size);
    if (client_sock < 0) {
        die(__LINE__, "Unable to accept client request");
    }

    // After connected, send data to client
    char str[] = "Hello";
    write(client_sock, str, sizeof(str));

    // Close the sockets
    close(client_sock);
    close(server_sock);
}

/**
 * @brief A quit function in case of any exceptions happen
 * @param line      The line which causes the error
 * @param format    A string that specify error ouput format & content
 * 
 * @return none
*/
static void die(int line, const char* format, ...) {
    va_list vargs;
    va_start(vargs, format);
    fprintf(stderr, "%d: ", line);
    vfprintf(stderr, format, vargs);
    fprintf(stderr, ".\n");
    va_end(vargs);
    exit(EXIT_FAILURE);
}

/**
 * @brief Prints the correct usage of the program
 * @param name      name of the program
 * 
 * @return none
*/
void printHelp(const char* name) {
    printf("Usage: %s [pn]\n", name);
    printf("-p  indicates the port which the socket is bind with\n");
    printf("-n  indicates max client connections allowed\n");
}

/**
 * @brief Main function
 * @param argc      number of params
 * @param argv      params
 * 
 * @return status value
*/
int main(int argc, char** argv) {

    // Initialize values
    int opt;
    unsigned int port = DEFAULT_PORT, max_client = DEFAULT_MAX_CLIENT;

    // Get user options
    while ((opt = getopt(argc, argv, "pn")) != -1) {
        switch (opt) {
            case 'p':
                try {
                    port = std::stoi(optarg);
                } catch (...) {
                    die(__LINE__, "Port must be a number");
                }
                if (port <= 0 || port >= (1 << PORT_EXP_LIMIT)) {
                    die(__LINE__, "Invalid port number");
                }
                break;
            case 'n':
                try {
                    max_client = std::stoi(optarg);
                } catch (...) {
                    die(__LINE__, "Max client connections must be a number");
                }
                if (max_client < 0) {
                    die(__LINE__, "Max client connections must be positive");
                }
                break;
            default:
                printHelp(argv[0]);
                exit(EXIT_FAILURE);
                break;
        }
    }

    // Initialize socket server
    initializeServer(port, max_client);

    return 0;
}