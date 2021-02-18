#include "mini_uart.hh"

void main(int argc, char** argv) {
    steel::uart_send_string("Hello World\r\n");
}