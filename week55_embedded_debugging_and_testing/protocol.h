/* protocol.h — the logic under test */
#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "hal.h"

#define PROTO_MAX_LINE 64

typedef enum { PROTO_OK, PROTO_OVERFLOW, PROTO_TIMEOUT } ProtoStatus;

typedef struct {
    const Hal  *hal;
    char        line[PROTO_MAX_LINE];
    uint8_t     used;
    uint32_t    last_byte_tick;
    uint32_t    lines_handled;
    ProtoStatus last_status;
} Protocol;

void        protocol_init(Protocol *p, const Hal *hal);
ProtoStatus protocol_poll(Protocol *p);
#endif
