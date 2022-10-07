/*******************************************************************************
* File Name: prot_unit_symbols.c
*
* Version: 1.0
* 
* Description: This file provides inline assembly to add symbols to determine the size of the
*              regions defined in the linker script. 
* 
********************************************************************************
* Copyright 2020-2022, Cypress Semiconductor Corporation (an Infineon company) or
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

/* Symbols that are added to the ELF file */
__asm 
(
    /* Prot Units specific symbols */
    ".global __cm0p_flash_start \n"
    ".global __cm0p_flash_length \n"
    ".global __cm0p_sram_start \n"
    ".global __cm0p_sram_length \n"
    ".global __cm4_flash_start \n"
    ".global __cm4_flash_length \n"
    ".global __cm4_sram_start \n"
    ".global __cm4_sram_length \n"
    ".global __shared_sram_start \n"
    ".global __shared_sram_length \n"
    
    /* Region Lengths assigment */
    ".equ __cm0p_flash_start,   0x10000000 \n"
    ".equ __cm0p_flash_length,  0x00008000 \n"
    ".equ __cm0p_sram_start,    0x08000000 \n"
    ".equ __cm0p_sram_length,   0x00004000 \n"
    ".equ __cm4_flash_start,    0x10000000 \n"
    ".equ __cm4_flash_length,   0x00040000 \n"
    ".equ __cm4_sram_start,     0x08008000 \n"
    ".equ __cm4_sram_length,    0x00017800 \n"
    ".equ __shared_sram_start,  0x08004000 \n"
    ".equ __shared_sram_length, 0x00004000 \n"
);


/* [] END OF FILE */
