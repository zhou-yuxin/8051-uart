#ifndef UART_H
#define UART_H

#include <8051.h>
#include <stdint.h>

// the oscillator frequency
#define UART_FOSC_HZ            11059200
// the system clock frequency
#define UART_SYSCLK_HZ          (UART_FOSC_HZ / 12)

// the interrupt vector index of uart
#define UART_INTERRUPT_NO       4

// initialize uart module,
// baud_rate should be selected carefully,
// TH1 = 256 - UART_SYSCLK_HZ / 16 / baud_rate
void uart_init(uint16_t baud_rate);

// send data on uart,
// if async, call returns immediately, and then
// sending callback will be invoked upon complete.
// otherwise wait until sent.
void uart_send(uint8_t data, uint8_t async);

// check whether a byte is received.
// return 1 if received, 0 otherwise.
uint8_t uart_available();

// get the received byte,
// if there is no data available, the result if undefined.
uint8_t uart_recv();

// set a callback function to be invoked when sent. 
void uart_set_send_callback(void (*func)());

// set a callback function to be invoked when received.
void uart_set_recv_callback(void (*func)(uint8_t data));

// sdcc requires that a prototype of the ISR must be present
// or included in the file that contains the function main().
// so the file contains main() should include this header file,
// and user shouldn't call it manually.
void uart_on_interrupt() __interrupt(UART_INTERRUPT_NO);

#endif