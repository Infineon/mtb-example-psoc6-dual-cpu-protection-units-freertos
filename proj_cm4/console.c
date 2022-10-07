/******************************************************************************
* File Name:   console.c
*
* Description: This file contains the implementation of the command console.
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
#include "console.h"

#include "command_console.h"
#include "cy_retarget_io.h"

#include "cy_sysint.h"
#include "cy_utils.h"

#include "cy_prot.h"

/*******************************************************************************
* Constants
*******************************************************************************/
/* Command console buffers */
#define CONSOLE_COMMAND_MAX_PARAMS     (32)
#define CONSOLE_COMMAND_MAX_LENGTH     (256)
#define CONSOLE_COMMAND_HISTORY_LENGTH (10)

/* List of console commands */
#define CONSOLE_COMMANDS \
    { "read", console_read, 1, NULL, NULL, (char *)"", (char *)"Read from given address. Ex: read 0x10000000"}, \
    { "write", console_write, 2, NULL, NULL, (char *)"", (char *)"Write to a specific address. Ex: write 0x8000000 0x00000000."}, \
    { "listmap", console_listmap, 0, NULL, NULL, (char *)"", (char *)"List the memory map of this firmware."}, \

/*******************************************************************************
* Local Functions
*******************************************************************************/
int console_read(int argc, char *argv[], tlv_buffer_t **data);
int console_write(int argc, char *argv[], tlv_buffer_t **data);
int console_listmap(int argc, char *argv[], tlv_buffer_t **data);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Command table */
const cy_command_console_cmd_t console_command_table[] =
 {
    CONSOLE_COMMANDS
    CMD_TABLE_END
 };

/* Console buffer variables */
static char command_buffer[CONSOLE_COMMAND_MAX_LENGTH];
static char command_history_buffer[CONSOLE_COMMAND_MAX_LENGTH * CONSOLE_COMMAND_HISTORY_LENGTH];
const char* console_delimiter_string = " ";

/* Externs from Linker script */
extern int __cm0p_flash_start;
extern int __cm0p_flash_length;
extern int __cm4_flash_start;
extern int __cm4_flash_length;
extern int __cm4_sram_start;
extern int __cm4_sram_length;
extern int __shared_sram_start;
extern int __shared_sram_length;

/*******************************************************************************
* Function Name: console_init
********************************************************************************
* Summary:
*   Initialize the console to process commands.
*
*******************************************************************************/
void console_init(void)
{
    cy_command_console_cfg_t console_cfg;
    cy_rslt_t result;

    console_cfg.serial             = (void *)&cy_retarget_io_uart_obj;
    console_cfg.line_len           = sizeof(command_buffer);
    console_cfg.buffer             = command_buffer;
    console_cfg.history_len        = CONSOLE_COMMAND_HISTORY_LENGTH;
    console_cfg.history_buffer_ptr = command_history_buffer;
    console_cfg.delimiter_string   = console_delimiter_string;
    console_cfg.params_num         = CONSOLE_COMMAND_MAX_PARAMS;

    /* Initialize command console library */
    result = cy_command_console_init(&console_cfg);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    result = cy_command_console_add_table( console_command_table );
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
}

/*******************************************************************************
* Function Name: console_read
********************************************************************************
* Summary:
*   Read a specific address in memory.
*
* Parameters:
*   argc: number of arguments
*   argv: list of arguments - address
*   data: not used
*
* Return
*   zero
*******************************************************************************/
int console_read(int argc, char *argv[], tlv_buffer_t **data)
{
    uint32_t address;

    sscanf(argv[1], "%x", (unsigned int *) &address);
    printf("Reading from address 0x%x -> 0x%x\n\r", (unsigned int) address, (unsigned int) CY_GET_REG32(address));

    return 0;
}

/*******************************************************************************
* Function Name: console_write
********************************************************************************
* Summary:
*   Write a value to a specific address in memory.
*
* Parameters:
*   argc: number of arguments
*   argv: list of arguments - address / value
*   data: not used
*
* Return
*   zero
*******************************************************************************/
int console_write(int argc, char *argv[], tlv_buffer_t **data)
{
    uint32_t address;
    uint32_t value;

    sscanf(argv[1], "%x", (unsigned int *) &address);
    sscanf(argv[2], "%x", (unsigned int *) &value);

    printf("Writing 0x%x to address 0x%x\n\r", (unsigned int) value, (unsigned int) address);
    CY_SET_REG32(address, value);

    return 0;
}

/*******************************************************************************
* Function Name: console_listmap
********************************************************************************
* Summary:
*   Print the memory map.
*
* Parameters:
*   argc: number of arguments
*   argv: list of arguments
*   data: not used
*
* Return
*   zero
*******************************************************************************/
int console_listmap(int argc, char *argv[], tlv_buffer_t **data)
{
    int cm0p_flash_start   = (int) &__cm0p_flash_start;
    int cm0p_flash_length  = (int) &__cm0p_flash_length;
    int shared_sram_start  = (int) &__shared_sram_start;
    int shared_sram_length = (int) &__shared_sram_length;
    int cm4_flash_start    = (int) &__cm4_flash_start;
    int cm4_flash_length   = (int) &__cm4_flash_length;
    int cm4_sram_start     = (int) &__cm4_sram_start;
    int cm4_sram_length    = (int) &__cm4_sram_length;

    printf("\n\r|--------------------------------------|\n\r" \
               "|             Memory Map               |\n\r" \
               "|--------------------------------------|\n\r" \
               "| Section  |  User Flash |    SRAM     |\n\r" \
               "|----------|-------------|-------------|\n\r");
    printf("|          | 0x%04x_%04x | 0x0800_0000 |\n\r", CY_HI16(cm0p_flash_start), 
                                                           CY_LO16(cm0p_flash_start));
    printf("| CM0+ App |      ~      |      ~      |\n\r");
    printf("|          | 0x%04x_%04x | 0x%04x_%04x |\n\r", CY_HI16(cm0p_flash_start + cm0p_flash_length - 1), 
                                                           CY_LO16(cm0p_flash_start + cm0p_flash_length - 1),
                                                           CY_HI16(shared_sram_start - 1),
                                                           CY_LO16(shared_sram_start - 1));
    printf("|----------|-------------|-------------|\n\r");
    printf("|  Shared  |             | 0x%04x_%04x |\n\r", CY_HI16(shared_sram_start),
                                                           CY_LO16(shared_sram_start));
    printf("|   SRAM   |      ~      |      ~      |\n\r");
    printf("|          |             | 0x%04x_%04x |\n\r", CY_HI16(shared_sram_start + shared_sram_length - 1),
                                                           CY_LO16(shared_sram_start + shared_sram_length - 1));
    printf("|----------|-------------|-------------|\n\r");
    printf("|          | 0x%04x_%04x | 0x%04x_%04x |\n\r", CY_HI16(cm4_flash_start), 
                                                           CY_LO16(cm4_flash_start),
                                                           CY_HI16(cm4_sram_start),
                                                           CY_LO16(cm4_sram_start));
    printf("| CM4 App  |      ~      |      ~      |\n\r");
    printf("|          | 0x%04x_%04x | 0x%04x_%04x |\n\r", CY_HI16(cm4_flash_start + cm4_flash_length - 1), 
                                                           CY_LO16(cm4_flash_start + cm4_flash_length - 1),
                                                           CY_HI16(cm4_sram_start + cm4_sram_length - 1),
                                                           CY_LO16(cm4_sram_start + cm4_sram_length - 1));
    printf("|----------|-------------|-------------|\n\n\r");
    
    return 0;
}