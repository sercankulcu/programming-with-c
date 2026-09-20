/* protocol.c — no register touches anywhere */
#include "protocol.h"
#include <string.h>
#include <stdio.h>

#define TIMEOUT_TICKS 1000

void protocol_init(Protocol *p, const Hal *hal)
{
    memset(p, 0, sizeof *p);
    p->hal = hal;
    p->last_status = PROTO_OK;
}

static void reply(Protocol *p, const char *s)
{
    for (; *s; s++) p->hal->uart_write(p->hal->ctx, (uint8_t)*s);
}

ProtoStatus protocol_poll(Protocol *p)
{
    uint8_t byte;
    uint32_t now = p->hal->ticks(p->hal->ctx);

    while (p->hal->uart_read(p->hal->ctx, &byte)) {
        p->last_byte_tick = now;

        if (byte == '\n') {
            p->line[p->used] = '\0';
            char out[PROTO_MAX_LINE + 16];
            int n = snprintf(out, sizeof out, "%u:%s\n",
                             (unsigned)++p->lines_handled, p->line);
            if (n > 0) reply(p, out);
            p->used = 0;
            continue;
        }

        if (p->used >= PROTO_MAX_LINE - 1) {     /* the bound that matters */
            reply(p, "overflow\n");
            p->used = 0;
            p->last_status = PROTO_OVERFLOW;
            return PROTO_OVERFLOW;
        }
        p->line[p->used++] = (char)byte;
    }

    if (p->used > 0 && now - p->last_byte_tick > TIMEOUT_TICKS) {
        reply(p, "timeout\n");
        p->used = 0;
        p->last_status = PROTO_TIMEOUT;
        return PROTO_TIMEOUT;
    }
    return PROTO_OK;
}
