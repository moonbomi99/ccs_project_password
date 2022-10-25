//*****************************************************************************



//



// keypadReading.c to read keypad



// line 41   for (k=0; k<5000; k++);       // avoid chattering



//  에서   k의 최대치를 변화시키면서, 스위치의 chattering 현상이 어떻게 감소하는지 살피시요



//



//*****************************************************************************







#include <KeypadSetup.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/sysctl.c"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"







#define CHATTERING_LIMIT  5000
static unsigned char g_char = 0;
char keyLetter[4][4]={
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' },
};


char password[10]={'0','0','0','0'}; //초기 비밀번호
char input[10]; //입력받는 값
int onoff=0; //금고가 열려있는지 여부
char changeword[10]={'A','B','C','D'}; //비밀번호 변경 코드
char gobasic[10]={'D','C','B','A'}; //비밀번호 초기화 코드



char keyPadRead(void){

    char key =0;
    int i, j, k, value, vvalue;

    for (i=0; i<4; i++)

    {
        GPIOPinWrite(GPIO_PORTA_BASE, 0xF0, ~(0x10<<i));
        for (j=0; j<4; j++)
        {
//            value = GPIOPinRead(GPIO_PORTB_BASE,0x01<<j);

            value = GPIOIntStatus(GPIO_PORTB_BASE,false) & (0x01<<j);

            for (k=0; k<CHATTERING_LIMIT; k++);       // avoid chattering

            if (value != 0x00){

                key = keyLetter[i][j];

            }

            GPIOIntClear(GPIO_PORTB_BASE,value);
        }
    }

    return key;

}


void UartSetup(void)

{

    // Enable the GPIO Peripheral used by the UART.

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Configure GPIO Pins for UART mode.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Initialize the UART for console I/O.

    UARTStdioConfig(0, 115200, 16000000);

}

int main(void)
{
    volatile uint32_t ui32Loop;

#if 0

    //

    // Enable the GPIO port that is used for the on-board LED.

    //

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //

    // Check if the peripheral access is enabled.

    //

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))

    {

    }

    //

    // Enable the GPIO pin for the LED (PF3).  Set the direction as output, and

    // enable the GPIO pin for digital function.

    //

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
#endif

    KeypadSetup();

//    GPIOPinWrite(GPIO_PORTA_BASE, 0xF0, 0xF0);

    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4);

    UartSetup();

    // Hello!

    UARTprintf("ENTER THE PASSWORD(*: DONE) : \n");

    int open=0;

    int i=0;

    while(1)

    {//비밀번호는 4자리수

        g_char=keyPadRead();
        if(g_char!=0){

            if(g_char!='*'){
                input[i]=g_char;
                UARTprintf("%c ",input[i]);

                i++;


                if(i>=4){

                    i=0;

                }

                }

            //UARTprintf("\n")

            if(g_char=='*'){

                UARTprintf("*\n");

                //UARTprintf("i= %d\n",i);
                UARTprintf("ENTERTED PASSWORD: %s \n",input);

                //UARTprintf("%s \n\n",password);

                if(onoff==0){//닫혀있을 때

                    if(strcmp(password,input)==0){ //입력한 값이 비밀번호와 같을 때

                        onoff=1;

                        UARTprintf("OPEN\n\n");

                        }

                    else if(strcmp(gobasic,input)==0){//초기화 시키고 싶을 때

                       UARTprintf("PASSWORD RESET\n");

                       strcpy(password,"0000");

                    }

                    else{

                        //비번 틀림

                        UARTprintf("!!PASSWORD ERROR!!\nTRY AGAIN \n");

                        UARTprintf("ENTER THE PASSWORD(*: DONE) : \n");

                        //i=0;

                      }

                    i=0;
                }

                else if(onoff==1){

                    //열려있을때
                   if(strcmp(input,changeword)==0){

                       //i=0;

                       //strcpy(input,"    ");

                       open=1;

                       UARTprintf("ENTER NEW PASSWORD: \n");

                   }

                   else if(strcmp(input,"#   ")==0){

                       UARTprintf("CLOSED\n");

                       onoff=0;

                   }

                   else if(open==1){

                       strcpy(password,input);

                       open=0;

                       UARTprintf("PASSWORD CHANGED\n\n");
                   }

                    i=0;

                    strcpy(input,"    ");

                }

            }

                    }

        }


    return 0;

}
