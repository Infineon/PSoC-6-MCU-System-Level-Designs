/*****************************************************************************
* File Name: CryptoAES.h
* 
* Version 1.20
*
* Description: This file contains macro definitions and user defined data type  
* used in main_cm4.c file  
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
#include "cy_crypto_config.h"

#define AES128_KEY_LENGTH  (uint32_t)(16u)
#define MAX_MESSAGE_SIZE   (uint32_t)(16u)
#define AES128_ENCRYPTION_LENGTH (uint32_t)(16u)
#define CRYPTO_BLOCKING             true

/* Structure where Crypto internal states and variables are stored  */
cy_stc_crypto_context_t cryptoScratch;
cy_stc_crypto_context_aes_t cryptoAES;

/* Key used for AES encryption*/
uint8_t AES_Key[AES128_KEY_LENGTH]={0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0xFF,0xEE,\
                                     0xDD,0xCC,0xBB,0xAA,0xAA,0xBB,0xCC,0xDD,};

/* Data type definition to store the data in text format as well as in hexadecimal
   format*/
typedef union datablock
{
    char text[16];
    uint32_t packedtext[4];
}datablock_t;
