#include <lib/elf.h>
#include <mm/vmspace.h>
#include <common/types.h>
#include <common/error.h>
#include <common/debug.h>
#include <object/object.h>
#include <arch/arm/mmu.h>

// 页大小和对齐宏
#define PAGE_SIZE 4096
#define ALIGN_DOWN(addr, align) ((addr) & ~((align) - 1))
#define ALIGN_UP(addr, align) (((addr) + (align) - 1) & ~((align) - 1))

// 验证ELF文件头
int elf_validate(const Elf64_Ehdr *ehdr) {
    // 检查魔数
    if (ehdr->e_ident[0] != 0x7f || 
        ehdr->e_ident[1] != 'E' ||
        ehdr->e_ident[2] != 'L' ||
        ehdr->e_ident[3] != 'F') {
        return ELF_EINVAL;
    }

    // 检查ELF类别(64位)
    if (ehdr->e_ident[4] != 2) {
        return ELF_ENOTSUP;
    }

    // 检查字节序(小端)
    if (ehdr->e_ident[5] != 1) {
        return ELF_ENOTSUP;
    }

    // 检查版本
    if (ehdr->e_ident[6] != EV_CURRENT || ehdr->e_version != EV_CURRENT) {
        return ELF_ENOTSUP;
    }

    // 检查机器类型(ARM)
    if (ehdr->e_machine != EM_ARM) {
        return ELF_ENOTSUP;
    }

    return 0;
}


// 读取ELF头
int elf_read_header(int fd, Elf64_Ehdr *ehdr) {
    ssize_t ret = vfs_read(fd, ehdr, sizeof(Elf64_Ehdr), 0);
    if (ret != sizeof(Elf64_Ehdr)) {
        return ELF_EBADF;
    }
    return 0;
}

// 读取程序头表
int elf_read_program_headers(int fd, const Elf64_Ehdr *ehdr, Elf64_Phdr **phdrs) {
    size_t phdr_size = ehdr->e_phnum * ehdr->e_phentsize;
    *phdrs = kmalloc(phdr_size);
    if (!*phdrs) {
        return ELF_ENOMEM;
    }

    ssize_t ret = vfs_read(fd, *phdrs, phdr_size, ehdr->e_phoff);
    if (ret != phdr_size) {
        kfree(*phdrs);
        *phdrs = NULL;
        return ELF_EBADF;
    }
    return 0;
}

// 读取段数据
static int elf_read_segment(int fd, const Elf64_Phdr *phdr, void **data) {
    if (phdr->p_filesz == 0) {
        *data = NULL;
        return 0;
    }

    *data = kmalloc(phdr->p_filesz);
    if (!*data) {
        return ELF_ENOMEM;
    }

    ssize_t ret = vfs_read(fd, *data, phdr->p_filesz, phdr->p_offset);
    if (ret != phdr->p_filesz) {
        kfree(*data);
        *data = NULL;
        return ELF_EBADF;
    }
    return 0;
}

// 映射ELF段到内存
int elf_map_segment(struct vmspace *vmspace, const Elf64_Phdr *phdr, const void *data) {
    u32 prot = 0;
    if (phdr->p_flags & PF_R) prot |= VMR_READ;
    if (phdr->p_flags & PF_W) prot |= VMR_WRITE;
    if (phdr->p_flags & PF_X) prot |= VMR_EXEC;

    // 计算对齐后的地址和大小
    vaddr_t vaddr = ALIGN_DOWN(phdr->p_vaddr, PAGE_SIZE);
    size_t size = ALIGN_UP(phdr->p_vaddr + phdr->p_memsz, PAGE_SIZE) - vaddr;

    // 分配虚拟内存
    int ret = vmspace_map(vmspace, vaddr, size, prot);
    if (ret < 0) {
        return ret;
    }

    // 复制段数据
    if (phdr->p_filesz > 0) {
        void *dst = (void*)phdr->p_vaddr;
        memcpy(dst, data, phdr->p_filesz);
    }

    // 清零BSS段
    if (phdr->p_memsz > phdr->p_filesz) {
        void *bss_start = (void*)(phdr->p_vaddr + phdr->p_filesz);
        size_t bss_size = phdr->p_memsz - phdr->p_filesz;
        memset(bss_start, 0, bss_size);
    }

    return 0;
}

// 加载ELF所有段
int elf_load_segments(struct vmspace *vmspace, int fd, 
                     const Elf64_Ehdr *ehdr, const Elf64_Phdr *phdrs) {
    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdrs[i].p_type == PT_LOAD) {
            // TODO: 实现段加载逻辑
            int ret = elf_map_segment(vmspace, &phdrs[i], NULL);
            if (ret < 0) {
                return ret;
            }
        }
    }
    return 0;
}

// 加载ELF文件
int elf_load(struct vmspace *vmspace, const char *path, vaddr_t *entry) {
    Elf64_Ehdr ehdr;
    Elf64_Phdr *phdrs = NULL;
    int fd = -1;
    int ret = 0;

    // TODO: 打开文件获取fd

    // 读取并验证ELF头
    ret = elf_read_header(fd, &ehdr);
    if (ret < 0) {
        goto out;
    }

    ret = elf_validate(&ehdr);
    if (ret < 0) {
        goto out;
    }

    // 读取程序头表
    ret = elf_read_program_headers(fd, &ehdr, &phdrs);
    if (ret < 0) {
        goto out;
    }

    // 加载所有段
    ret = elf_load_segments(vmspace, fd, &ehdr, phdrs);
    if (ret < 0) {
        goto out;
    }

    // 设置入口地址
    *entry = ehdr.e_entry;

out:
    if (phdrs) {
        kfree(phdrs);
    }
    if (fd >= 0) {
        // TODO: 关闭文件
    }
    return ret;
}

// 释放ELF加载器资源
void elf_free_loader(elf_loader_t *loader) {
    if (loader->phdrs) {
        kfree(loader->phdrs);
    }
    memset(loader, 0, sizeof(*loader));
}