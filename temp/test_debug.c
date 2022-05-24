//
// Created by DemonLee on 5/24/22.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "common/datetime.h"

int main(int argc, char **argv) {
    char buffer[30];
    memset(buffer, '\0', sizeof(buffer));
    printf("time1: [%s]\n", now(buffer));

    usleep(1000);

    memset(buffer, '\0', sizeof(buffer));
    printf("time2: [%s]\n", now(buffer));
}
