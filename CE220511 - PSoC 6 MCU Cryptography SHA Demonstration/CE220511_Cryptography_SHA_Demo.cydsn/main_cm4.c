/*********************************************************************************
* File Name: main_cm4.c
*
* Version: 1.10
*
* Description: This file contains the code for initializing and starting of the 
* crypto client. The client provides the configuration data for generating message
* digest using SHA algorithm and requests the crypto server (cm0p core) to run the
* cryptographic operation
* This example prompts the user to enter a message. The message digest is generated 
* using SHA-1 algorithm. The message digest is displayed on the UART terminal.
*
* Related Document: CE220511.pdf
*
* Hardware Dependency: CY8CKIT-062-BLE PSoC 6 BLE Pioneer Kit
*
*******************************************************************************
* Copyright (2017), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software (“EULA”).
*
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress 
* reserves the right to make changes to the Software without notice. Cypress 
* does not assume any liability arising out of the application or use of the 
* Software or any product or circuit described in the Software. Cypress does 
* not authorize its products for use in any products where a malfunction or 
* failure of the Cypress product may reasonably be expected to result in 
* significant property damage, injury or death (“High Risk Product”). By 
* including Cypress’s product in a High Risk Product, the manufacturer of such 
* system or application assumes all risk of such use and in doing so agrees to 
* indemnify Cypress against all liability.
*******************************************************************************/
#include "project.h"
#include "cy_crypto_config.h"
#include "CryptoSHA.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_HEADER "\r\n__________________________________________________"\
                  "____________________________\r\n*\t\tCE220511 PSoC 6 "\
                  "Cryptography: SHA Demonstration\r\n*\r\n*\tIf you are able to "\
                  "read the text the terminal connection is configured \n\r*"\
                  "\tcorrectly\r\n*\r\n*\tBaud Rate : 9600 bps\r\n*"\
                  "\r\n__________________________________________________"\
                  "____________________________\r\n"
                
#define SCREEN_HEADER1 "\r\n\n__________________________________________________"\
                  "____________________________\r\n" 

DataBlock_t data,hash;
void PrintData(void);

/**********************************************************************************
* Function Name: int main()
***********************************************************************************
* Summary:
* main function initializes the crypto driver and enables the crypto hardware.
* enables the UART SCB for accepting the input message from the user and displays
* the message digest on the UART terminal
*
* Parameters:
*  None
*
* Return:
*  int
*
***********************************************************************************/

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    
    /* Configure the UART SCB */
    Cy_SCB_UART_Init(UART_HW, &UART_config, &UART_context);
   
    /* enable the UART SCB hardware*/
    Cy_SCB_UART_Enable(UART_HW);
    
     /*Initialization of Crypto Driver */
    Cy_Crypto_Init(&cryptoConfig,&context);
    
    /* Wait for Crypto Block to be available */
    Cy_Crypto_Sync(CRYPTO_BLOCKING);
    
    /* Enable Crypto Hardware */
    Cy_Crypto_Enable();
    Cy_Crypto_Sync(CRYPTO_BLOCKING);
    
    char message[MAX_MESSAGE_SIZE],temp;
    uint32_t messagesize;
    
    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    Cy_SCB_UART_PutString(UART_HW, "\x1b[2J\x1b[;H");
    
    Cy_SCB_UART_PutString(UART_HW,SCREEN_HEADER);
    
    for(;;)
    {
        
        Cy_SCB_UART_PutString(UART_HW,"\r\n\nEnter the message:\r\n");
        messagesize=0;
        Cy_SCB_UART_GetArrayBlocking(UART_HW,&temp,1);
        while(temp!=0x0D)
        {
            message[messagesize]=temp;
            messagesize++;
            Cy_SCB_UART_GetArrayBlocking(UART_HW,&temp,1);
        }
        message[messagesize]='\0';
        if(messagesize > MAX_MESSAGE_SIZE)
        {
            Cy_SCB_UART_PutString(UART_HW,"\r\n\nMessage length exceeds 100 characters!!!"\
            " Please enter a shorter message\r\nor edit the macro MAX_MESSAGE_SIZE defined"\
            " in CryptoSHA.h to suit your\r\n message size\r\n"); 
        }
        else
        {
            memcpy(data.text,message,messagesize);
        
            /* Perform SHA Hash function*/
            Cy_Crypto_Sha_Run(data.packedtext,messagesize,hash.packedtext,\
                          CY_CRYPTO_MODE_SHA256,&contextSHA);
        
            Cy_Crypto_Sync(CRYPTO_BLOCKING);
            Cy_SCB_UART_PutString(UART_HW,"\r\n\nHash Value for the message:\r\n\n");
            PrintData();
        }
       
        
    }
}

/***********************************************************************************
* Function Name: PrintData()
************************************************************************************
* Summary: Function used to display the data in hexadecimal format
*
* Parameters:
*  None
*
* Return:
*  void
*
************************************************************************************/
void PrintData()
{    
    char printvalue[4];
    for(int i=0;i<MESSAGE_DIGEST_SIZE;i++)
    {
        sprintf(printvalue,"0x%2X ",hash.text[i]);
        Cy_SCB_UART_PutString(UART_HW,printvalue);
    }
    Cy_SCB_UART_PutString(UART_HW,SCREEN_HEADER1);
    
}

/* [] END OF FILE */
