#include "buffered_allocator.hh"

struct meta_block {
    uint32 size_in_blocks;
    uint32 status;
} __attribute__((packed));

namespace steel {
    buffered_allocator::buffered_allocator(void* start, uint32 size) 
        : buffer_start(static_cast<uint64*>(start)),
          buffer_element_count(size / sizeof(uint64))
    {
    }

    buffered_allocator::~buffered_allocator() {
        // Zero all the buffer memory
        for(int i = 0; i < buffer_element_count; i++)
            buffer_start[i] = 0;
    }

    void* buffered_allocator::allocate(uint32 size) {
       uint32 index = 0;

        // Calculate the number of blocks to store the data, as
        // the size cant be 0, is manually set to 1 if necessary
        uint32 size_in_blocks = size / 8;
        if(size_in_blocks == 0) {
            size_in_blocks = 1;
        }

        while(true) {
            meta_block* meta = (meta_block*)(buffer_start + index);

            if(meta->status == 0 && meta->size_in_blocks >= size_in_blocks) {
                // Update the free control block to full and set the next
                // empty meta block
                ((meta_block*)(buffer_start + index))->size_in_blocks = size_in_blocks;
                ((meta_block*)(buffer_start + index))->status = 1;
                
                /// TODO: this only makes sense on the first allocations because
                ///       the size is set using the memory size instead of the next 
                ///       meta block
                ((meta_block*)(buffer_start + index + size_in_blocks + 1))->size_in_blocks = buffer_element_count - size_in_blocks - 1;
                ((meta_block*)(buffer_start + index + size_in_blocks + 1))->status = 0;

                return buffer_start + index + 1;
            } else {
                index += meta->size_in_blocks + 1;
            }
        }


        return nullptr;
    }

    void buffered_allocator::deallocate(void* ptr) {
        // Point to the control block
        meta_block* control_block = static_cast<meta_block*>(ptr);
        control_block--;

        // Set the status to free
        control_block->status = 0;
    }
}