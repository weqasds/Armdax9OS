/*Exception sync Registers*/
#pragma once
#include <common/types.h>

#include <common/macro.h>
/* ESR_EL1 Exception Syndrome Register */
/* Exception Class, ESR bits[31:26].
 * A subset of exception is recoreded here. */
//EC掩码，
#define EC_MASK                 (BIT(6) - 1)//0b1000000-1=0b111111
#define ESR_EL1_EC_SHIFT        26
#define GET_ESR_EL1_EC(esr_el1) (((esr_el1) >> ESR_EL1_EC_SHIFT) & EC_MASK)

#define ESR_EL1_EC_UNKNOWN 0b000000
#define ESR_EL1_EC_WFI_WFE \
        0b000001 /* Trapped WFI or WFE instruction execution */
#define ESR_EL1_EC_ENFP                                             \
        0b000111 /* Access to SVE, Advanced SIMD, or floating-point \
                    functionality */
#define ESR_EL1_EC_ILLEGAL_EXEC 0b001110 /* Illegal Execution state */
#define ESR_EL1_EC_SVC_32 \
        0b010001 /* SVC instruction execution in AArch32 state */
#define ESR_EL1_EC_SVC_64 \
        0b010101 /* SVC instruction execution in AArch64 state */
#define ESR_EL1_EC_MRS_MSR_64                                           \
        0b011000 /* Trapped MSR, MRS or System instruction execution in \
                    AArch64 state */
#define ESR_EL1_EC_IABT_LEL \
        0b100000 /* Instruction Abort from a lower Exception level) */
#define ESR_EL1_EC_IABT_CEL \
        0b100001 /* Instruction Abort from a current Exception level */
#define ESR_EL1_EC_PC_ALIGN 0b100010 /* PC alignment fault exception */
#define ESR_EL1_EC_DABT_LEL \
        0b100100 /* Data Abort from a lower Exception level */
#define ESR_EL1_EC_DABT_CEL \
        0b100101 /* Data Abort from a current Exception level */
#define ESR_EL1_EC_SP_ALIGN 0b100110 /* SP alignment fault exception */
#define ESR_EL1_EC_FP_32                                                      \
        0b101000 /* Trapped floating-point exception taken from AArch32 state \
                  */
#define ESR_EL1_EC_FP_64                                                      \
        0b101100 /* Trapped floating-point exception taken from AArch64 state \
                  */
#define ESR_EL1_EC_SError 0b101111 /* SERROR */
#define ESR_EL1_EC_SMC    0b010111 /* SMC instruction executed in AArch64 state */
#define ESR_EL1_EC_UNDEFINED 0b001100 /* Undefined instruction */
#define ESR_EL1_EC_BRK    0b110000 /* Breakpoint instruction (BRK) */

/* Instruction Length for synchronous exceptions */
// #define ESR_EL1_IL                  BIT(25)

/* ESR_EL1_ISS, ESR bits[24:0] is relay on the specific cases */

/* ISS fields for SMC instructions */
//iss字段值（有ec值决定显式的错误码）
#define ESR_EL1_SMC_IMM16_MASK  0xFFFF
#define ESR_EL1_SMC_IMM16_SHIFT 0
#define GET_ESR_EL1_SMC_IMM16(esr_el1) (((esr_el1) >> ESR_EL1_SMC_IMM16_SHIFT) & ESR_EL1_SMC_IMM16_MASK)

/* ISS fields for BRK instructions */
#define ESR_EL1_BRK_COND_MASK   0xF
#define ESR_EL1_BRK_COND_SHIFT  20
#define ESR_EL1_BRK_IS16BIT     BIT(24)
#define GET_ESR_EL1_BRK_COND(esr_el1) (((esr_el1) >> ESR_EL1_BRK_COND_SHIFT) & ESR_EL1_BRK_COND_MASK)

/* ISS fields for UNDEFINED instruction */
#define ESR_EL1_UNDEF_ISS_MASK  0x1FFFFFF
#define GET_ESR_EL1_UNDEF_ISS(esr_el1) ((esr_el1) & ESR_EL1_UNDEF_ISS_MASK)

/* ISS fields for system register access */
#define ESR_EL1_SYSREG_OP0_MASK  0x3
#define ESR_EL1_SYSREG_OP0_SHIFT 20
#define ESR_EL1_SYSREG_OP1_MASK  0x7
#define ESR_EL1_SYSREG_OP1_SHIFT 16
#define ESR_EL1_SYSREG_CRN_MASK  0xF
#define ESR_EL1_SYSREG_CRN_SHIFT 12
#define ESR_EL1_SYSREG_CRM_MASK  0xF
#define ESR_EL1_SYSREG_CRM_SHIFT 8
#define ESR_EL1_SYSREG_OP2_MASK  0x7
#define ESR_EL1_SYSREG_OP2_SHIFT 5

/* IFSC & DFSC */
#define FSC_MASK                 (BIT(6) - 1)
#define ESR_EL1_FSC_SHIFT        0
#define GET_ESR_EL1_FSC(esr_el1) (((esr_el1) >> ESR_EL1_FSC_SHIFT) & FSC_MASK)

/* Instruction Abort */
//指令异常
/* Instruction Fault Status Code, IFSC, ESR bits[5:0] */
#define IFSC_ADDR_SIZE_FAULT_L0 0b000000
#define IFSC_ADDR_SIZE_FAULT_L1 0b000001
#define IFSC_ADDR_SIZE_FAULT_L2 0b000010
#define IFSC_ADDR_SIZE_FAULT_L3 0b000011
#define IFSC_TRANS_FAULT_L0 0b000100
#define IFSC_TRANS_FAULT_L1 0b000101
#define IFSC_TRANS_FAULT_L2 0b000110
#define IFSC_TRANS_FAULT_L3 0b000111
//#define IFSC_ACCESS_FAULT_L0 0b001000 will not implemented
#define IFSC_ACCESS_FAULT_L1 0b001001
#define IFSC_ACCESS_FAULT_L2 0b001010
#define IFSC_ACCESS_FAULT_L3 0b001011

#define IFSC_PERM_FAULT_L1 0b001101
#define IFSC_PERM_FAULT_L2 0b001110
#define IFSC_PERM_FAULT_L3 0b001111


/* Data Abort */
//数据异常
/* WnR, ESR bit[6]. Write not Read. The cause of data abort. */
#define DABT_BY_READ  0b0
#define DABT_BY_WRITE 0b1

#define WnR_MASK                 1
#define ESR_EL1_WnR_SHIFT        6
#define GET_ESR_EL1_WnR(esr_el1) (((esr_el1) >> ESR_EL1_WnR_SHIFT) & WnR_MASK)

/* Data Fault Status Code, DFSC, ESR bits[5:0] */
#define DFSC_TRANS_FAULT_L0 0b000100
#define DFSC_TRANS_FAULT_L1 0b000101
#define DFSC_TRANS_FAULT_L2 0b000110
#define DFSC_TRANS_FAULT_L3 0b000111

#define DFSC_ACCESS_FAULT_L1 0b001001
#define DFSC_ACCESS_FAULT_L2 0b001010
#define DFSC_ACCESS_FAULT_L3 0b001011

#define DFSC_PERM_FAULT_L1 0b001101
#define DFSC_PERM_FAULT_L2 0b001110
#define DFSC_PERM_FAULT_L3 0b001111
