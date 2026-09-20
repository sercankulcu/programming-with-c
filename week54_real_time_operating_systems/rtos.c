/* rtos.c */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "board.h"

#define PRIO_SENSOR  4      /* 10 ms deadline: highest */
#define PRIO_CONTROL 3
#define PRIO_UI      2
#define PRIO_LOG     1      /* no deadline: lowest     */

typedef struct { uint32_t tick; int32_t value; } Reading;

static QueueHandle_t     readings;      /* sensor  → control */
static QueueHandle_t     log_queue;     /* control → log     */
static SemaphoreHandle_t stats_mutex;   /* a MUTEX: inheritance */

static uint32_t sensor_runs, control_runs, log_runs;
static uint32_t sensor_worst_gap, last_sensor_tick;

static void busy_wait(uint32_t ticks)
{
    TickType_t start = xTaskGetTickCount();
    while (xTaskGetTickCount() - start < ticks) { }   /* real work */
}

/* --- highest priority: a hard 10 ms period --- */
static void sensor_task(void *arg)
{
    (void)arg;
    TickType_t last_wake = xTaskGetTickCount();

    for (;;) {
        uint32_t now = (uint32_t)xTaskGetTickCount();
        uint32_t gap = now - last_sensor_tick;

        if (xSemaphoreTake(stats_mutex, pdMS_TO_TICKS(5)) == pdTRUE) {
            if (last_sensor_tick != 0 && gap > sensor_worst_gap) {
                sensor_worst_gap = gap;
            }
            sensor_runs++;
            xSemaphoreGive(stats_mutex);
        }
        last_sensor_tick = now;

        Reading r = { .tick = now, .value = (int32_t)(now % 100) };
        xQueueSend(readings, &r, 0);          /* never block the sensor */

        busy_wait(2);
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(10));   /* exact period */
    }
}

/* --- blocks on the queue: costs nothing while waiting --- */
static void control_task(void *arg)
{
    (void)arg;
    Reading r;
    for (;;) {
        if (xQueueReceive(readings, &r, portMAX_DELAY) == pdTRUE) {
            if (xSemaphoreTake(stats_mutex, portMAX_DELAY) == pdTRUE) {
                control_runs++;
                xSemaphoreGive(stats_mutex);
            }
            busy_wait(1);
            xQueueSend(log_queue, &r, 0);     /* drop if the log is behind */
        }
    }
}

/* --- lowest priority: preempted constantly, and that is correct --- */
static void log_task(void *arg)
{
    (void)arg;
    Reading r;
    for (;;) {
        if (xQueueReceive(log_queue, &r, portMAX_DELAY) == pdTRUE) {
            if (xSemaphoreTake(stats_mutex, portMAX_DELAY) == pdTRUE) {
                log_runs++;
                xSemaphoreGive(stats_mutex);
            }
            busy_wait(50);                    /* the slow operation */
        }
    }
}

static void report_task(void *arg)
{
    (void)arg;
    vTaskDelay(pdMS_TO_TICKS(2000));

    uart_puts("\n=== FreeRTOS, same work ===\n");
    uart_puts("in 2000 ticks:\n");
    uart_puts("  sensor runs      : "); put_int((int32_t)sensor_runs);
    uart_puts("\n  control runs     : "); put_int((int32_t)control_runs);
    uart_puts("\n  log runs         : "); put_int((int32_t)log_runs);
    uart_puts("\n  WORST sensor gap : "); put_int((int32_t)sensor_worst_gap);
    uart_puts(" ticks  (wanted 10)\n");

    uart_puts("\nstack high-water marks (words unused):\n");
    uart_puts("  sensor : ");
    put_int((int32_t)uxTaskGetStackHighWaterMark(NULL));
    uart_puts("\n");
    uart_puts("  free heap: ");
    put_int((int32_t)xPortGetFreeHeapSize());
    uart_puts(" bytes\n");

    vTaskSuspend(NULL);
}

int main(void)
{
    board_init();

    readings    = xQueueCreate(8, sizeof(Reading));
    log_queue   = xQueueCreate(8, sizeof(Reading));
    stats_mutex = xSemaphoreCreateMutex();      /* MUTEX, not a semaphore */

    xTaskCreate(sensor_task,  "sensor",  256, NULL, PRIO_SENSOR,  NULL);
    xTaskCreate(control_task, "control", 256, NULL, PRIO_CONTROL, NULL);
    xTaskCreate(log_task,     "log",     256, NULL, PRIO_LOG,     NULL);
    xTaskCreate(report_task,  "report",  256, NULL, PRIO_UI,      NULL);

    vTaskStartScheduler();                       /* never returns */
    for (;;) { }
}
