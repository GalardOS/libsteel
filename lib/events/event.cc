#include "event.hh"

#include "common.hh" 
#include "devices/mini_uart.hh"
 
enum class exception_type : uint64 {
    synchronous_exception = 0,
    interrupt_exception = 1,
    finterrupt_exception = 2,
    serror_exception = 3
};

extern "C" void exception_entry(uint64 type, uint64 esr, uint64 elr) {
    auto etype = static_cast<exception_type>(type);

    switch (etype) {
        case exception_type::synchronous_exception: {
            steel::uart_send_string("synchronous exception\r\n");
        } break;

        case exception_type::interrupt_exception: { 
            steel::uart_send_string("interrupt exception\r\n");
        } break;

        case exception_type::finterrupt_exception: {
            steel::uart_send_string("finterrupt exception\r\n");
        } break;

        case exception_type::serror_exception: { 
            steel::uart_send_string("serror exception\r\n");
        } break;
    }
}

extern "C" void setup_vector_table();

namespace steel {
    void __event_initialize() {
        setup_vector_table();
    }
}