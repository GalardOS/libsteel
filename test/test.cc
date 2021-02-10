#include "../src/drivers/auxuart.hh"

void main(int argc, char** argv) {
    steel::auxuart::write("Hello World\r\n");
}