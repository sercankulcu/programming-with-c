/* uart.c — a driver written against the register map */
#include <stdint.h>
#include <stdbool.h>

/* ---------- register overlay ---------- */

typedef struct {
    volatile uint32_t DR;          /* 0x000 data                */
    volatile uint32_t RSR_ECR;     /* 0x004 receive status      */
    uint32_t reserved0[4];
    volatile uint32_t FR;          /* 0x018 flags               */
    uint32_t reserved1;
    volatile uint32_t ILPR;        /* 0x020                     */
    volatile uint32_t IBRD;        /* 0x024 baud, integer part  */
    volatile uint32_t FBRD;        /* 0x028 baud, fraction      */
    volatile uint32_t LCRH;        /* 0x02C line control        */
    volatile uint32_t CTL;         /* 0x030 control             */
    volatile uint32_t IFLS;        /* 0x034                     */
    volatile uint32_t IM;          /* 0x038 interrupt mask      */
    volatile uint32_t RIS;         /* 0x03C raw interrupt status*/
    volatile uint32_t MIS;         /* 0x040 masked status       */
    volatile uint32_t ICR;         /* 0x044 interrupt clear W1C */
} UART_Type;

#define UART0 ((UART_Type *)0x4000C000u)

/* Flag register bits, from the manual's table. */
#define FR_BUSY  (1u << 3)
#define FR_RXFE  (1u << 4)     /* receive FIFO empty  */
#define FR_TXFF  (1u << 5)     /* transmit FIFO full  */

/* Line control bits. */
#define LCRH_FEN        (1u << 4)
#define LCRH_WLEN_MASK  (3u << 5)
#define LCRH_WLEN_8     (3u << 5)

/* Control bits. */
#define CTL_UARTEN (1u << 0)
#define CTL_TXE    (1u << 8)
#define CTL_RXE    (1u << 9)

void uart_init(void)
{
    UART0->CTL = 0;                     /* disable before configuring */

    UART0->IBRD = 10;                   /* 115200 baud at 18.432 MHz  */
    UART0->FBRD = 54;

    /* Read-modify-write: set the word length, preserve everything else. */
    uint32_t lcrh = UART0->LCRH;
    lcrh &= ~LCRH_WLEN_MASK;
    lcrh |= LCRH_WLEN_8 | LCRH_FEN;
    UART0->LCRH = lcrh;

    UART0->CTL = CTL_UARTEN | CTL_TXE | CTL_RXE;   /* enable last */
}

void uart_putc(char c)
{
    while (UART0->FR & FR_TXFF) { }     /* poll: needs volatile */
    UART0->DR = (uint32_t)(unsigned char)c;
}

bool uart_getc(char *out)
{
    if (UART0->FR & FR_RXFE) {
        return false;                   /* nothing waiting */
    }
    *out = (char)(UART0->DR & 0xFFu);   /* reading REMOVES it from the FIFO */
    return true;
}

void uart_flush(void)
{
    while (UART0->FR & FR_BUSY) { }     /* wait for the shift register */
}

void uart_puts(const char *s)
{
    for (; *s != '\0'; s++) {
        if (*s == '\n') uart_putc('\r');
        uart_putc(*s);
    }
}
