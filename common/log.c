//
// Created by DemonLee on 5/23/22.
//

#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>
#include "log.h"
#include "general.h"
#include "datetime.h"

void error_logging(FILE *fsd, const char *msg) {
    char *errmsg = NULL;
    if (0 != errno) {
        errmsg = strerror(errno);
    }
    char timeStr[30];
    bzero(timeStr, sizeof(timeStr));
    now(timeStr);

    fputs("[", fsd);
    fputs(timeStr, fsd);
    fputs("]: ", fsd);
    fputs(msg, fsd);
    fputs(": ", fsd);
    fputs(errmsg, fsd);
    fputc('\n', fsd);
}

void error_handling(FILE *fsd, const char *msg) {
    error_logging(fsd, msg);
    int status = errno;
    if (0 == status) {
        status = UNKNOWN_ERROR;
    }
    exit(status);
}

void logging(const char *msg) {
    char timeStr[30];
    bzero(timeStr, sizeof(timeStr));

    now(timeStr);

    printf("[%s]: %s\n", timeStr, msg);
}
