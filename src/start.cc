#include "drivers/auxuart.hh"

extern void main(int argc, char** argv);

typedef void (*ctor)();
extern "C" ctor start_ctors;
extern "C" ctor end_ctors;

extern "C" void _start() {
    // Call the constructor of global
    //for(ctor* i = &start_ctors; i != &end_ctors; i++) {
    //    (*i)();
    //}

    steel::auxuart::initialize();

    // Call the main function 
    /// TODO: think about the argc and argv 
    main(0, nullptr);

    while(true);
}