/******************************************************************************
* File Name: main_cm4.c
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
#include<stdio.h>

/* Constants */
#define SWITCH_PRESSED      (0u)
#define SWITCH_RELEASED     (!SWITCH_PRESSED)
#define SWITCH_PRESS_DELAY  (50u)   /* 50 msec */
#define LONG_SWITCH_PRESS   (40u)   /* 40 * 50 msec = 2 sec   */
#define SHORT_SWITCH_PRRESS (3u)    /* 3 * 50 msec   150 msec */

/* Macro used to read switch state */
#define SwitchState         Cy_GPIO_Read(SW2_PORT, SW2_NUM)

const uint32_t writeToCauseFaultCM4 = 0u;

/* Function prototype */
static void InitializeFaultRegister(void);
static void ForceBusFaultCM4(void);
static uint32_t ForceUsageFaultCM4(uint32_t*); 

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
    uint32_t pressCount = 0;
    uint32_t var = 0;
    printf("CM4 Started\r\n\n");
    printf("Press SW2 and release once quickly - Force usage fault for CM4\r\n");
    printf("Press SW2 and release three times in quick succession - Force hard fault for CM0+\r\n");
    printf("Press the RST button to start over once a fault has been triggered\r\n");
    printf("Press and hold SW2 for at least 2 seconds, then release - Force a CM4 bus fault\r\n");
    
    __enable_irq(); /* Enable global interrupts. */

    /* Configure necessary fault handling register */
    InitializeFaultRegister();

    for(;;)
    {
        if(SWITCH_PRESSED == SwitchState)
        {
            if(SWITCH_PRESSED == prevState)
            {
                pressCount++;
            }
            else
            {
                prevState = SWITCH_PRESSED;
            }
        }
        else
        {
            if(pressCount >= LONG_SWITCH_PRESS)
            {
                printf("\r\nForce CM4 Bus Fault!");
                ForceBusFaultCM4();                
            }
            else if(pressCount >= SHORT_SWITCH_PRRESS)
            {
                printf("\r\nForce CM4 Usage Fault!");
                (void)ForceUsageFaultCM4(&var);
            }
            prevState = SWITCH_RELEASED;
        }
        Cy_SysLib_Delay(SWITCH_PRESS_DELAY);
    }
}

/*******************************************************************************
* Function Name: InitializeFaultRegister
********************************************************************************
* Summary:
*  This function configures the fault registers(bus fault and usage fault).
*
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void InitializeFaultRegister(void)
{
    /* Set SCB->SHCSR.BUSFAULTENA so that BusFault handler instead of the HardFault
       handler handles the bus fault. */
    SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
    
    /* If ACTLR.DISDEFWBUF is not set to 1, the imprecise BusFault will occur. 
       The fault stack information won't be accurate. Setting this bit to 1 disables
       write buffer during default memory map accesses so that bus faults will be
       precise.        
       WARNING: This will decrease the performance because any store to memory must 
       complete before the processor can execute the next instruction.
       Don't enable always, if it is not necessary. */
    SCnSCB->ACTLR |= SCnSCB_ACTLR_DISDEFWBUF_Msk;    
    
    /* Enable the trap for divide by 0. SCB->CCR.DIV_0_TRP = 1 */
    SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;
    
    /* Set SCB->SHCSR.USGFAULTENA so that faults such as DIVBYZERO, UNALIGNED, UNDEFINSTR
       etc are handled by UsageFault handler instead of the HardFault handler.*/
    
    /* Enable the UsageFault, SCB->SHCSR.USGFAULTENA = 1. */
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;
}


/*******************************************************************************
* Function Name: ForceBusFaultCM4
********************************************************************************
* Summary:
*  The BusFault exception will be generated intentionally by writing a value 
*  into read-only area. 
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void ForceBusFaultCM4(void)
{
    /* Get the address of makingFault (constant) */ 
    uint32_t *ptrFault = (uint32_t *)&writeToCauseFaultCM4;
    
    /* Write any value into the read-only variable to cause bus fault */
    *ptrFault = 10u;
}

/*******************************************************************************
* Function Name: ForceUsageFaultCM4
********************************************************************************
* Summary:
*  This function divdes 100 by intVal. It will generate the usage fault, if the
*  intVal is zero.
*  This simple logic is intentionally used a function to avoid the code 
*  optimized out by a compiler. 
*
* Parameters:
*  None
*
* Return:
*  uint32_t
*
*******************************************************************************/
static uint32_t ForceUsageFaultCM4(uint32_t* intVal)
{
    uint32_t faultNum = 100u;
    
    /* If *intVal = 0u then it triggers a fault because of DIVBYZERO (Divide by zero).
       The SCB->UFSR bit 9(=CFSR bit 25) will be set to 1, once the fault has occured. */
    faultNum /= *intVal;
    return faultNum;
}

/*******************************************************************************
* Function Name: Cy_SysLib_ProcessingFault
********************************************************************************
* Summary:
*  This function prints out the stacked register at the moment the hard fault 
*  occurred.
*  cy_syslib.c include same function as __WEAK option, this function will  
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
    printf("\r\nCM4 FAULT!!\r\n");
    printf("SCB->CFSR = 0x%08lx\r\n", (uint32_t)cy_faultFrame.cfsr.cfsrReg);
    
    /* If MemManage fault valid bit is set to 1, print MemManage fault address */
    if((cy_faultFrame.cfsr.cfsrReg & SCB_CFSR_MMARVALID_Msk) == SCB_CFSR_MMARVALID_Msk )
    {
        printf("MemManage Fault! Fault address = 0x%08lx\r\n", SCB->MMFAR);
    }
    
    /* If Bus Fault valid bit is set to 1, print BusFault Address */
    if((cy_faultFrame.cfsr.cfsrReg & SCB_CFSR_BFARVALID_Msk) == SCB_CFSR_BFARVALID_Msk )
    {
        printf("Bus Fault! \r\nFault address = 0x%08lx\r\n", SCB->BFAR);
    }
    
    /* Print Fault Frame */
    printf("r0 = 0x%08lx\r\n", cy_faultFrame.r0);
    printf("r1 = 0x%08lx\r\n", cy_faultFrame.r1);
    printf("r2 = 0x%08lx\r\n", cy_faultFrame.r2);
    printf("r3 = 0x%08lx\r\n", cy_faultFrame.r3);
    printf("r12 = 0x%08lx\r\n", cy_faultFrame.r12);
    printf("lr = 0x%08lx\r\n", cy_faultFrame.lr);
    printf("pc = 0x%08lx\r\n", cy_faultFrame.pc);
    printf("psr = 0x%08lx\r\n", cy_faultFrame.psr);

    while(1);
}

/* [] END OF FILE */
