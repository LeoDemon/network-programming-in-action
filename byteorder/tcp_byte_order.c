//
// Created by DemonLee on 10/13/22.
//

#include <stdio.h>
#include "tcp_byte_order.h"

void display_message(struct message *msg) {
    printf("msg.buf=[%s], msg.length=[%d]\n", msg->buf, msg->len);
}
