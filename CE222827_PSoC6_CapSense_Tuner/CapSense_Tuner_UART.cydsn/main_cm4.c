/******************************************************************************
* File Name: main_cm4.c
*
* Version 1.0
*
* Description: This project is to demonstrate how get the CapSense data over UART.
*  Tuner GUI is used to display buttons active state and slider position. The UART 
*  component is used to establish communication with Tuner GUI.
*
* Related Document: CE222827_PSoC6_CapSense_Tuner.pdf
*
* Hardware Dependency: CY8CKIT-062-BLE PSoC 6 BLE Pioneer Kit
*
*******************************************************************************
* Copyright (2018), Cypress Semiconductor Corporation. All rights reserved.
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

/* Packet header and tail information */
#define PACKET_HEADER_DATA  {0x0Du, 0x0Au}
#define PACKET_TAIL_DATA    {0x00u, 0xFFu, 0xFFu}

/*******************************************************************************
* Function Name: SendDataToTuner
********************************************************************************
* Summary:
*  This function sends CapSense data to Tuner application.
*******************************************************************************/
void SendDataToTuner(void)
{
    /* Variables that stores packet header and tail information */
    static uint8_t packetHeader[]   = PACKET_HEADER_DATA;
    static uint8_t packetTail[]     = PACKET_TAIL_DATA;
    
    /* Send packet header */
    Tx8_PutArrayBlocking((uint8_t *)(&packetHeader), sizeof(packetHeader));
    /* Send packet with CapSense data */
    Tx8_PutArrayBlocking((uint8_t *)(&CapSense_dsRam), sizeof(CapSense_dsRam));
    /* Send packet tail */
    Tx8_PutArrayBlocking((uint8_t *)(&packetTail), sizeof(packetTail));
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  System entrance point. This function starts the CapSense Component and UART
*  Component and sends CapSense data to Tuner application.
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
    __enable_irq(); /* Enable global interrupts. */

    /* Start UART SCB Component */
    Tx8_Start();
    
    /* Start CapSense Component */
    CapSense_Start();
    /* Start initial scan */
    CapSense_ScanAllWidgets();
    
    for(;;)
    {
        /* Do this only when a scan is done */
        if (CapSense_NOT_BUSY == CapSense_IsBusy())
        {
            /* Process all widgets */
            CapSense_ProcessAllWidgets();
            
            /* Send CapSense data to Tuner */
            SendDataToTuner();
           
            /* Start next scan */
            CapSense_ScanAllWidgets();
        }
    }
}

/* [] END OF FILE */
