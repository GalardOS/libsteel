#include "event.hh"

#include "common.hh" 
#include "devices/mini_uart.hh"
 
enum class exception_type : uint64 {
    synchronous = 0,
    interrupt = 1,
    finterrupt = 2,
    serror = 3
};

void(*events[4])(uint64, uint64);

extern "C" void exception_entry(uint64 type, uint64 esr, uint64 elr) {
    steel::uart_send_string("entered an exception: ");

    /// TODO: do this only when debug is enabled
    if(type == 0) {
        steel::uart_send_string("synchronous\r\n");
    } else if(type == 1) {
        steel::uart_send_string("interrupt\r\n");
    } else if(type == 2) {
        steel::uart_send_string("finterrupt\r\n");
    } else {
        steel::uart_send_string("serror\r\n");
    }

    // Call the event given by the user if it exists, if not,
    // do nothing
    if(events[type] != nullptr)
        events[type](esr, elr);
}

extern "C" void setup_vector_table();

namespace steel {
    void __event_initialize() {
        // Function that setups the vector table used by the cpu
        // to know where to jump in case of exceptions
        setup_vector_table();

        // In case that the event table has garbage values
        // that could lead to jumps to nowhere, add nullptr so
        // the exception entry can know if it needs to jump or 
        // not
        for(uint32 i = 0; i < 4; i++)
            events[i] = nullptr;
    }

    void event(exception_type type, event_handler handler) {
        auto index = static_cast<uint64>(type);
        events[index] = handler;
    }
}