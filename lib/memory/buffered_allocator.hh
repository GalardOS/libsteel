#pragma once 

#include "common.hh"

namespace steel {
    class buffered_allocator {
    public:
        buffered_allocator(void* start, uint32 size);
        ~buffered_allocator();

        void* allocate(uint32 size);
        void deallocate(void* ptr);
    private:
        // Information about the buffer
        uint64* buffer_start;
        uint32 buffer_element_count;
    protected:
    };
}