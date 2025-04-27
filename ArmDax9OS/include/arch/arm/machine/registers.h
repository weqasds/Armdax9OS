#pragma once
#include <common/types.h>
#include <common/macro.h>

/* 处理器状态寄存器(SPSR)位域定义 */
#define SPSR_MODE_EL0    0x0
#define SPSR_MODE_EL1    0x4


//异常返回时目标执行等级和栈选择其中,t:选择的栈指针 h:选则eln指针
#define SPSR_EL1_EL0t SPSR_MODE_EL0
#define SPSR_EL1_EL1t SPSR_MODE_EL1
#define SPSR_EL1_EL1h 0b0101
#define SPSR_EL1_KERNEL SPSR_EL1_EL1h
#define SPSR_EL1_USER   SPSR_EL1_EL0t
//掩码
#define SPSR_MASK        0xF

#define SPSR_EL1_N_BIT         BIT(31) // 负标志
#define SPSR_EL1_Z_BIT         BIT(30) // 零标志
#define SPSR_EL1_C_BIT         BIT(29) // 进位标志
#define SPSR_EL1_V_BIT         BIT(28) // 溢出标志
#define SPSR_EL1_TC0_BIT       BIT(25) //
#define SPSR_EL1_DIT_BIT       BIT(24) // Data Independent Timing
#define SPSR_EL1_UAO_BIT       BIT(23) // User Access Override
#define SPSR_EL1_PAN_BIT       BIT(22) // Privileged Access Never
#define SPSR_EL1_SS_BIT        BIT(21) // 单步执行位
#define SPSR_EL1_IL_BIT        BIT(20) // 非法执行状态位
#define SPSR_EL1_ALLINT_BIT    BIT(13)   
#define SPSR_EL1_SSBS_BIT      BIT(12) // 投机写旁路安全
#define SPSR_EL1_BTYPE_BIT     BIT(11) // BTYPE  状态位
#define SPSR_EL1_DEBUG_BIT     BIT(9)  // Endian 状态位
#define SPSR_EL1_A_BIT         BIT(8)  // SError屏蔽位
#define SPSR_EL1_IRQ_BIT       BIT(7)  // IRQ屏蔽位
#define SPSR_EL1_FIQ_BIT       BIT(6)  // FIQ屏蔽位
#define SPSR_EL1_M_BIT         BIT(4)  // 指明异常源（AArch64 还是 AArch32）

/**/
#define CTR_EL0_DIC      BIT(29)
#define CTR_EL0_IDC      BIT(28)
#define CTR_EL0_L1Tp     0b11
// 0b1100,0000,0000,0000
#define CTR_EL0_L1Tp_SHIFT 14



/* 系统控制寄存器(SCTLR)位域定义 */
#define SCTLR_EL1_EnIA                                         \
        BIT(31) /* Controls enabling of pointer authentication \
                   启用指令地址验证（PAC 指令）                  \
                 */
#define SCTLR_EL1_EnIB                                         \
        BIT(30) /* Controls enabling of pointer authentication \
                   	启用分支地址验证（PAC 分支）                 \
                 */
#define SCTLR_EL1_LSMAOE                                                   \
        BIT(29) /* Load Multiple and Store Multiple Atomicity and Ordering \
                   启用 LDM/STM 指令的原子性和顺序性                         \
                   Enable */
#define SCTLR_EL1_nTLSMD                                       \
        BIT(28) /* No Trap Load Multiple and Store Multiple to   \
                   Device-nGRE/Device-nGnRE/Device-nGnRnE memory \
                   禁止 LDM/STM 到 Device 类型内存触发异常         \
                   */
#define SCTLR_EL1_EnDA                                         \
        BIT(27) /* Controls enabling of pointer authentication \
                   启用数据地址验证（PAC 数据）                  \
                 */
#define SCTLR_EL1_UCI                                                       \
        BIT(26) /* Traps EL0 execution of cache maintenance instructions to \
                   EL1, from AArch64 state only 陷阱用户态执行 cache 操作  */
#define SCTLR_EL1_EE                                                           \
        BIT(25) /* Endianness of data accesses at EL1, and stage 1 translation \
                   table walks in the EL1&0 translation regime \
                   设置 EL1 的数据访问大小端（1=大端） */
#define SCTLR_EL1_E0E BIT(24) /* Endianness of data accesses at EL0 \
                                设置 EL0 的数据访问大小端（1=大端）*/
#define SCTLR_EL1_SPAN                                                       \
        BIT(23) /* Set Privileged Access Never, on taking an exception to EL1 \
                   在异常返回 EL1 时自动启用 PAN                              \
                 */
#define SCTLR_EL1_IESB                                                      \
        BIT(21) /* Implicit error synchronization event enable              \
                  启用隐式错误同步屏障（Error Synchronization Barrier）       \
                 */
#define SCTLR_EL1_WXN                                          \
        BIT(19) /* Write permission implies XN (Execute-never) \
                  写权限隐含不可执行（写不可执行）                \
                 */
#define SCTLR_EL1_nTWE                                                       \
        BIT(18) /* Traps EL0 execution of WFE instructions to EL1, from both \
                Execution states陷阱 EL0 执行 WFE 到 EL1  */
#define SCTLR_EL1_nTWI                                                       \
        BIT(16) /* Traps EL0 execution of WFI instructions to EL1, from both \
                   Execution states 陷阱 EL0 执行 WFI 到 EL1 */
#define SCTLR_EL1_UCT                                                     \
        BIT(15) /* Traps EL0 accesses to the CTR_EL0 to EL1, from AArch64 \
                   	state only                                            \
                    陷阱 EL0 访问 CTR_EL0 */
#define SCTLR_EL1_DZE                                                      \
        BIT(14) /* Traps EL0 execution of DC ZVA instructions to EL1, from \
                   AArch64 state only。陷阱 EL0 执行 DC ZVA 清零指令 */
#define SCTLR_EL1_EnDB                                         \
        BIT(13) /* Controls enabling of pointer authentication \
                   	启用数据分支验证                           \
                 */
#define SCTLR_EL1_I                                                         \
        BIT(12) /* Instruction access Cacheability control, for accesses at \
                   EL0 and EL1  \
                   启用指令缓存 \
                   */
#define SCTLR_EL1_UMA                                                  \
        BIT(9) /* User Mask Access: Traps EL0 execution of MSR and MRS \
                  instructions that access the PSTATE.{D, A, I, F} 	   \
                  启用用户态修改 PSTATE（A/I/F）标志 */
#define SCTLR_EL1_SED                                                         \
        BIT(8) /* SETEND instruction disable. Disables SETEND instructions at \
                  EL0 using AArch32  禁用 SETEND 指令（控制字节序） */
#define SCTLR_EL1_ITD                                                      \
        BIT(7) /* IT Disable. Disables some uses of IT instructions at EL0 \
                  using AArch32 禁用 IT（If-Then）条件执行指令 */
#define SCTLR_EL1_nAA                                                 \
        BIT(6) /* Non-aligned access. This bit controls generation of \
                  Alignment faults at EL1 and EL0 under certain conditions \
                  禁止非对齐访问（0=允许，1=禁止） */
#define SCTLR_EL1_CP15BEN \
        BIT(5) /* System instruction memory barrier enable (AArch32) \
                  启用 AArch32 的 barrier 指令（如 ISB） */
#define SCTLR_EL1_SA0 BIT(4) /* SP Alignment check enable for EL0 \
                             	启用 EL0 栈指针对齐检查 */ 
#define SCTLR_EL1_SA  BIT(3) /* SP Alignment check \
                                启用 EL1 栈指针对齐检查 */
#define SCTLR_EL1_C   BIT(2) /* Cacheability control for data accesses */
#define SCTLR_EL1_A   BIT(1) /* Alignment check enable */
#define SCTLR_EL1_M \
        BIT(0) /* MMU enable for EL1 and EL0 stage 1 address translation \
                  启用 MMU（内存管理单元）*/


/* 寄存器枚举 */
//注意x为8字节，即每个寄存器间隔8字节
typedef enum {
    // 通用寄存器
    REG_X0=0, /* 0x00 */ 
    REG_X1=1, /* 0x08 */ 
    REG_X2=2, /* 0x10 */ 
    REG_X3=3, /* 0x18 */ 
    REG_X4=4, /* 0x20 */
    REG_X5=5, /* 0x28 */ 
    REG_X6=6, /* 0x30 */ 
    REG_X7=7, /* 0x38 */ 
    REG_X8=8, /* 0x40 */ 
    REG_X9=9, /* 0x48 */
    REG_X10=10, /* 0x50 */ 
    REG_X11=11, /* 0x58 */ 
    REG_X12=12, /* 0x60 */ 
    REG_X13=13, /* 0x68 */ 
    REG_X14=14, /* 0x70 */
    REG_X15=15, /* 0x78 */ 
    REG_X16=16, /* 0x80 */ 
    REG_X17=17, /* 0x88 */ 
    REG_X18=18, /* 0x90 */ 
    REG_X19=19, /* 0x98 */
    REG_X20=20, /* 0xa0 */ 
    REG_X21=21, /* 0xa8 */ 
    REG_X22=22, /* 0xb0 */ 
    REG_X23=23, /* 0xb8 */ 
    REG_X24=24, /* 0xc0 */
    REG_X25=25, /* 0xc8 */ 
    REG_X26=26, /* 0xd0 */ 
    REG_X27=27, /* 0xd8 */ 
    REG_X28=28, /* 0xe0 */ 
    REG_X29=29, /* 0xe8 */
    REG_X30=30, /* 0xf0 *///也是REG_LR链接寄存器
    REG_SP,        // 栈指针
    REG_SPSR,      // 程序状态寄存器
    REG_ELR_EL1,   // 异常链接寄存器
} reg_enum_t;
#define REG_COUNT (34)          // 寄存器总数


/* 寄存器访问接口 */
#define read_sysreg(reg) ({                         \
    u64 __val;                                      \
    __asm__ volatile("mrs %0, " #reg : "=r"(__val));\
    __val;                                          \
})
#define write_sysreg(reg, val) \
    __asm__ volatile("msr " #reg ", %0" :: "r"(val))

/* 常用寄存器操作宏 */
#define READ_SCTLR_EL1()    read_sysreg(sctlr_el1)
#define WRITE_SCTLR_EL1(v)  write_sysreg(sctlr_el1, v)
#define READ_SPSR_EL1()     read_sysreg(spsr_el1)
#define WRITE_SPSR_EL1(v)   write_sysreg(spsr_el1, v)
#define READ_ELR_EL1()      read_sysreg(elr_el1)
#define WRITE_ELR_EL1(v)    write_sysreg(elr_el1, v)


/* SMP启动相关寄存器 */
#define __smp_up_entry      "S3_1_C15_C2_0"  // CPU启动入口地址寄存器
#define __smp_up_coreid     "S3_1_C15_C2_1"  // CPU核心ID寄存器
