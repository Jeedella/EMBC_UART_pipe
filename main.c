/* 
 * File:   main.c
 * Author: Monica Ignat
 *
 * Created on April 3rd, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include <stdbool.h>
#include "config.h"
#include "uart.h"
#include "lcd.h"
#include "uartjb.h"


#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF      


/* ------------------------------------------------------------ */
/*						Configuration Bits		                */
/* ------------------------------------------------------------ */


// Device Config Bits in  DEVCFG1:	
#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	XT
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_2

// Device Config Bits in  DEVCFG2:	
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1
#define TENSMICR 100
int main(int argc, char** argv) 
{
    unsigned char strTxt[cchRxMax], strTxti[cchRxMax];
    unsigned char cch, cchi;
    bool commandMode = false;

    //UARTJB and LCD basic initializations
    UARTJB_InitPoll(9600); 
    UARTJB_PutString("\r\n JB_UAER \r\n");;
    UART_InitPoll(9600); 
    LCD_Init();
    //send a text to UART terminal on PC using UARTJB library
    UART_PutString("Basys MX3 #1 \n\r");
    LCD_WriteStringAtPos(" UART Demo Pmod", 0, 0);
    UART_PutString("UART Demo Pmod \n\r");
    LCD_DisplayClear();
    //when sending a text file, make sure the content ends with CR+LF and your terminal is also setup accordingly, to transmit both characters
    //otherwise, the file sending will not function correctly
    while(1){  
        
        cch = UARTJB_GetStringPoll(strTxt);
        DelayAprox10Us( TENSMICR );
        if(cch > 0)
        {
          //LCD_DisplayClear();
          LCD_WriteStringAtPos(strTxt, 0, 0);
          LCD_WriteStringAtPos(" UART Demo Pmod", 1, 0);
          //send the content of the file back to UART to be visualized
          UART_PutString(strTxt);
    //      UART_PutString("\r\n");
        } 
        DelayAprox10Us( TENSMICR );
        cchi = UART_GetStringPoll(strTxti);
        
        if(cchi > 0)
        {
            UARTJB_PutString(strTxti);
 //          UARTJB_PutString("\r\n");
            DelayAprox10Us( TENSMICR );
            if(commandMode)
            {
                if(strcmp(strTxti,"---") == 0)
                    commandMode = false;
            }
            else
                if(strcmp(strTxti,"$$$") == 0) { 
                    commandMode = true;
                }
                                   
        } 
       DelayAprox10Us( TENSMICR );
    }
    return (1);    
}
