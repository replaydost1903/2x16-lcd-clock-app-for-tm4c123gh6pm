#include "LCD.h"





void Lcd_Port_Enable(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(LCDPORT,0xFF);
}
void Send_Thirty_Hex(void)
{
    delayMs(2);
    Send_Command(0x03);
}
void Send_Command(uint8_t data)
{
    GPIOPinWrite(LCDPORT,RS, 0x00);
    GPIOPinWrite(LCDPORT, DB4|DB5|DB6|DB7, (data & 0xF0));
    GPIOPinWrite(LCDPORT, E, E);
    delayMs(2);
    GPIOPinWrite(LCDPORT, E, 0x00);

    delayMs(2);

    GPIOPinWrite(LCDPORT,RS, 0x00);
    GPIOPinWrite(LCDPORT, DB4|DB5|DB6|DB7, (data & 0x0F) << 4);
    GPIOPinWrite(LCDPORT, E, E);
    delayMs(2);
    GPIOPinWrite(LCDPORT, E, 0x00);
}
void Send_Data(uint8_t data)
{
    GPIOPinWrite(LCDPORT,RS, RS);
    GPIOPinWrite(LCDPORT, DB4|DB5|DB6|DB7, (data & 0xF0));
    GPIOPinWrite(LCDPORT, E, E);
    delayMs(2);
    GPIOPinWrite(LCDPORT, E, 0x00);
    GPIOPinWrite(LCDPORT,RS, 0x00);

    delayMs(2);

    GPIOPinWrite(LCDPORT,RS, RS);
    GPIOPinWrite(LCDPORT, DB4|DB5|DB6|DB7, (data & 0x0F) << 4);
    GPIOPinWrite(LCDPORT, E, E);
    delayMs(2);
    GPIOPinWrite(LCDPORT, E, 0x00);
    GPIOPinWrite(LCDPORT,RS, 0x00);
}
void Lcd_Init(uint8_t I_D,uint8_t S,uint8_t D,uint8_t C,uint8_t B,uint8_t S_C,uint8_t R_L,uint8_t D_L,uint8_t N,uint8_t F)
{

    Send_Thirty_Hex();
    Send_Thirty_Hex();
    Send_Thirty_Hex();

    Select_Function(D_L,N,F);
    Screen_On_Off(D,C,B);
    Clean_Screen();
    Select_Login_Mode(I_D,S);
}
void Clean_Screen(void)
{
    Send_Command(0x01);
}
void Cursor_Back_to_Top(void)
{
    Send_Command(0x02);
}
void Select_Login_Mode(uint8_t I_D,uint8_t S)
{
    Send_Command((0x1<<2)|(I_D)|(S));
}
void Screen_On_Off(uint8_t D,uint8_t C,uint8_t B)
{
    Send_Command((D)|(C)|(B)|(0x1<<3));
}
void Slide_Cursor(uint8_t S_C,uint8_t R_L)
{
    Send_Command((R_L)|(S_C)|(0x1<<4));
}
void Select_Function(uint8_t D_L,uint8_t N,uint8_t F)
{
    Send_Command((D_L)|(N)|(F)|(0x1<<5));
}
void Write_Word_On_the_LCD_Screen_Only_String(char *s)
{
    while(*s != '\0')
    {
        Send_Data(*s);
        ++s;
    }
}
void Write_Word_On_the_LCD_Screen_Only_Array(char *array)
{

}
void Puts_to_x_y(uint8_t x,uint8_t y)
{
    if(x==1)
    {
            Cursor_Back_to_Top();
            uint8_t i;
            for(i=0;i<y-1;i++)
            {
                Slide_Cursor(S_C_RESET,R_L_SET);
            }
    }
    else if(x==2)
    {
            Cursor_Back_to_Top();
            uint8_t i;
            for(i=0;i<y+39;i++)
            {
                Slide_Cursor(S_C_RESET,R_L_SET);
            }
    }
}
void Write_Clock_Data_to_LCD(uint8_t *clock_h,uint8_t *clock_l,uint8_t *minute_h,uint8_t *minute_l,uint8_t *second_h,uint8_t *second_l)
{

    Send_Data(*clock_h);
    Send_Data(*clock_l);
    Send_Data(0x3A);
    Send_Data(*minute_h);
    Send_Data(*minute_l);
    Send_Data(0x3A);
    Send_Data(*second_h);
    Send_Data(*second_l);
    Cursor_Back_to_Top();

    ++(*second_l);
    if((*second_l)==0x3A)
    {
        ++(*second_h);
        (*second_l)=0x30;

        if((*second_h) == 0x36)
        {
           ++(*minute_l);
           (*second_h)=0x30;

           if((*minute_l) == 0x3A)
           {
               ++(*minute_h);
               (*minute_l)=0x30;

               if((*minute_h) == 0x36)
               {
                   ++(*clock_l);
                   (*minute_h)=0x30;

                   if((*clock_l) == 0x3A)
                   {
                       ++(*clock_h);
                       (*clock_l)=0x30;
                   }
                   else if(((*clock_l) == 0x34) & ((*clock_h) == 0x32))
                   {
                       (*clock_h)=0x30;
                       (*clock_l)=0x30;
                   }
               }
           }
        }
    }
}

































