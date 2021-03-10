#include "cpu_functions.hh"

namespace steel {
    void instruction_sync_barrier() {
        asm volatile ("isb");
    }
}