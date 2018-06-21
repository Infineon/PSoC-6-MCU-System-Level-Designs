/*****************************************************************************
* File Name: main_cm4.c
*
* Version 1.10
*
* Description: Demonstration of the Profiler module in event mode
* tracing the effects of using the UART with and without interrupts.
*
* Related Document: Code example CE219765.pdf
*
* Hardware Dependency: See CE219765.pdf
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

#include <project.h>
#include <stdio.h>
#include "my_stdio_user.h"

/**
********************************************************************************
* Setup area for modifiable Energy Profiler settings.
*******************************************************************************/

#define PROFILE_USE_IRQ               /* < handle counter overflows in profiler            */
#define PROFILE_IRQ_INTR_PRIORITY 7   /* < Int priority number (Refer to __NVIC_PRIO_BITS) */

/*  The Energy Profiler implements the concept of a coefficient. This allows
 *  you to adjust the raw count by a factor so you can tune the results to
 *  the actual energy use of the operation. In this example we simply use
 * a coefficient of 1 and deliver the raw count of events.
 */

//#define WEIGHT_NORM  1000000ul  /* Scaling coefficients can be any value */
#define WEIGHT_NORM  1ul          /* Value of 1 returns actual count */

/* Interrupt configuration structure */
#if defined (PROFILE_USE_IRQ)
    const cy_stc_sysint_t profileIrqCfg =
    {
    	.intrSrc = (IRQn_Type)profile_interrupt_IRQn, /**< Interrupt source */
    	.intrPriority = PROFILE_IRQ_INTR_PRIORITY   /**< Interrupt priority number */
    };
#endif /* (PROFILE_USE_IRQ) */
/*
 *
 *******************************************************************************/

/* profile counter configuration structure */
cy_stc_profile_ctr_ptr_t cy_profiler_ctrs[PROFILE_PRFL_CNT_NR];

/********************************************************************************
* Externally used parameters
********************************************************************************/

/**
********************************************************************************
* Function Name: main
********************************************************************************
*
*  Sets up the UART and stdout
*  Configures the profiler interrupt
*  Enables the UART interrupt and profiles UART use (counts # of events)
*  Disables the UART interrupt and profiles UART use (counts # of events)
*******************************************************************************/
int main(void)
{
    uint8_t  myLoopCount;
    uint8_t  myTestNumb = 30; //<<-- Can be modified not to exceed 60. Which is sizeof(myString[])
    uint16_t myStatusReturn; // Error return value of Profile functions - no error handling in this CE
    char  myString[]= ("123456789012345678901234567890123456789012345678901234567890");

    /* Define counters in SRAM to hold values of all eight hardware counters
       only really need two for example, but defined for max */
    uint64_t my_Profile_current_raw_value[8];

    /* Turn fitter define to variable */
    #if defined(UART_SCB_IRQ__INTC_ASSIGNED)
        UartUsesIRQ = true;
    #else
        UartUsesIRQ = false;
    #endif /* (UART_SCB_IRQ__INTC_ASSIGNED) */

    /********************************************************************************
    * Initialize and set initial conditions of a LED for visual effect.
    * These can change for a different kit, or be removed entirely
    ********************************************************************************/
    Cy_GPIO_Pin_FastInit(P11_1_PORT, P11_1_NUM, CY_GPIO_DM_STRONG, 1U, P1_1_GPIO); /* Init GPIO off BLUE LED pin  */
    Cy_GPIO_Pin_FastInit( P1_1_PORT, P1_1_NUM, CY_GPIO_DM_STRONG, 1U, P1_1_GPIO);  /* Init GPIO off GREEN LED pin  */
    Cy_GPIO_Pin_FastInit( P0_3_PORT, P0_3_NUM, CY_GPIO_DM_STRONG, 1U, P1_1_GPIO);  /* Init GPIO off RED LED pin */

    /********************************************************************************
    * Setup and start UART function. UART number is selected by choosing pins in
    * Design Wide Resources. Currently set for SCB #5
    ********************************************************************************/

    /* Setup and start UART function.*/
    UART_Start();
	
#if defined (__GNUC__)
    /* Change stream buffering - each output operation is written as soon as possible.*/
    setvbuf(stdout, NULL, _IONBF, 0); // switch off buffering in (printf->) STDOUT),
                                      // otherwise input function will require 1024 characters (default value)
                                      // to be entered before sending printf to terminal
#endif   /* (__GNUC__) */

    /******************************************************************************
    * The PDL implementation includes a profiler ISR named Cy_Profile_ISR. This
    * default ISR handles counter overflow.
    *  If a more a complex interrupt process is needed, write your own function and place
    *  a pointer to it in the Cy_SysInt_Init() call replacing &Cy_Profile_ISR.
    *******************************************************************************/

    #if defined(PROFILE_USE_IRQ)
        (void) Cy_SysInt_Init(&profileIrqCfg, &Cy_Profile_ISR); /* ISR provided in API */
        NVIC_EnableIRQ(profileIrqCfg.intrSrc);
    #endif

    __enable_irq(); /* Enable global interrupts. */

    printf("\x1b[2J\x1b[;H");/* <ESC>[2J<ESC>[;H - ANSI ESC sequence for clear screen*/

    /* Transmit header to the terminal.*/
    printf("**********************************************************************\n\r");
    printf("*                CE219765 - PSoC6 MCU Event Profiling                *\n\r");
    printf("*           If you are able to read this text the terminal           *\n\r");
    printf("*           connection is configured correctly.   9600 8N1           *\n\r");
    printf("**********************************************************************\n\r");
    printf("*           Built: %s",__DATE__);  //Uses date and time functions
    printf(" at %s\n\r",__TIME__);             //from the compiler
    printf("**********************************************************************\n\r");
    printf("    Core Freq = %ld MHz         ",cy_Hfclk0FreqHz/1000000ul);
    printf("    PERI Freq = %ld MHz\n\n\r",cy_PeriClkFreqHz/1000000ul);

    printf ("    UART output set to (stdout)  - Done\n\n\r"); //see lines 112-120
    UartUsesIRQ = true;
    printf ("    UART interrupt - Enabled\n\r");

    /* Initialize Profile Module. */
    Cy_Profile_Init();
    printf ("    Profile Module - Initializing - Done\n\r");

    /* Clear the Profile configuration as good practice. */
    Cy_Profile_ClearConfiguration();
    printf ("    Profile Module - Clear any old configuration data - Done\n\r");

    /* Initialize counter handle. */
    cy_profiler_ctrs[0] = Cy_Profile_ConfigureCounter(SCB5_MONITOR_AHB,CY_PROFILE_EVENT,CY_PROFILE_CLK_HF,WEIGHT_NORM); //SCB5
    printf ("    Profile Module - Assign SCB5 to Counter[0] - Done\n\r");

    myStatusReturn = Cy_Profile_EnableCounter(cy_profiler_ctrs[0]);
       if (false != myStatusReturn)
        {
            /* insert error handling here*/
        }

    printf ("    Profile Module - Set enabled for Profile Counter[0] - Done\n\n\r");

    /* Run a series of tests (30 by default) myTestNumb */
    for(myLoopCount = 1u; myLoopCount < myTestNumb; myLoopCount++)
    {
        /* Start things running. Toggle RED led so we know it's running */
        Cy_GPIO_Inv(P0_3_PORT, P0_3_NUM); /* toggle the RED LED pin */

        /*******************************************************************************/
        printf ("************  Monitoring UART in Interrupt Mode  *********************\n\r");
        /*******************************************************************************/
        /* Start the Profiler measuring */
        Cy_Profile_StartProfiling();

        /* We are measuring these accesses to UART */
        printf("%*c", myLoopCount, myString[myLoopCount] );  //prints myLoopCount-1 blanks followed by the character at TestCharBuf[i]

        Cy_Profile_StopProfiling();
        /*******************************************************************************/

        myStatusReturn = Cy_Profile_GetRawCount(cy_profiler_ctrs[0], &my_Profile_current_raw_value[0]);
           if (0 != myStatusReturn)
            {
                /* insert error handling here*/
            }
        printf ("\r\n\nSCB5 Profile counter[0] total for %02d characters  =  %018lld", myLoopCount+1,my_Profile_current_raw_value[0]);
        printf ("\033[18;H");  /* <ESC>[23;H - ANSI ESC sequence for Cursor Home + 18 lines down*/
        Cy_Profile_ClearCounters();
    }

    /* set up for the next test */
    printf ("\033[23;H");  /* <ESC>[23;H - ANSI ESC sequence for Cursor Home + 23 lines down*/

    Cy_GPIO_Set(P0_3_PORT, P0_3_NUM); /* Turn off RED LED pin */

    /* Disable the UART interrupt, this test uses polling */
    UartUsesIRQ = false;
    printf ("    UART interrupt - Disabled\n\r");

    /* disable the original counter */
    myStatusReturn = Cy_Profile_DisableCounter(cy_profiler_ctrs[0]);
       if (false != myStatusReturn)
        {
            /* insert error handling here*/
        }
    printf ("    Profile Module - Disable Profile Counter[0] - Done\n\r");

    /* Clear the Profile configuration as good practice. */
    Cy_Profile_ClearConfiguration();
    printf ("    Profile Module - Clear any old configuration data - Done\n\r");

    /* Configure the counter, use a different counter just for demonstration */
    cy_profiler_ctrs[3] = Cy_Profile_ConfigureCounter(SCB5_MONITOR_AHB,CY_PROFILE_EVENT,CY_PROFILE_CLK_HF,WEIGHT_NORM); //
    printf ("    Profile Module - Assign SCB5 to Counter[3] - Done\n\r");

    /* Enable the new counter */
    Cy_Profile_EnableCounter(cy_profiler_ctrs[3]);
    printf ("    Profile Module - Set enabled for Profile Counter[3] - Done\n\n\r");

    /* Run a series of tests (30 by default) */
    for(myLoopCount = 1u; myLoopCount < myTestNumb; myLoopCount++)
    {
       /* Start things running. Toggle the Blue led for this test */
       Cy_GPIO_Inv(P11_1_PORT, P11_1_NUM);  /* toggle the BLUE LED */

       /*******************************************************************************/
       printf ("**************  Monitoring UART in Polling Mode  *********************\n\r");
       /*******************************************************************************/
        /* Start the Profiler measuring */
        Cy_Profile_StartProfiling();

        /* We are measuring these accesses to UART */
        printf("%*c", myLoopCount, myString[myLoopCount] );  //prints myLoopCount-1 blanks followed by the character at TestCharBuf[i]

        Cy_Profile_StopProfiling();
        /*******************************************************************************/

        myStatusReturn = Cy_Profile_GetRawCount(cy_profiler_ctrs[3], &my_Profile_current_raw_value[3]);
           if (false != myStatusReturn)
            {
                /* insert error handling here*/
            }

        printf ("\n\r\nSCB5 Profile counter[3] total for %02d characters  =  %018lld", myLoopCount+1,my_Profile_current_raw_value[3]);

        printf ("\033[29;H");  /* <ESC>[23;H - ANSI ESC sequence for Cursor Home + 29 lines down*/
        Cy_Profile_ClearCounters();
    }

    /* Demonstration completed */
    printf ("\n\n\n\n\n\n\r******** This run has completed - press Reset to restart ********\n\r");

    /* disable the NVIC interrupt */
    #if defined(PROFILE_USE_IRQ)
        NVIC_DisableIRQ(profileIrqCfg.intrSrc);
    #endif

    Cy_GPIO_Set(P11_1_PORT, P11_1_NUM);  /* Turn off BLUE LED  */
    Cy_GPIO_Set(P0_3_PORT, P0_3_NUM);    /* Turn off RED LED   */
    while (1)  /* all done with example code stick here and blink green LED  */
    {
        Cy_GPIO_Inv(P1_1_PORT, P1_1_NUM); /* Toggle the GREEN LED */
        /* Delay to toggle Green LED */
        CyDelay(750/*msec*/);
    }
}
