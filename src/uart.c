#include "uart.h"
#include "io.h"
#include "gpio.h"


void uart_init() {
    out32bit(AUX_ENABLES, 1); //enable UART1
    out32bit(AUX_MU_IER_REG, 0);
    out32bit(AUX_MU_CNTL_REG, 0);
    out32bit(AUX_MU_LCR_REG, 3); //8 bits
    out32bit(AUX_MU_MCR_REG, 0);
    out32bit(AUX_MU_IER_REG, 0);
    out32bit(AUX_MU_IIR_REG, 0xC6); //disable interrupts
    out32bit(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));
    gpio_set_pull(14, Pull_None);
    gpio_set_function(14, GPIO_FUNCTION_ALT5);
    gpio_set_pull(15, Pull_None);
    gpio_set_function(15, GPIO_FUNCTION_ALT5);
    out32bit(AUX_MU_CNTL_REG, 3); //enable RX/TX
}

unsigned int uart_isWriteReady() 
{
    unsigned int status;
    in32bit(AUX_MU_LSR_REG , &status);
    return status & (0x1 << 5); //bit 5
}

void uart_putc(unsigned char c)
{
    while (!uart_isWriteReady()); 
    out32bit(AUX_MU_IO_REG, (unsigned int)c);
}

void uart_puts(const char *str)
{    
    while (*str) {
       if (*str == '\n') uart_putc('\r');
       uart_putc(*str++);
    }
}

char uart_getc(void)
{
    return 'c';

}