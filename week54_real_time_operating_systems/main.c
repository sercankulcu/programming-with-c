int main(void)
{
    init();
    for (;;) {
        read_sensors();       /*  2 ms */
        update_control();     /*  1 ms */
        log_to_flash();       /* 50 ms — the problem */
        handle_ui();          /*  1 ms */
    }
}
