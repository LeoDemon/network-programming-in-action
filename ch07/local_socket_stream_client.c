//
// Created by DemonLee on 5/27/22.
//

#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "common/log.h"
#include "common/general.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        error_handling(stderr, "Usage: local_socket_stream_client <local_path>");
    }

    int sock_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        error_logging(stderr, "create socket failed");
    }

    struct sockaddr_un server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, argv[1]);

    int connect_rt = connect(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (connect_rt < 0) {
        error_logging(stderr, "connect server failed");
    }

    char send_line[MAX_LINE];
    char recv_line[MAX_LINE];
    bzero(send_line, sizeof(send_line));

    while (fgets(send_line, MAX_LINE, stdin) != NULL) {
        // int nbytes = sizeof(send_line); // using strlen() here, do not use sizeof()
        size_t nbytes = strlen(send_line);
        ssize_t wbytes = write(sock_fd, send_line, nbytes);
        printf("wbytes(send_line)=[%lu]\n", wbytes);
        if (wbytes != nbytes) {
            error_logging(stderr, "write msg failed");
        }

        bzero(recv_line, sizeof(recv_line));
        ssize_t read_rt = read(sock_fd, recv_line, MAX_LINE);
        if (read_rt == 0) {
            printf("server terminated prematurely\n");
            break;
        } else if (read_rt < 0) {
            error_handling(stderr, "read msg error");
        }

        fputs(recv_line, stdout);
        fputs("\n", stdout);
    }

    close(sock_fd);

    return 0;
}