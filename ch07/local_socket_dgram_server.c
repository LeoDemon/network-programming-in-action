//
// Created by DemonLee on 5/27/22.
//

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include "common/log.h"
#include "common/general.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        error_handling(stderr, "Usage: local_socket_dgram_server <local_path>");
    }

    int socket_fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        error_handling(stderr, "socket create failed");
    }

    struct sockaddr_un client_addr, server_addr;
    socklen_t client_len = sizeof(client_addr);

    char *local_path = argv[1];
    unlink(local_path);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, local_path);

    if (bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        error_handling(stderr, "bind socket failed");
    }

    char buf[MAX_LINE];
    char send_line[MAX_LINE];
    while (1) {
        bzero(buf, sizeof(buf));
        ssize_t read_rt = recvfrom(socket_fd, buf, MAX_LINE, 0, (struct sockaddr *) &client_addr, &client_len);
        if (read_rt < 0) {
            error_handling(stderr, "receive msg error");
        }
        printf("receive msg: [%s]\n", buf);

        sprintf(send_line, "we received your msg: %s", buf);
        size_t send_len = strlen(send_line);
        if (sendto(socket_fd, send_line, send_len, 0, (struct sockaddr *) &client_addr, client_len) != send_len) {
            error_handling(stderr, "send msg failed");
        }
    }

    close(socket_fd);

    return 0;
}
