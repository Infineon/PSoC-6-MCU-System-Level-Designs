/***************************************************************************//**
* \file stdio_user.h
* \version 1.10
*
* \brief
* This file provides configuration macros and function prototypes to retarget
* I/O functions of the standard C run-time library.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _STDIO_USER_H_
#define _STDIO_USER_H_


/**
* \page page_retarget_io Retarget I/O
* <p>
* Application code frequently uses standard I/O library functions, such as
* printf() to perform output operations. This utility allows you to retarget I/O
* functions of the standard C run-time library to the user defined target.
* </p>
*
* To use the utility you need:<br>
* * Add retarget.c file to your project, and add the retarget_io folder to
* your list of include paths.<br>
*
* * Provide low level function implementation. The example can be found in the
* stdio_user.c file.
*
* <h1>MISRA-C Compliance</h1>
* The Retarget IO utility has the following specific deviations:
*
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>5.6</td>
*     <td>A</td>
*     <td>No identifier in one name space should have the same spelling
*         as an identifier in another name space, with the exception of
*         structure member and union member names.</td>
*     <td>Violated since the utility redefines the function declared in standard
*         library.</td>
*   </tr>
*   <tr>
*     <td>6.3</td>
*     <td>A</td>
*     <td><i>typedefs</i> that indicate size and signedness should be used in
*         place of the basic numerical type.</td>
*     <td>Basic numerical types are used to match the definition of the
*         function with the prototype defined in the standard library.</td>
*   </tr>
*   <tr>
*     <td>8.8</td>
*     <td>R</td>
*     <td>An external object or function shall be declared in one and only one file.</td>
*     <td>The <i>_write</i> is declared in the standard i/o library with
*         <i>weak</i> attribute and is redefined in the utility.</td>
*   </tr>
*   <tr>
*     <td>14.2</td>
*     <td>R</td>
*     <td>All non-null statements shall either:<br>(a) have at least one
*         side-effect however executed, or <br>(b) cause control flow to change.</td>
*     <td>The unused function parameters are cast to void. This statement
*         has no side-effect and is used to suppress a compiler warning.</td>
*   </tr>
*   <tr>
*     <td>20.9</td>
*     <td>R</td>
*     <td>The input/output library <i><stdio.h></i> shall not be used in
*         production code.</td>
*     <td><i>stdio.h</i> file is included to connect the standard function
*         definition with their declaration in the standard library.</td>
*   </tr>
* </table>
*/
#include "cy_device_headers.h"
#include <stdbool.h>

extern bool UartUsesIRQ;

/* Must remain uncommented to use this utility */
#define IO_STDOUT_ENABLE
#define IO_STDIN_ENABLE
#define IO_STDOUT_UART
#define IO_STDIN_UART
#define IO_STDIO_UART_NUMBER SCB5


#if defined(IO_STDOUT_ENABLE) || defined(IO_STDIN_ENABLE)
#if defined(IO_STDOUT_UART) || defined(IO_STDIN_UART)
#include "scb/cy_scb_uart.h"
#endif /* IO_STDOUT_UART || IO_STDIN_UART */
#endif /* IO_STDOUT_ENABLE || IO_STDIN_ENABLE */

/* Controls whether CR is added for LF */
#ifndef STDOUT_CR_LF
#define STDOUT_CR_LF    0
#endif /* STDOUT_CR_LF */

#if defined(__cplusplus)
extern "C" {
#endif

#if defined (IO_STDOUT_ENABLE) && defined (IO_STDOUT_UART)
void STDIO_PutChar(uint32_t ch);
#endif /* IO_STDOUT_ENABLE && IO_STDOUT_UART */

#if defined (IO_STDIN_ENABLE) && defined (IO_STDIN_UART)
uint32_t STDIO_GetChar(void);
#endif /* IO_STDIN_ENABLE && IO_STDIN_UART */

#if defined(__cplusplus)
}
#endif

#endif /* _STDIO_USER_H_ */


/* [] END OF FILE */
