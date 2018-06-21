/*****************************************************************************
* File Name: main_cm0.c
*
* Version: 1.20
*
* Description: This example project demonstrates the use of an Crypto PDL for 
*              AES encryption. 
*              This is the source code for m0p core. m0p core runs crypto server,
*              where all crypto instructions are executed.
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

cy_stc_crypto_server_context_t cryptoServerContext;

/*******************************************************************************
* Function Name: main
********************************************************************************
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
*******************************************************************************/
int main(void)
{     
      __enable_irq();
    /* Start the Crypto Server, only on M0 core */ 
    Cy_Crypto_Server_Start(&cryptoConfig,&cryptoServerContext);
    
    /* Start the CM4 core */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR);

    /*The Crypto Server (CM0p core), handles the client (CM4 core) 
    requests in the ISR*/
    for(;;)
    {
        
    }
}
