/**
 * TCP_SERVER_T_ copied from:
 * https://github.com/raspberrypi/pico-examples/blob/master/pico_w/wifi/access_point/picow_access_point.c
 */

#ifndef ACCESS_POINT_H_
#define ACCESS_POINT_H_

#include <string.h>

#include "dhcpserver.h"
#include "dnsserver.h"
#include "pico/cyw43_arch.h"

typedef struct TCP_SERVER_T_ {
    struct tcp_pcb *server_pcb;
    bool complete;
    ip_addr_t gw;
    async_context_t *context;
} TCP_SERVER_T;

bool tcp_server_open(void *, const char *);
void tcp_server_close(TCP_SERVER_T *);

#endif