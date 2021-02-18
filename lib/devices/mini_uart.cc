extern "C" {
    #include "utils.h"
}

#include "common.hh"

#define PBASE 0x3F000000

#define GPFSEL1         ((volatile uint32*)(PBASE+0x00200004))
#define GPSET0          ((volatile uint32*)(PBASE+0x0020001C))
#define GPCLR0          ((volatile uint32*)(PBASE+0x00200028))
#define GPPUD           ((volatile uint32*)(PBASE+0x00200094))
#define GPPUDCLK0       ((volatile uint32*)(PBASE+0x00200098))

#define AUX_ENABLES     ((volatile uint32*)(PBASE+0x00215004))
#define AUX_MU_IO_REG   ((volatile uint32*)(PBASE+0x00215040))
#define AUX_MU_IER_REG  ((volatile uint32*)(PBASE+0x00215044))
#define AUX_MU_IIR_REG  ((volatile uint32*)(PBASE+0x00215048))
#define AUX_MU_LCR_REG  ((volatile uint32*)(PBASE+0x0021504C))
#define AUX_MU_MCR_REG  ((volatile uint32*)(PBASE+0x00215050))
#define AUX_MU_LSR_REG  ((volatile uint32*)(PBASE+0x00215054))
#define AUX_MU_MSR_REG  ((volatile uint32*)(PBASE+0x00215058))
#define AUX_MU_SCRATCH  ((volatile uint32*)(PBASE+0x0021505C))
#define AUX_MU_CNTL_REG ((volatile uint32*)(PBASE+0x00215060))
#define AUX_MU_STAT_REG ((volatile uint32*)(PBASE+0x00215064))
#define AUX_MU_BAUD_REG ((volatile uint32*)(PBASE+0x00215068))

namespace steel {
    void uart_init ( void ) {
        unsigned int selector;

        selector = *GPFSEL1;
        selector &= ~(7<<12);                   // clean gpio14
        selector |= 2<<12;                      // set alt5 for gpio14
        selector &= ~(7<<15);                   // clean gpio15
        selector |= 2<<15;                      // set alt5 for gpio15
        *GPFSEL1 = selector;

        *GPPUD = 0;
        delay(150);
        *GPPUDCLK0 = (1 << 14)|(1 << 15);
        delay(150);
        *GPPUDCLK0 = 0;

        *AUX_ENABLES = 1;                   //Enable mini uart (this also enables access to it registers)
        *AUX_MU_CNTL_REG = 0;               //Disable auto flow control and disable receiver and transmitter (for now)
        *AUX_MU_IER_REG = 0;                //Disable receive and transmit interrupts
        *AUX_MU_LCR_REG = 3;                //Enable 8 bit mode
        *AUX_MU_MCR_REG = 0;                //Set RTS line to be always high
        *AUX_MU_BAUD_REG = 270;             //Set baud rate to 115200

        *AUX_MU_CNTL_REG = 3;               //Finally, enable transmitter and receiver
    }

    void uart_send (char c) {
        while(!(*AUX_MU_LSR_REG & 0x20));
        *AUX_MU_IO_REG = c;
    }

    char uart_recv (void) {
        while(!(*AUX_MU_LSR_REG & 0x01));
        return *AUX_MU_IO_REG & 0xFF;
    }

    void uart_send_string(const char* str) {
        for (int i = 0; str[i] != '\0'; i ++) {
            uart_send((char)str[i]);
        }
    }

    

}