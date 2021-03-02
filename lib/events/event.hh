#pragma once

#include "common.hh"

namespace steel {
    enum class exception_type : uint64 {
        synchronous = 0,
        interrupt = 1,
        finterrupt = 2,
        serror = 3
    };

    typedef void(*event_handler)(uint64, uint64);

    void event(exception_type type, event_handler handler);
}