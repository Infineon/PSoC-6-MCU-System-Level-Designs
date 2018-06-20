/******************************************************************************
* File Name: main_cm0p.c
* Version 1.0
*
* Description:
*  This example demonstrates the fault handling fuctionality of PSoC 6 MCU and
*  how to add a custom fault handling function to find the fault location. 
*  The UART interface will be used for showing ARM register information.
*
* Hardware Dependency: CY8CKIT-062-BLE PSoC 6 BLE Pioneer Kit
*
*******************************************************************************
* Copyright (2017-2018), Cypress Semiconductor Corporation. All rights reserved.
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
#include "stdio_user.h"
#include <stdio.h>

/* Constants */
#define SWITCH_PRESSED          (0u)
#define SWITCH_RELEASED         (1u)
#define SWITCH_PRESS_DELAY      (50u)   /* 50msec */
#define SWITCH_TRIGGER_COUNT    (3u)   

/* Macro used to read switch state */
#define SwitchState             Cy_GPIO_Read(SW2_PORT, SW2_NUM)

const uint32_t makingFault = 0u;

/* Function prototype */
static void ForceHardFault(void);

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  System entrance point. This function calls the initializing functions and 
*  processes switch press event.
*
* Parameters:
*  None
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    uint32_t prevState = SWITCH_RELEASED;
    uint8_t pressCount = 0 ;
    
    /* Initialize with the configuration set in component and enable the UART */
    Cy_SCB_UART_Init(UART_HW, &UART_config, &UART_context);
    Cy_SCB_UART_Enable(UART_HW);
    
    __enable_irq(); /* Enable global interrupts. */
    
    printf("CM0+ Started\r\n");
    
    /* Enable CM4. CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 

    for(;;)
    {
        if(SWITCH_PRESSED == SwitchState)
        {
            prevState = SWITCH_PRESSED;
        }
        else
        {
            if(prevState == SWITCH_PRESSED)
            {
                pressCount++;
                if(pressCount >= SWITCH_TRIGGER_COUNT)
                {
                    /* Generate CM0+ hard fault */
                    ForceHardFault();
                }
            }
            prevState = SWITCH_RELEASED;
        }
        Cy_SysLib_Delay(SWITCH_PRESS_DELAY);
    }
}

/*******************************************************************************
* Function Name: ForceHardFault
********************************************************************************
* Summary:
*  This function generates the hard fault exception intentionally by writing a
*  value into read-only area. 
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ForceHardFault(void)
{
    /* Get the address of makingFault (constant) */ 
    uint32_t *ptrFault = (uint32_t *)&makingFault;

    /* Write 100u into RO area. HARD FAULT!! */
    *ptrFault = 100u;
}

/*******************************************************************************
* Function Name: Cy_SysLib_ProcessingFault
********************************************************************************
* Summary:
*  This function prints out the stacked registers at the moment the hard fault 
*  occured.
*  cy_syslib.c includes same function as __WEAK option, this function will  
*  replace the weak function. Cy_SysLib_ProcessingFault() is called at the end
*  of Cy_SysLib_FaultHandler() function which is the default exception handler 
*  set for hard faults.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Cy_SysLib_ProcessingFault(void)
{
    /* Print the Fault Frame */
    printf("\r\nCM0+ FAULT!!\r\n");
    printf("r0  = 0x%08lx\r\n", (uint32_t)cy_faultFrame.r0);
    printf("r1  = 0x%08lx\r\n", (uint32_t)cy_faultFrame.r1);
    printf("r2  = 0x%08lx\r\n", (uint32_t)cy_faultFrame.r2);
    printf("r3  = 0x%08lx\r\n", (uint32_t)cy_faultFrame.r3);
    printf("r12 = 0x%08lx\r\n", (uint32_t)cy_faultFrame.r12);
    printf("lr  = 0x%08lx\r\n", (uint32_t)cy_faultFrame.lr);
    printf("pc  = 0x%08lx\r\n", (uint32_t)cy_faultFrame.pc);
    printf("psr = 0x%08lx\r\n", (uint32_t)cy_faultFrame.psr);
    
    while(1);
}

/* [] END OF FILE */
