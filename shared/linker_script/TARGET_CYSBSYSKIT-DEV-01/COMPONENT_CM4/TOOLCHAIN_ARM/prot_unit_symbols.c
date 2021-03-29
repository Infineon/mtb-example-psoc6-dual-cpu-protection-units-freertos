/*******************************************************************************
* \file dfu_elf_symbols.c
* \version 4.0
* 
* This file provides inline assembly to add symbols to determine the size of the
* regions defined in the linker script. 
*
* \note
* This file requires modifications of DFU specific symbols for the 
* application #1.
* 
********************************************************************************
* \copyright
* Copyright 2020, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
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
    ".equ __cm0p_flash_length,  0x00040000 \n"
    ".equ __cm0p_sram_start,    0x08000000 \n"
    ".equ __cm0p_sram_length,   0x00020000 \n"
    ".equ __cm4_flash_start,    0x10040000 \n"
    ".equ __cm4_flash_length,   0x001C0000 \n"
    ".equ __cm4_sram_start,     0x08040000 \n"
    ".equ __cm4_sram_length,    0x000BF800 \n"
    ".equ __shared_sram_start,  0x08020000 \n"
    ".equ __shared_sram_length, 0x00020000 \n"
);


/* [] END OF FILE */
