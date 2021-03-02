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

    if(events[type] != nullptr)
        events[type](esr, elr);
}

extern "C" void setup_vector_table();


namespace steel {
    void __event_initialize() {
        setup_vector_table();
    }

    void event(exception_type type, event_handler handler) {
        auto index = static_cast<uint64>(type);
        events[index] = handler;
    }
}