#include "devices/mini_uart.hh"
#include "events/event.hh"

extern void main(int argc, char** argv);

typedef void (*ctor)();
extern "C" ctor start_ctors;
extern "C" ctor end_ctors;

extern "C" 
void gimme_all_your_lovin(void)
{
    steel::uart_init();
    steel::uart_send_string("Platform -> Raspberry PI 3\r\n");

    steel::__event_initialize();

    // Call all global constructors
    for(ctor* constructor = &start_ctors; constructor != &end_ctors; constructor++) {
        (*constructor)();
    }

    main(0, nullptr);
}
