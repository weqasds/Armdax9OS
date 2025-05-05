# 树莓派3B MMU内存映射图

```mermaid
flowchart TD
    %% 页表结构
    TTBR0["TTBR0 (用户空间)"] --> L0_0["L0页表(用户)"]
    TTBR1["TTBR1 (内核空间)"] --> L0_1["L0页表(内核)"]
    
    L0_0 --> L1_0["L1页表"]
    L0_1 --> L1_1["L1页表"]
    
    L1_0 --> L2_0["L2页表"]
    L1_1 --> L2_1["L2页表"]

    %% 物理内存区域
    PHYS_MEM["物理内存(1GB)"]
    NORMAL["普通内存\n0x0-0x3EFFFFFF"]
    PERIPH["外设内存\n0x3F000000-0x3FFFFFFF"]
    PHYS_MEM --> NORMAL
    PHYS_MEM --> PERIPH

    %% 虚拟地址映射
    USER_SPACE["用户空间"] --> NORMAL
    USER_SPACE --> PERIPH
    KERNEL_SPACE["内核空间\nKERNEL_VADDR_BASE+..."] --> NORMAL
    KERNEL_SPACE --> PERIPH

    %% 内存属性
    NORMAL_ATTR["普通内存属性:\nNORMAL_MEMORY\nINNER_SHARABLE"]
    PERIPH_ATTR["外设内存属性:\nDEVICE_MEMORY\nUXN"]
    USER_ATTR["用户空间附加:\nNG\nUXN"]

    %% 页表映射关系
    L2_0 --> NORMAL
    L2_0 --> PERIPH
    L2_1 --> NORMAL
    L2_1 --> PERIPH

    %% 属性关联
    NORMAL --> NORMAL_ATTR
    PERIPH --> PERIPH_ATTR
    L2_0 --> USER_ATTR
```

## 关键说明
1. **三级页表结构**:
   - L0: 顶级页表，由TTBR0/TTBR1指向
   - L1: 中间级页表
   - L2: 最终级页表，包含实际物理页映射

2. **内存区域划分**:
   - 普通内存: 0x00000000-0x3EFFFFFF (992MB)
   - 外设内存: 0x3F000000-0x3FFFFFFF (16MB)

3. **映射特性**:
   - 用户空间(TTBR0): 直接映射，附加NG和UXN属性
   - 内核空间(TTBR1): 高位地址映射，共享相同物理内存

4. **内存属性**:
   - 普通内存: 可缓存、内部共享
   - 外设内存: 设备内存、不可执行