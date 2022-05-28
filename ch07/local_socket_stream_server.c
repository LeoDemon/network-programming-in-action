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
        error_handling(stderr, "Usage: local_socket_stream_server <local_path>");
    }

    int listen_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        error_handling(stderr, "socket create failed");
    }

    struct sockaddr_un client_addr, server_addr;
    socklen_t client_len = sizeof(client_addr);

    char *local_path = argv[1];
    unlink(local_path);
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, local_path);

    if (bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        error_handling(stderr, "bind socket failed");
    }

    if (listen(listen_fd, LISTEN_QUEUE) < 0) {
        error_handling(stderr, "listen socket failed");
    }

    int conn_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);
    if (conn_fd < 0) {
        error_handling(stderr, "accept client connection failed");
    }

    char buf[MAX_LINE];
    char send_line[MAX_LINE];
    while (1) {
        bzero(buf, sizeof(buf));
        ssize_t read_rt = read(conn_fd, buf, MAX_LINE);
        if (read_rt == 0) {
            printf("client quit\n");
            break;
        } else if (read_rt < 0) {
            error_handling(stderr, "read msg error");
        }
        printf("read_rt==[%lu]\n", read_rt);
        if (buf[read_rt - 1] == '\n') {
            buf[read_rt - 1] = '\0';
        }
        printf("receive msg: [%s]\n", buf);

        // sleep(3); // for client mocking: server terminated prematurely
        sprintf(send_line, "we received your msg: [%s]", buf);
        size_t send_len = strlen(send_line);
        if (write(conn_fd, send_line, send_len) != send_len) {
            error_handling(stderr, "write msg failed");
        }
    }

    close(conn_fd);
    close(listen_fd);

    return 0;
}
