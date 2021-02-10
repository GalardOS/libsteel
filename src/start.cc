#include "mini_uart.hh"

extern "C" 
void gimme_all_your_lovin(void)
{
	uart_init();
	uart_send_string("Hello, world!\r\n");

	while (1) {
		uart_send(uart_recv());
	}
}
