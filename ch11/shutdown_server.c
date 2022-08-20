//
// Created by DemonLee on 8/5/22.
//

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include "common/log.h"
#include "common/general.h"

static int count;

static void sig_int(int sig_no) {
    printf("received signal: %d\n", sig_no);
    printf("received %d datagrams.\n", count);
    exit(0);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        error_handling(stderr, "Usage: shutdown_server <Port>");
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    int ret = bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret < 0) {
        error_handling(stderr, "server socket bind failed");
    }

    ret = listen(listen_fd, LISTEN_QUEUE);
    if (ret < 0) {
        error_handling(stderr, "server socket listen failed");
    }

    signal(SIGINT, sig_int);
    // signal(SIGPIPE, sig_int);

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int conn_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);
    if (conn_fd < 0) {
        error_handling(stderr, "server accept client socket failed");
    }

    char message[MAX_LINE];
    count = 0;

    for (;;) {
        ssize_t n = read(conn_fd, message, MAX_LINE);
        if (n < 0) {
            error_logging(stderr, "read message error");
            break;
        } else if (n == 0) {
            error_logging(stderr, "client closed");
            break;
        }
        message[n] = '\0';
        printf("received %zu bytes: [%s]\n", n, message);
        count++;

        char send_line[MAX_LINE];
        sprintf(send_line, "message [%s] received.", message);

        sleep(5);

        printf("sending bytes: %s\n", send_line);
        ssize_t write_nc = send(conn_fd, send_line, strlen(send_line), 0);
        printf("send %zu bytes: %s\n", write_nc, send_line);
        if (write_nc < 0) {
            error_logging(stderr, "send message error");
            break;
        }
    }
    close(conn_fd);
    close(listen_fd);

    return 0;
}
