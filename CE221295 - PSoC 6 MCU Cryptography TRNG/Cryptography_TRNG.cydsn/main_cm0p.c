/*****************************************************************************
* File Name: main_cm0.c
*
* Version: 1.00
*
* Description: This example project demonstrates the use of an Crypto PDL for 
*              generating a true random number. 
*              This is the source code for cm0p core. cm0p core runs crypto
*              server, where all crypto instructions are executed.
*
* Related Document: 
*      CE221295_PSoC_6_MCU_Cryptography_True_Random_Number_Generation.pdf
*
* Hardware Dependency: 
*      See CE221295_PSoC_6_MCU_Cryptography_True_Random_Number_Generation.pdf
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
#include "stdio_user.h"
#include <stdio.h>

/* Crypto Server context structure. The driver uses this structure tostore and
   manipulate the server context. */
cy_stc_crypto_server_context_t cryptoServerContext;

/*****************************************************************************
* Function Name: main
******************************************************************************
* Summary:
*  Start the CM4 core,
*  start the Crypto server on CM0 core.
*
* Parameters:
*  None
*
* Return
*  int
*
*****************************************************************************/

int main(void)
{
    /* Enable global interrupts. */
    __enable_irq(); 
    
    /* Start the UART Component to printdebug messages on to 
       the console output */
    UART_Start();
    
    /* Start the Crypto Server, only on Cm0 core */
    Cy_Crypto_Server_Start(&cryptoConfig,&cryptoServerContext);
    
    /* Wait for Crypto Block to be available */
    Cy_Crypto_Sync(CY_CRYPTO_SYNC_BLOCKING);
     
        
    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if
        CM4 memory layout is changed. */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 
    
    /* The Crypto Server (CM0p core), handles the client (CM4 core) 
       requests in the ISR */
    for(;;)
    {
        
    }
}

/* [] END OF FILE */
