//
// Created by DemonLee on 10/13/22.
//

#ifndef NETWORK_PROGRAMMING_IN_ACTION_TCP_BYTE_ORDER_H
#define NETWORK_PROGRAMMING_IN_ACTION_TCP_BYTE_ORDER_H

#include <arm/types.h>

struct message {
    u_int32_t len;
    u_int32_t type;
    char buf[256];
};

void display_message(struct message *msg);

#endif //NETWORK_PROGRAMMING_IN_ACTION_TCP_BYTE_ORDER_H
