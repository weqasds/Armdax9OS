// 可执行文件格式
#pragma once

#include <common/types.h>

// ELF文件类型
#define ET_NONE   0
#define ET_REL    1
#define ET_EXEC   2
#define ET_DYN    3
#define ET_CORE   4

// ELF机器类型
#define EM_ARM    40

// ELF版本
#define EV_CURRENT 1

// ELF标识位
#define EI_NIDENT 16

// ELF头结构
typedef struct {
    unsigned char e_ident[EI_NIDENT]; // ELF标识
    u16 e_type;                      // 文件类型
    u16 e_machine;                   // 机器架构
    u32 e_version;                   // ELF版本
    u64 e_entry;                     // 程序入口点
    u64 e_phoff;                     // 程序头表偏移
    u64 e_shoff;                     // 节头表偏移
    u32 e_flags;                     // 处理器特定标志
    u16 e_ehsize;                    // ELF头大小
    u16 e_phentsize;                 // 程序头表项大小
    u16 e_phnum;                     // 程序头表项数量
    u16 e_shentsize;                 // 节头表项大小
    u16 e_shnum;                     // 节头表项数量
    u16 e_shstrndx;                  // 节名字符串表索引
} Elf64_Ehdr;

// 程序头类型
#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_TLS     7

// 程序头标志
#define PF_X 0x1 // 可执行
#define PF_W 0x2 // 可写
#define PF_R 0x4 // 可读

// 程序头结构
typedef struct {
    u32 p_type;   // 段类型
    u32 p_flags;  // 段标志
    u64 p_offset; // 段在文件中的偏移
    u64 p_vaddr;  // 段在内存中的虚拟地址
    u64 p_paddr;  // 段在内存中的物理地址
    u64 p_filesz; // 段在文件中的大小
    u64 p_memsz;  // 段在内存中的大小
    u64 p_align;  // 段对齐
} Elf64_Phdr;

// ELF加载状态
typedef struct {
    struct vmspace *vmspace; // 目标地址空间
    vaddr_t entry;           // 入口地址
    size_t phnum;            // 程序头数量
    Elf64_Phdr *phdrs;       // 程序头数组
} elf_loader_t;

// ELF错误码
#define ELF_EINVAL      -1  // 无效ELF文件
#define ELF_ENOTSUP     -2  // 不支持的类型
#define ELF_ENOMEM      -3  // 内存不足
#define ELF_EBADF       -4  // 文件错误
#define ELF_EACCESS     -5  // 权限错误

// ELF段映射信息
struct elf_segment {
    vaddr_t vaddr;      // 虚拟地址
    size_t size;        // 大小
    u32 prot;           // 保护标志
    char is_executable; // 是否可执行
};

// ELF接口函数
int elf_load(struct vmspace *vmspace, const char *path, vaddr_t *entry);
int elf_validate(const Elf64_Ehdr *ehdr);
int elf_map_segment(struct vmspace *vmspace, const Elf64_Phdr *phdr, const void *data);
int elf_read_header(int fd, Elf64_Ehdr *ehdr);
int elf_read_program_headers(int fd, const Elf64_Ehdr *ehdr, Elf64_Phdr **phdrs);
int elf_load_segments(struct vmspace *vmspace, int fd, const Elf64_Ehdr *ehdr,
                     const Elf64_Phdr *phdrs);
void elf_free_loader(elf_loader_t *loader);
