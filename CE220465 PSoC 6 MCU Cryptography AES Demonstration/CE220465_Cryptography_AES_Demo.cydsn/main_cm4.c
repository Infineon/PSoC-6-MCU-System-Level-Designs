/*********************************************************************************
* File Name: main_cm4.c
*
* Version: 1.20
*
* Description: This file contains the code for initializing and starting of the 
* crypto client. The client provides the configuration data for AES encryption 
* and requests the crypto server (cm0p core) to run the cryptographic operation
*
* This example prompts the user to enter a message. The message is encrypted 
* using AES-128 algorithm. The encryption result, key used for the encryption and
* the decrypted message are displayed on the UART terminal.
*
* Related Document: CE220465.pdf
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
#include "CryptoAES.h"
#include <stdio.h>
#include <string.h>

#define SCREEN_HEADER "\r\n__________________________________________________"\
                  "____________________________\r\n*\t\tCE220465 PSoC 6 "\
                  "Cryptography: AES Demonstration\r\n*\r\n*\tIf you are able to "\
                  "read the text the terminal connection is configured \n\r*"\
                  "\tcorrectly\r\n*\r\n*\tBaud Rate : 9600 bps\r\n*\n"\
                  "\r\n__________________________________________________"\
                  "____________________________\r\n"
                
#define SCREEN_HEADER1 "\r\n__________________________________________________"\
                  "____________________________\r\n" 

void DisplayMenu(void);
void PrintData(uint8_t*);

datablock_t ciphertext;
char message[MAX_MESSAGE_SIZE+1],Decoded_Message[MAX_MESSAGE_SIZE+1],temp;
uint32_t message_size;

/**********************************************************************************
* Function Name: int main()
***********************************************************************************
* Summary:
* main function initializes the crypto driver and enables the crypto hardware.
* enables the UART SCB for accepting the input message from the user and displays
* the encryption and decryption results on the UART terminal
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
    Cy_Crypto_Init(&cryptoConfig,&cryptoScratch);
    
    /* Wait for Crypto Block to be available */
    Cy_Crypto_Sync(CRYPTO_BLOCKING);
    
    /* Enable Crypto Hardware */
    Cy_Crypto_Enable();
    Cy_Crypto_Sync(CRYPTO_BLOCKING);
        
    DisplayMenu();

    for(;;)
    {
        message_size=0;
        Cy_SCB_UART_PutString(UART_HW,"\r\n\nEnter the message( not"\
                                      " more than 16 characters):\r\n\n");
        
        /* Read the user input message till the user press the "ENTER" Key
        (ASCII value ='0x0D')*/
        Cy_SCB_UART_GetArrayBlocking(UART_HW,&temp,1);
        while(temp!=0x0D)
        { 
            message[message_size]=temp;
            message_size++;
            UART_GetArrayBlocking(&temp,1);
        }
        message[message_size]='\0';
        
        /*Check whether message length exceeds 16 characters*/
        if(message_size > MAX_MESSAGE_SIZE)
        {
            Cy_SCB_UART_PutString(UART_HW,"\r\n\nMessage size exceeds"\
            " 16 characters! Please enter a message of not more "\
            "\r\nthan 16 characters \r\n");
        }
        
        else
        {
            /* Initializes the AES opeeration by setting key and key length */
            Cy_Crypto_Aes_Init((uint32_t*)AES_Key,CY_CRYPTO_KEY_AES_128,&cryptoAES);
                
            /* Wait for Crypto Block to be available */                
            Cy_Crypto_Sync(CRYPTO_BLOCKING);
                
            /* Perform AES ECB Encryption mode of operation */
            Cy_Crypto_Aes_Ecb_Run(CY_CRYPTO_ENCRYPT,ciphertext.packedtext,\
                                     (uint32_t*)message,&cryptoAES);
            Cy_Crypto_Sync(CRYPTO_BLOCKING);
            Cy_SCB_UART_PutString(UART_HW,"\r\n\nKey used for Encryption:\r\n");
            PrintData(AES_Key);            
            Cy_SCB_UART_PutString(UART_HW,"\r\nResult of Encryption:\r\n");
            PrintData((uint8_t*)ciphertext.text);
            
             /* Perform AES ECB Decryption mode of operation */
            Cy_Crypto_Aes_Ecb_Run(CY_CRYPTO_DECRYPT,(uint32_t*)Decoded_Message\
                                     ,ciphertext.packedtext,&cryptoAES);
            Cy_Crypto_Sync(CRYPTO_BLOCKING);
            Decoded_Message[message_size]='\0';               
            /* Print the decrypted message on the UART terminal */
            Cy_SCB_UART_PutString(UART_HW,"\r\nResult of Decryption:\r\n\n");
            Cy_SCB_UART_PutString(UART_HW,Decoded_Message);
            Cy_SCB_UART_PutString(UART_HW,SCREEN_HEADER1);
            
            
        }
    }
}

/***********************************************************************************
* Function Name: DisplayMenu()
************************************************************************************
* Summary: Function used to display the main menu to the user 
*
* Parameters:
*  None
*
* Return:
*  void
*
************************************************************************************/
void DisplayMenu()
{
    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    Cy_SCB_UART_PutString(UART_HW, "\x1b[2J\x1b[;H");
    
    Cy_SCB_UART_PutString(UART_HW,SCREEN_HEADER);
}

/***********************************************************************************
* Function Name: PrintData()
************************************************************************************
* Summary: Function used to display the data in hexadecimal format
*
* Parameters:
*  uint8_t*
*  Pointer to location of data to be printed
*
* Return:
*  void
*
************************************************************************************/
void PrintData(uint8_t* text)
{
    datablock_t printdata;
    
    memcpy(printdata.text,text,AES128_ENCRYPTION_LENGTH);
    Cy_SCB_UART_PutString(UART_HW,"\r\n");
    char print[10];
    for(uint32 i=0;i<AES128_ENCRYPTION_LENGTH;i++)
    {
        sprintf(print,"0x%2X ",printdata.text[i]);
        Cy_SCB_UART_PutString(UART_HW,print);   
        
    }
    Cy_SCB_UART_PutString(UART_HW,"\r\n");
    
}

/* [] END OF FILE */
