/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for CM4 in the the Dual CPU IPC Pipes 
*              Application for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2021-2022, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>

#include "console.h"

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*   CM4 main function. Initialize the BSP and run the scheduler.
*
* Return
*   int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* Initialize the User Button */
    cyhal_gpio_init(CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);   

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("\r\n\n******* Protection Units Example *******\r\n\n");

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler() ;

    for (;;)
    {

    }
}

/*******************************************************************************
* Function Name: vApplicationDaemonTaskStartupHook
********************************************************************************
* Summary:
*   Executes after the scheduler starts. 
*
*******************************************************************************/
void vApplicationDaemonTaskStartupHook(void)
{
    console_init();
}

/*******************************************************************************
* Function Name: Cy_SysLib_ProcessingFault
********************************************************************************
* Summary:
*   Executes when a fault occurs.
*
*******************************************************************************/
void Cy_SysLib_ProcessingFault (void)
{
    char msg[]= "\r\nCM4 Fault!!!\r\n";
    size_t size = sizeof(msg);

    cyhal_uart_write(&cy_retarget_io_uart_obj, &msg, &size);

    /* Handle write failures */
    if ((cy_faultFrame.cfsr.cfsrReg & SCB_CFSR_IMPRECISERR_Msk)
            == SCB_CFSR_IMPRECISERR_Msk)
    {
        char msg_wr[] = "\n\rFailed to write to memory\n\r";
        size_t size_wr = sizeof(msg_wr);
        cyhal_uart_write(&cy_retarget_io_uart_obj, &msg_wr, &size_wr);
    }

    /* Handle accesses to protected memory */
    if ((cy_faultFrame.cfsr.cfsrReg & SCB_CFSR_BFARVALID_Msk)
            == SCB_CFSR_BFARVALID_Msk)
    {
        char msg_pr[] = "\n\rAttempt to access protected memory by CM4\n\r";
        size_t size_pr = sizeof(msg_pr);
        cyhal_uart_write(&cy_retarget_io_uart_obj, &msg_pr, &size_pr);
    }

    while(1);
}

/* [] END OF FILE */
