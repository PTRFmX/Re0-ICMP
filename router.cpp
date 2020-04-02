/**
 * @file router.cpp
 * @author Mingxuan Fan
*/

#include <iostream>
#include <string>
#include "server.h"
#include "constants.h"

void handleHomePage(int client_sock) {
    char res_msg[] = "Welcome";
    send(client_sock, res_msg, strlen(res_msg), 0);
}

void handleTestPage(int client_sock) {
    char res_msg[] = "Test";
    send(client_sock, res_msg, strlen(res_msg), 0);
}

void handle404Page(int client_sock) {
    char res_msg[] = "404 NOT FOUND";
    send(client_sock, res_msg, strlen(res_msg), 0);
}

void handleRoute(int client_sock, std::string const & url) {
    if (url == "/") handleHomePage(client_sock);
    else if (url == "/test") handleTestPage(client_sock);
    else handle404Page(client_sock);
}