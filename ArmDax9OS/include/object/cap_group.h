#pragma once
#include <common/types.h>
#include <common/list.h>
#include <common/lock.h>
#include <object/object.h>
#include <stdbool.h>

/* 能力类型定义 */
typedef enum {
    CAP_NULL = 0,
    CAP_PROCESS,
    CAP_THREAD,
    CAP_VMSPACE,
    CAP_IRQ,
    CAP_IO,
    CAP_TYPE_MAX
} cap_type_t;

/* 能力权限位 */
#define CAP_READ    (1 << 0)
#define CAP_WRITE   (1 << 1)
#define CAP_EXEC    (1 << 2)
#define CAP_DELEG   (1 << 3)

/* 单个能力结构 */
struct capability {
    cap_type_t type;      // 能力类型
    u32 rights;           // 权限位图
    capid_t capid;        // 能力ID
    void *object;         // 指向实际对象的指针
    struct list_head node; // 能力组链表节点
};

/* 能力组结构 */
struct cap_group {
    struct spin_lock lock;       // 保护能力组的锁
    struct list_head caps_list;  // 能力链表
    capid_t capid;               // 能力组ID
    tid_t owner;                 // 所属线程ID
    u32 cap_count;               // 能力数量
};

/* 全局能力管理器 */
struct cap_manager {
    struct spin_lock lock;
    struct list_head groups_list;
    u32 group_count;
};

/* 能力组管理 */
int get_capgroup(capid_t *capid);
int set_capgroup(capid_t capid);

/* 能力操作 */
int cap_create(capid_t *capid, cap_type_t type, void *object, u32 rights);
int cap_destroy(capid_t capid);
int cap_copy(capid_t src_cap, capid_t *dst_cap);
int cap_transfer(capid_t src_cap, capid_t dst_group, capid_t *dst_cap);
int cap_query(capid_t capid, cap_type_t *type, u32 *rights);
int cap_modify(capid_t capid, u32 rights);

/* 能力组操作 */
int cap_group_create(capid_t *capid);
int cap_group_destroy(capid_t capid);
int cap_group_query(capid_t capid, u32 *cap_count);

/* 权限检查 */
bool cap_check(capid_t capid, u32 required_rights);
