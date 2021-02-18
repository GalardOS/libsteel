#include "devices/mini_uart.hh"

extern void main(int argc, char** argv);

extern "C" 
void gimme_all_your_lovin(void)
{
	steel::uart_init();
	steel::uart_send_string("Platform -> Raspberry PI 3\r\n");

	main(0, nullptr);
}
