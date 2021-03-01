#include "event.hh"

#include "common.hh" 
#include "devices/mini_uart.hh"
 
enum class exception_type : uint64 {
    synchronous = 0,
    interrupt = 1,
    finterrupt = 2,
    serror = 3
};

extern "C" void exception_entry(uint64 type, uint64 esr, uint64 elr) {
    auto etype = static_cast<exception_type>(type);

    steel::uart_send_string("entered an exception: ");

    switch (etype) {
        case exception_type::synchronous: {
            steel::uart_send_string("synchronous\r\n");
        } break;

        case exception_type::interrupt: { 
            steel::uart_send_string("interrupt\r\n");
        } break;

        case exception_type::finterrupt: {
            steel::uart_send_string("finterrupt\r\n");
        } break;

        case exception_type::serror: { 
            steel::uart_send_string("serror\r\n");
        } break;
    }
}

extern "C" void setup_vector_table();

namespace steel {
    void __event_initialize() {
        setup_vector_table();
    }
}