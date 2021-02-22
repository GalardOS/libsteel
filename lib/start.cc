#include "devices/mini_uart.hh"

extern void main(int argc, char** argv);

typedef void (*ctor)();
extern "C" ctor start_ctors;
extern "C" ctor end_ctors;

extern "C" 
void gimme_all_your_lovin(void)
{
    steel::uart_init();
    steel::uart_send_string("Platform -> Raspberry PI 3\r\n");

    // Call all global constructors
    for(ctor* constructor = &start_ctors; constructor != &end_ctors; constructor++) {
        (*constructor)();
    }

    main(0, nullptr);
}
