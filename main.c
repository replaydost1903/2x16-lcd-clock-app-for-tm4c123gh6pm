#include "driverlib/sysctl.h"
#include "inc/tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/sysctl.c"
#include "driverlib/interrupt.h"
#include "driverlib/interrupt.c"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/timer.c"
#include "driverlib/gpio.c"
#include "inc/hw_memmap.h"
#include "LCD.h"
#include "driverlib/uart.h"


void Timer_Init(void);
void LED_Config();
static void Send_Clock();

int flag=0;

/*
 *  hh:mm:ss
 */
uint8_t second_l=0x39;
uint8_t second_h=0x34;
uint8_t minute_l=0x39;
uint8_t minute_h=0x35;
uint8_t clock_l =0x38;
uint8_t clock_h =0x31;

void timer0_kesme()
{
    if(flag==0)
    {
        Screen_On_Off(D_RESET, C_RESET, B_RESET);
        Write_Clock_Data_to_LCD(&clock_h, &clock_l, &minute_h, &minute_l, &second_h, &second_l);
    }
    else if(flag==1)
    {
        Send_Clock();
        Write_Clock_Data_to_LCD(&clock_h, &clock_l, &minute_h, &minute_l, &second_h, &second_l);
        Screen_On_Off(D_SET, C_RESET, B_RESET);
        flag=0;
    }
    TimerIntClear(TIMER0_BASE, TIMER_A);
}



void main(void)
{
    Lcd_Port_Enable();
    Lcd_Init(I_D_RESET, S_C_SET, D_SET, C_RESET, B_RESET, S_C_SET, R_L_SET, D_L_RESET, N_SET, F_SET);
    LED_Config();
    Timer_Init();


    while(1)
    {

    }
}



void Timer_Init(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);  //timer aþaðý sayýcý
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1);

    IntMasterEnable();
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, timer0_kesme);
    TimerEnable(TIMER0_BASE, TIMER_A);
}
void LED_Config()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); // pin 1, 2, 3 output
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
}
static void Send_Clock()
{
    UARTCharPut(UART0_BASE, clock_h);
    UARTCharPut(UART0_BASE, clock_l);
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, minute_h);
    UARTCharPut(UART0_BASE, minute_l);
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, second_h);
    UARTCharPut(UART0_BASE, second_l);
    UARTCharPut(UART0_BASE, 10);
}







































