#include <uart.h>

void on_recv(uint8_t data) {
    if ('a' <= data && data <= 'z') {
        data = data - 'a' + 'A';
    }
    else if ('A' <= data && data <= 'Z') {
        data = data - 'A' + 'a';
    }
    uart_send(data, 0);
}

void main() {
    uart_init(57600);
    uart_set_recv_callback(on_recv);
    while (1);
}