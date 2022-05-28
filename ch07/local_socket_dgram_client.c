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
        error_handling(stderr, "Usage: local_socket_dgram_client <local_path>");
    }

    int sock_fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        error_logging(stderr, "create socket failed");
    }

    struct sockaddr_un server_addr, client_addr;
    bzero(&client_addr, sizeof(client_addr));
    client_addr.sun_family = AF_LOCAL;

    char tmp_file[] = "/var/tmp/tmp-XXXXXX";
//    unlink(tmp_file);
    mktemp(tmp_file);
    printf("tmp_file==[%s]\n", tmp_file);
    strcpy(client_addr.sun_path, tmp_file);
//    char *tmp_file = tmpnam(NULL);
//    printf("tmp_file==[%s]\n", tmp_file);
//    strcpy(client_addr.sun_path, tmp_file);

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, argv[1]);

    if (bind(sock_fd, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) {
        error_handling(stderr, "bind socket failed");
    }

    char send_line[MAX_LINE];
    char recv_line[MAX_LINE];
    bzero(send_line, sizeof(send_line));

    while (fgets(send_line, MAX_LINE, stdin) != NULL) {
        size_t nbytes = strlen(send_line);
        if (send_line[nbytes - 1] == '\n') {
            send_line[nbytes - 1] = '\0';
        }
        nbytes = strlen(send_line);
        if (sendto(sock_fd, send_line, nbytes, 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) != nbytes) {
            error_handling(stderr, "send msg failed");
        }

        ssize_t recv_rt = recvfrom(sock_fd, recv_line, MAX_LINE, 0, NULL, NULL);
        if (recv_rt < 0) {
            error_handling(stderr, "recvfrom msg error");
        }
        recv_line[recv_rt] = '\0';
        fputs(recv_line, stdout);
        fputs("\n", stdout);
    }

    close(sock_fd);

    return 0;
}
