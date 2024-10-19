/*
 *
 *
 */

#include "uart.h"
#include "timer.h"

void uart_init(int baud)
{
    SYSCTL_RCGCGPIO_R |= 0b00000010;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0b00000010;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R = 0b00000011;      // sets PB0 and PB1 as peripherals (page 671)
    GPIO_PORTB_PCTL_R  = 0x00000011;       // pmc0 and pmc1       (page 688)  also refer to page 650
    GPIO_PORTB_DEN_R   = 0b00000011;        // enables pb0 and pb1
    GPIO_PORTB_DIR_R   = 0b00000010;        // sets pb0 as output, pb1 as input

    //compute baud values [UART clock= 16 MHz]
    double fbrd;
    int    ibrd;

    fbrd = 16000000.0 / (16.0 * baud); // page 903
    ibrd = (int)fbrd;
    fbrd = (fbrd - ibrd) * 64 + 0.5;

    UART1_CTL_R &= 0xFFFE;      // disable UART1 (page 918)
    UART1_IBRD_R = ibrd;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = (int)fbrd;   // write fractional portion of BRD to FBRD
    UART1_LCRH_R = 0b01100000;        // write serial communication parameters (page 916) * 8bit and no parity
    UART1_CC_R   = 0x0;          // use system clock as clock source (page 939)
    UART1_CTL_R |= 0x0001;        // enable UART1

}

void uart_sendChar(char data)
{
    while(UART1_FR_R & 0x20) // wait until there is room to send data
    {

    }
    UART1_DR_R= data;   // send data


}

char uart_receive(void)
{
    char data = 0;

    while(UART1_FR_R & UART_FR_RXFE)
    {

    }

    data = (char)(UART1_DR_R & 0xFF);

    return data;


}

void uart_sendStr(const char *data)
{
    while(*data != '\0'){

        uart_sendChar(*data);
        data++;
    }
}

// Interrupt Initialization
void uart_interrupt_init()
{
    // Enable interrupts for receiving bytes through UART1
    UART1_IM_R |= 0x10;   // Enable interrupt on receive - page 924 (RXIM)

    // Find the NVIC enable register and bit responsible for UART1
    NVIC_EN0_R |= (1 << 6); // Enable UART1 interrupts (Interrupt 22 is in bit 6 of NVIC_EN0) - page 104

    // Register the interrupt handler (if not using TivaWare driverlib)
    IntRegister(INT_UART1, uart_interrupt_handler);  // Give the microcontroller the address of our interrupt handler
}

void uart_interrupt_handler()
{
    // Check the Masked Interrupt Status
    if (UART1_MIS_R & 0x10) // If RX interrupt
    {
        uart_data = (char)(UART1_DR_R & 0xFF); // Read received data
        flag = 1;  // Set flag indicating new data has been received

        // Clear the interrupt
        UART1_ICR_R |= 0x10;  // Clear RX interrupt - page 924
    }
}
