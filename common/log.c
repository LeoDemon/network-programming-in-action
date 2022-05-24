//
// Created by DemonLee on 5/23/22.
//

#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>
#include "log.h"

void error_logging(FILE *fsd, const char *msg) {
    char *errmsg = NULL;
    if (0 != errno) {
        errmsg = strerror(errno);
    }
    fputs(msg, fsd);
    fputs(": ", fsd);
    fputs(errmsg, fsd);
    fputc('\n', fsd);
}

void error_handling(FILE *fsd, const char *msg) {
    error_logging(fsd, msg);
    exit(errno);
}
