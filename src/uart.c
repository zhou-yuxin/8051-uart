#include <uart.h>
#include <stdlib.h>

static void (*g_on_send)() = NULL;
static void (*g_on_recv)(uint8_t data) = NULL;
static volatile __bit g_sending = 0;
static volatile __bit g_recved = 0;
static uint8_t g_recv_byte;

void uart_init(uint16_t baud_rate) {
    TMOD = (TMOD & 0x0f) | 0x20;
    TH1 = 256 - UART_SYSCLK_HZ / 16 / baud_rate;
    TL1 = TH1;
    SCON = 0x50;
    PCON |= 0x80;
    ES = 1;
    ET1 = 0;
    TR1 = 1;
    EA = 1;
}

void uart_send(uint8_t data, uint8_t async) {
    while (g_sending && !TI);
    g_sending = 1;
    TI = 0;
    SBUF = data;
    if (!async) {
        while (g_sending && !TI);
    }
}

uint8_t uart_available() {
    return g_recved;
}

uint8_t uart_recv() {
    g_recved = 0;
    return g_recv_byte;
}

void uart_set_send_callback(void (*func)()) {
    g_on_send = func;
}

void uart_set_recv_callback(void (*func)(uint8_t data)) {
    g_on_recv = func;
}

void uart_on_interrupt() __interrupt(UART_INTERRUPT_NO) {
    if (RI) {
        RI = 0;
        g_recv_byte = SBUF;
        g_recved = 1;
        if (g_on_recv) {
            g_on_recv(g_recv_byte);
        }
    }
    if (TI) {
        TI = 0;
        g_sending = 0;
        if (g_on_send) {
            g_on_send();
        }
    }
}