//
// Created by DemonLee on 5/24/22.
//

#include "datetime.h"
#include "log.h"
#include <time.h>
#include <string.h>

char *now(char *time_buffer) {
    if (NULL == time_buffer) {
        error_logging(stderr, "time_buffer is null");
        return NULL;
    }
    time_t timer;
    struct tm *tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(time_buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    return time_buffer;
}
