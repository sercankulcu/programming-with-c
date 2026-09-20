/* superloop.c */
#include <stdint.h>
#include "board.h"          /* uart_puts, put_int, tick_count from week 51 */

#define LOG_COST_TICKS 50   /* the slow operation */

static uint32_t sensor_runs, control_runs, log_runs, ui_runs;
static uint32_t sensor_worst_gap, last_sensor_tick;

static void busy_wait(uint32_t ticks)
{
    uint32_t start = tick_count;
    while (tick_count - start < ticks) { }
}

static void read_sensors(void)
{
    uint32_t now = tick_count;
    uint32_t gap = now - last_sensor_tick;
    if (gap > sensor_worst_gap) sensor_worst_gap = gap;
    last_sensor_tick = now;
    sensor_runs++;
    busy_wait(2);
}

static void update_control(void) { control_runs++; busy_wait(1); }
static void write_log(void)      { log_runs++;     busy_wait(LOG_COST_TICKS); }
static void handle_ui(void)      { ui_runs++;      busy_wait(1); }

int main(void)
{
    board_init();
    uart_puts("\n=== superloop ===\n");

    uint32_t started = tick_count;
    while (tick_count - started < 2000) {     /* 2 seconds */
        read_sensors();
        update_control();
        write_log();
        handle_ui();
    }

    uart_puts("in 2000 ticks:\n");
    uart_puts("  sensor runs      : "); put_int((int32_t)sensor_runs);
    uart_puts("\n  control runs     : "); put_int((int32_t)control_runs);
    uart_puts("\n  log runs         : "); put_int((int32_t)log_runs);
    uart_puts("\n  WORST sensor gap : "); put_int((int32_t)sensor_worst_gap);
    uart_puts(" ticks  (wanted 10)\n");
    uart_puts("\n  the log's 50 ticks are in front of every sensor read\n");

    for (;;) { }
}
