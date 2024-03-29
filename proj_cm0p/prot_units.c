/******************************************************************************
* File Name:   prot_units.c
*
* Description: This file contains the implementation of the protection units
*   configuration.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2021-YEAR Cypress Semiconductor $
*******************************************************************************/
#include "prot_units.h"

/*******************************************************************************
* Constants
*******************************************************************************/
#define PROT_UNITS_ALL_PC_MASK      (0x00007FFFUL)  /**< Mask for all supported PC values */
#define PROT_UNITS_DEVICE_PC_MASK   (PROT_UNITS_ALL_PC_MASK & ~CY_PROT_SMPU_PC_LIMIT_MASK) /**< Mask for device PC values */

#define PROT_UNITS_FLASH_SIZE_2MB   0x00200000UL
#define PROT_UNITS_FLASH_SIZE_1MB   0x00100000UL
#define PROT_UNITS_FLASH_SIZE_512KB 0x00080000UL
#define PROT_UNITS_FLASH_SIZE_256KB 0x00040000UL

#define PROT_UNITS_SRAM_SIZE_1MB    0x00100000UL
#define PROT_UNITS_SRAM_SIZE_512KB  0x00080000UL
#define PROT_UNITS_SRAM_SIZE_288KB  0x00048000UL
#define PROT_UNITS_SRAM_SIZE_256KB  0x00040000UL
#define PROT_UNITS_SRAM_SIZE_128KB  0x00020000UL

/*******************************************************************************
* Functions
*******************************************************************************/
/* Externs from Linker script */
extern int __cm0p_flash_start;
extern int __cm0p_flash_length;
extern int __cm0p_sram_start;
extern int __cm0p_sram_length;
extern int __cm4_flash_start;
extern int __cm4_flash_length;
extern int __cm4_sram_start;
extern int __cm4_sram_length;
extern int __shared_sram_start;
extern int __shared_sram_length;

/** ------------------------ FLASH Setup ---------------------------- **/
/** Slave SMPU config for CM0+ Application Flash region */
static const cy_stc_smpu_cfg_t cm0p_flash_prot_cfg_s = {
    .address = (uint32_t *)(CY_FLASH_BASE),           /* Start of CM0+ App Flash */
#if CY_FLASH_SIZE == PROT_UNITS_FLASH_SIZE_2MB
    .regionSize = CY_PROT_SIZE_256KB,
#elif CY_FLASH_SIZE == PROT_UNITS_FLASH_SIZE_1MB
    .regionSize = CY_PROT_SIZE_128KB,
#elif CY_FLASH_SIZE == PROT_UNITS_FLASH_SIZE_512KB
    .regionSize = CY_PROT_SIZE_64KB,
#else
    .regionSize = CY_PROT_SIZE_32KB,
#endif
    .subregions = (uint8_t)(0x00),                    /* Include all sub-regions */
    .userPermission = CY_PROT_PERM_RWX,               /* Full access to PC=1 */
    .privPermission = CY_PROT_PERM_RWX,               /* Full access to PC=1 */
    .secure = true,
    .pcMatch = false,
    .pcMask = (uint16_t)CY_PROT_PCMASK1 /* Only allow PC=1 */
};

/** Slave SMPU config for CM4 Application Flash region */
static const cy_stc_smpu_cfg_t cm4_flash_prot_cfg_s = {
    .address = (uint32_t *)(CY_FLASH_BASE),     /* Start of Flash */
#if CY_FLASH_SIZE == PROT_UNITS_FLASH_SIZE_2MB
    .regionSize = CY_PROT_SIZE_2MB,             /* Region size 2M, subregion = 256K */
#elif CY_FLASH_SIZE == PROT_UNITS_FLASH_SIZE_1MB
    .regionSize = CY_PROT_SIZE_1MB,             /* Region size 1M, subregion = 128K */
#elif CY_FLASH_SIZE == PROT_UNITS_FLASH_SIZE_512KB
    .regionSize = CY_PROT_SIZE_512KB,           /* Region size 512KB, subregion = 64K */
#else
    .regionSize = CY_PROT_SIZE_256KB,           /* Region size 256KB, subregion = 32K */
#endif
    .subregions = (uint8_t)(0x01),              /* Disable the first subregion only */
    .userPermission = CY_PROT_PERM_RWX,         /* Access is RWX for PC=4 */
    .privPermission = CY_PROT_PERM_RWX,
    .secure = false,
    .pcMatch = false,
    .pcMask = (uint16_t)CY_PROT_PCMASK4 /* Only allow PC=4 */
};

/** ------------------------ SRAM Setup ---------------------------- **/
/** Slave SMPU config for CM0+ Application SRAM region */
static const cy_stc_smpu_cfg_t cm0p_sram_prot_cfg_s = {
    .address = (uint32_t *)(CY_SRAM_BASE),           /* Beginning of SRAM  */
#if CY_SRAM_SIZE == PROT_UNITS_SRAM_SIZE_1MB
    .regionSize = CY_PROT_SIZE_128KB,                /* 128K of SRAM */
#elif CY_SRAM_SIZE == PROT_UNITS_SRAM_SIZE_512KB
    .regionSize = CY_PROT_SIZE_64KB,                 /* 64K of SRAM */
#elif CY_SRAM_SIZE == PROT_UNITS_SRAM_SIZE_288KB
    .regionSize = CY_PROT_SIZE_32KB,                 /* 32K of SRAM */
#elif CY_SRAM_SIZE == PROT_UNITS_SRAM_SIZE_256KB
    .regionSize = CY_PROT_SIZE_32KB,                 /* 32K of SRAM */
#else
    .regionSize = CY_PROT_SIZE_16KB,                 /* 8K of SRAM */
#endif
    .subregions = (uint8_t)(0x00),                   /* Include all sub-regions */
    .userPermission = CY_PROT_PERM_RW,               /* Read/Write no execute  */
    .privPermission = CY_PROT_PERM_RW,
    .secure = true,
    .pcMatch = false,
    .pcMask = (uint16_t)CY_PROT_PCMASK1              /* Only allow PC=1 */
};
/** Slave SMPU config for Shared SRAM region */
static const cy_stc_smpu_cfg_t shared_sram_prot_cfg_s = {
    .address = (uint32_t *)(&__shared_sram_start),   /* Beginning of Shared SRAM  */
#if CY_SRAM_SIZE == PROT_UNITS_SRAM_SIZE_1MB
    .regionSize = CY_PROT_SIZE_128KB,                /* 128K of SRAM */
#elif CY_SRAM_SIZE == PROT_UNITS_SRAM_SIZE_512KB
    .regionSize = CY_PROT_SIZE_64KB,                 /* 64K of SRAM */
#elif CY_SRAM_SIZE == PROT_UNITS_SRAM_SIZE_288KB
    .regionSize = CY_PROT_SIZE_32KB,                 /* 32K of SRAM */
#elif CY_SRAM_SIZE == PROT_UNITS_SRAM_SIZE_256KB
    .regionSize = CY_PROT_SIZE_32KB,                 /* 32K of SRAM */
#else
    .regionSize = CY_PROT_SIZE_16KB,                 /* 16K of SRAM */
#endif
    .subregions = (uint8_t)(0x00),                   /* Include all sub-regions */
    .userPermission = CY_PROT_PERM_RW,               /* Read/Write no execute  */
    .privPermission = CY_PROT_PERM_RW,
    .secure = false,
    .pcMatch = false,
    .pcMask = (uint16_t)(CY_PROT_PCMASK1|CY_PROT_PCMASK4) /* Only allow PC=1,4 to R/W */
};
/** Slave SMPU config for CM4 App SRAM region */
static const cy_stc_smpu_cfg_t cm4_sram_prot_cfg_s = {
    .address = (uint32_t *)(CY_SRAM_BASE),    /* Beginning of SRAM, block out first block only  */
#if CY_SRAM_SIZE == PROT_UNITS_SRAM_SIZE_1MB
    .regionSize = CY_PROT_SIZE_1MB,           /* 1MB with 128KB subregions of SRAM */
#elif CY_SRAM_SIZE == PROT_UNITS_SRAM_SIZE_512KB
    .regionSize = CY_PROT_SIZE_512KB,         /* 512 with 64KB subregions of SRAM */
#elif CY_SRAM_SIZE == PROT_UNITS_SRAM_SIZE_288KB
    .regionSize = CY_PROT_SIZE_256KB,         /* 256 with 32KB subregions of SRAM */
#elif CY_SRAM_SIZE == PROT_UNITS_SRAM_SIZE_256KB
    .regionSize = CY_PROT_SIZE_256KB,         /* 256 with 32KB subregions of SRAM */
#else
    .regionSize = CY_PROT_SIZE_128KB,         /* 128 with 16KB subregions of SRAM */
#endif
    .subregions = (uint8_t)(0x03),            /* Disable (0,1) regions */
    .userPermission = CY_PROT_PERM_RW,        /* Read/Write no execute  */
    .privPermission = CY_PROT_PERM_RW,
    .secure = false,
    .pcMatch = false,
    .pcMask = (uint16_t)(CY_PROT_PCMASK4) /* Only allow PC=4 to R/W */
};

/** Common Master SMPU config for all SMPUs */
static const cy_stc_smpu_cfg_t smpu_prot_cfg_m = {
    .userPermission = CY_PROT_PERM_R, /* Allow all to have read access (PC=0 has write access) */
    .privPermission = CY_PROT_PERM_R, /* Allow all to have read access (PC=0 has write access) */
    .secure = false,
    .pcMatch = false,
    .pcMask = (uint16_t)PROT_UNITS_DEVICE_PC_MASK
};

#if(CY_IP_MXPERI_VERSION == 1u)
    /** Slave (RG) PPU config for MS_CTL MMIO region */
    static const cy_stc_ppu_rg_cfg_t ms_ctl_prot_cfg_s = {
        .userPermission = CY_PROT_PERM_DISABLED,
        .privPermission = CY_PROT_PERM_DISABLED,
        .secure = true,
        .pcMatch = false,
        .pcMask = (uint16_t)0u /* Only allow PC=0 */
    };

    /** Common master config for all Region PPUs */
    static const cy_stc_ppu_rg_cfg_t rg_prot_cfg_m = {
        .userPermission = CY_PROT_PERM_R, /* Allow all to have read access (PC=0 has write access) */
        .privPermission = CY_PROT_PERM_R, /* Allow all to have read access (PC=0 has write access) */
        .secure = false,
        .pcMatch = false,
        .pcMask = (uint16_t)PROT_UNITS_DEVICE_PC_MASK
    };

    /** Common master config for all PROG PPUs */
    static const cy_stc_ppu_prog_cfg_t prog_prot_cfg_m = {
        .userPermission = CY_PROT_PERM_R, /* Allow all to have read access (PC=0 has write access) */
        .privPermission = CY_PROT_PERM_R, /* Allow all to have read access (PC=0 has write access) */
        .secure = false,
        .pcMatch = false,
        .pcMask = (uint16_t)PROT_UNITS_DEVICE_PC_MASK
    };
#endif

/*******************************************************************************
* Function Name: prot_units_init
********************************************************************************
* Summary:
*   Initialize the protection units.
*
*******************************************************************************/
cy_en_prot_status_t prot_units_init(void)
{
    cy_en_prot_status_t status;

    /* Configure Bus masters [CM0+ and CM4] */
    status = Cy_Prot_ConfigBusMaster(CPUSS_MS_ID_CM0, true, true, 0UL); /* Only allow PC=0 */
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_ConfigBusMaster(CPUSS_MS_ID_CM4, true, false, PROT_UNITS_DEVICE_PC_MASK) : status;
    
    /* Configure SMPU slaves */
    /* SMPU 13 - CM0+ App Flash */
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_ConfigSmpuSlaveStruct(PROT_SMPU_SMPU_STRUCT13, &cm0p_flash_prot_cfg_s) : status;
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_EnableSmpuSlaveStruct(PROT_SMPU_SMPU_STRUCT13) : status;

    /* SMPU 12 - CM0+ App SRAM */
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_ConfigSmpuSlaveStruct(PROT_SMPU_SMPU_STRUCT12, &cm0p_sram_prot_cfg_s) : status;
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_EnableSmpuSlaveStruct(PROT_SMPU_SMPU_STRUCT12) : status;

    /* SMPU 11 - CM4 App Flash */
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_ConfigSmpuSlaveStruct(PROT_SMPU_SMPU_STRUCT11, &cm4_flash_prot_cfg_s) : status;
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_EnableSmpuSlaveStruct(PROT_SMPU_SMPU_STRUCT11) : status;

    /* SMPU 10 - Shared SRAM */
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_ConfigSmpuSlaveStruct(PROT_SMPU_SMPU_STRUCT10, &shared_sram_prot_cfg_s) : status;
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_EnableSmpuSlaveStruct(PROT_SMPU_SMPU_STRUCT10) : status;

    /* SMPU 9 - CM4 App SRAM */
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_ConfigSmpuSlaveStruct(PROT_SMPU_SMPU_STRUCT9, &cm4_sram_prot_cfg_s) : status;
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_EnableSmpuSlaveStruct(PROT_SMPU_SMPU_STRUCT9) : status;
    
#if(CY_IP_MXPERI_VERSION == 1u)
    /* PPU RG - MS_CTL (Bus master control in SMPU) */
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_ConfigPpuFixedRgSlaveStruct(PERI_GR_PPU_RG_SMPU, &ms_ctl_prot_cfg_s) : status;
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_ConfigPpuFixedRgMasterStruct(PERI_GR_PPU_RG_SMPU, &rg_prot_cfg_m) : status;
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_EnablePpuFixedRgSlaveStruct(PERI_GR_PPU_RG_SMPU) : status;
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_EnablePpuFixedRgMasterStruct(PERI_GR_PPU_RG_SMPU) : status;
#else
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_ConfigPpuFixedSlaveAtt(PERI_MS_PPU_FX_PROT_SMPU_MAIN, (uint16_t)0xFFFFu, CY_PROT_PERM_DISABLED, CY_PROT_PERM_DISABLED, true) : status;
#endif
    /************************************************************
    * Take control of the master structs of protection units
    * that can possibly be used to override the protection
    * settings or initiate a denial-of-service attack.
    ************************************************************/
    for(uint8_t idx = 0u; idx < CPUSS_PROT_SMPU_STRUCT_NR; idx++)
    {
        /* Master structs for SMPU 15-0 */
        status = (status == CY_PROT_SUCCESS) ? Cy_Prot_ConfigSmpuMasterStruct((PROT_SMPU_SMPU_STRUCT_Type*) &PROT->SMPU.SMPU_STRUCT[idx], &smpu_prot_cfg_m) : status;
        status = (status == CY_PROT_SUCCESS) ? Cy_Prot_EnableSmpuMasterStruct((PROT_SMPU_SMPU_STRUCT_Type*) &PROT->SMPU.SMPU_STRUCT[idx]) : status;

        /* Master structs for PROG PPU 15-0 */
    #if(CY_IP_MXPERI_VERSION == 1u)    
        status = (status == CY_PROT_SUCCESS) ? Cy_Prot_ConfigPpuProgMasterStruct((PERI_PPU_PR_Type*) &PERI->PPU_PR[idx], &prog_prot_cfg_m) : status;
        status = (status == CY_PROT_SUCCESS) ? Cy_Prot_EnablePpuProgMasterStruct((PERI_PPU_PR_Type*) &PERI->PPU_PR[idx]) : status;
    #else
        status = (status == CY_PROT_SUCCESS) ? Cy_Prot_ConfigPpuProgMasterAtt(((PERI_MS_PPU_PR_Type*) &PERI_MS->PPU_PR[idx]), (uint16_t)PROT_UNITS_DEVICE_PC_MASK, CY_PROT_PERM_R, CY_PROT_PERM_R, true) : status;
    #endif

        if(status != CY_PROT_SUCCESS)
        {
            break;
        }
    }
    
    /* Transition the bus master PC values */
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_SetActivePC(CPUSS_MS_ID_CM4, CY_PROT_PC4) : status;
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_SetActivePC(CPUSS_MS_ID_TC, CY_PROT_PC1) : status;
    status = (status == CY_PROT_SUCCESS) ? Cy_Prot_SetActivePC(CPUSS_MS_ID_CM0, CY_PROT_PC1) : status;
    
    return status;
}