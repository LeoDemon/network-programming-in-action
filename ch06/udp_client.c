//
// Created by DemonLee on 5/25/22.
//

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common/log.h"

//#define MAX_LINE 9217
#define MAX_LINE 4096

int main(int argc, char **argv) {
    if (argc < 3) {
        error_handling(stderr, "Usage: udp_client <IP> <Port>");
    }

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    socklen_t server_len = sizeof(server_addr);
    struct sockaddr *reply_addr = malloc(server_len);

    char send_line[MAX_LINE], recv_line[MAX_LINE + 1];
    socklen_t reply_addr_len;
    ssize_t receive_len;
    while (fgets(send_line, MAX_LINE, stdin) != NULL) {
        size_t send_len = strlen(send_line);
        if (send_line[send_len - 1] == '\n') {
            send_line[send_len - 1] = '\0';
        }

        /**
         * macOS 12.4
         * running `sudo sysctl -w net.inet.udp` to get the max length for udp sending limit
           net.inet.udp.checksum: 1
           net.inet.udp.maxdgram: 9216
           net.inet.udp.recvspace: 786896
           net.inet.udp.log_in_vain: 0
           net.inet.udp.blackhole: 0
           net.inet.udp.pcbcount: 61
           net.inet.udp.randomize_ports: 1
         * the default size is 9216, when the send message length greater 9216，will occur an error: Message too long
         */
        printf("client sending: %s\n", send_line);
        ssize_t rt = sendto(socket_fd, send_line, send_len, 0, (struct sockaddr *) &server_addr, server_len);
        if (rt < 0) {
            error_handling(stderr, "client sendto failed");
        }
        printf("client sending bytes: %zu\n", rt);

        reply_addr_len = 0;
        receive_len = recvfrom(socket_fd, recv_line, MAX_LINE, 0, reply_addr, &reply_addr_len);
        if (receive_len < 0) {
            error_handling(stderr, "client receive failed");
        }
        recv_line[receive_len] = '\0';
        fputs(recv_line, stdout);
        fputs("\n", stdout);

        if (strcmp("bye", send_line) == 0) {
            break;
        }
    }
    free(reply_addr);
    close(socket_fd);
}