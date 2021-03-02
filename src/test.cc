#include "devices/mini_uart.hh"

void main(int argc, char** argv) {
    steel::uart_send_string("Hello World\r\n");

    //asm volatile ("brk #7");

    steel::uart_send_string("Ameisin interrupts\r\n");
}