/***************************************************************************//**
* \file retarget.c
* \version 1.10
*
* \brief
* 'Retarget' layer for target-dependent low level function.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <stdio.h>
#include "my_stdio_user.h"
#include "UART.h"

#if defined(__cplusplus)
extern "C" {
#endif

bool UartUsesIRQ;
/******************************************************************************
* Function Name: stdio write
*******************************************************************************
* Description:
*  These functions overrides the C library function _write() with an
*  implementation that prints the data to UART. This helps to use printf()
*  to print formatted output to an UART console.
*  -->  Make sure to have enough heap space to hold messages.
******************************************************************************/
#ifdef IO_STDOUT_ENABLE
/* For GCC compiler overload the _write() function for printf functionality */
#if defined (__GNUC__)

    /**
    ******************************************************************************
    ** \brief  Overloads system function to let printf() write to UART
    **
    ** \param  handle  Standard interface
    ** \param  pu8Buf  Print buffer
    ** \param  u8Size  Print buffer size
    ** \return u8Size  Size of actual print
    ******************************************************************************/
    __attribute__((weak))int _write(int file, char *data, int len)
        {
        int bytes_written = 0;
        file = file; /* Suppress compiler warning about unused variable. */

    /******************************************************************************
    * Check if interrupt for SBC are enabled in component.
    * myUart_uses_IRQ will be defined globally in main.c
    * if internal interrupt are selected in the PSoC UART component
    *******************************************************************************/
        if (true == UartUsesIRQ)
        {
            while (CY_SCB_UART_TRANSMIT_BUSY == UART_Transmit((void *)data,len)) //uart_transmit uses interrupt
            {}

            return (len);
        }
    /******************************************************************************
    * If UART IRQ are not available then use polling mode to output characters.
    *******************************************************************************/
        else
        {// Use polling
            for(bytes_written = 0; bytes_written < len; bytes_written++)
            {
                /* Block until there is place in TX FIFO or buffer. */
                while (0UL == UART_Put(*data)) //put array (blocking)
                {}
                ++data;
            }
            return (bytes_written);
        }
     }
    #endif   /* (__GNUC__) */

#endif /* IO_STDOUT_ENABLE */

#if defined(IO_STDIN_ENABLE)



#if defined(__ARMCC_VERSION) /* ARM-MDK */
    /***************************************************************************
    * Function Name: fgetc
    ***************************************************************************/
    __attribute__((weak)) int fgetc(FILE *f)
    {
        (void)f;
        return (STDIO_GetChar());
    }

#elif defined (__ICCARM__) /* IAR */
    __weak size_t __read(int handle, unsigned char * buffer, size_t size)
    {
        /* This template only reads from "standard in", for all other file
        handles it returns failure. */
//jpwi        if ((handle != _LLIO_STDIN) || (buffer == NULL))
//jpwi        {
//jpwi           return (_LLIO_ERROR);
//jpwi        }
//jpwi        else
//jpwi        {
            *buffer = STDIO_GetChar();
            return (1);
//jpwi        }
    }

#else /* (__GNUC__)  GCC */
/* For GCC compiler overload the _write() function for printf functionality */
//#if defined (__GNUC__)

    /* Add an explicit reference to the floating point scanf library to allow
    the usage of floating point conversion specifier. */
    __asm (".global _scanf_float");
    __attribute__((weak)) int _read (int fd, char *ptr, int len)
    {
        int nChars = 0;
        (void)fd;
        if (ptr != NULL)
        {
            for(/* Empty */;nChars < len;++ptr)
            {
                *ptr = (char)STDIO_GetChar();
                ++nChars;
                if(*ptr == '\n')
                {
                    break;
                }
            }
        }
        return (nChars);
    }
#endif

#endif /* IO_STDIN_ENABLE */

#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */

