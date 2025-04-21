#include <common/radix.h>
#include <common/types.h>
#include <common/lock.h>
#include <mm/kmalloc.h>

/* 初始化radix树 */
void radix_tree_init(radix_tree_t *tree) {
    tree->root = NULL;
    tree->height = 0;
    spin_lock_init(&tree->lock);
}

/* 销毁radix树 */
void radix_tree_destroy(radix_tree_t *tree) {
    if (!tree) return;
    
    spin_lock_acquire(&tree->lock);
    if (tree->root) {
        radix_node_free(tree->root);
        tree->root = NULL;
    }
    tree->height = 0;
    spin_lock_release(&tree->lock);
}

/* 创建新节点 */
static radix_node_t *radix_node_create(void) {
    radix_node_t *node = kmalloc(sizeof(radix_node_t));
    if (!node) return NULL;
    
    memset(node->children, 0, sizeof(node->children));
    node->value = NULL;
    spin_lock_init(&node->lock);
    return node;
}

/* 插入节点 */
int radix_tree_insert(radix_tree_t *tree, u64 key, void *value) {
    if (!tree) return -1;
    
    radix_node_t **slot = &tree->root;
    u64 index;
    int level = 0;
    
    while (level < RADIX_MAX_LEVELS) {
        if (!*slot) {
            *slot = radix_node_create();
            if (!*slot) return -1;
        }
        
        index = (key >> (RADIX_BITS * (RADIX_MAX_LEVELS - level - 1))) & RADIX_MASK;
        slot = &((*slot)->children[index]);
        
        if (level == RADIX_MAX_LEVELS - 1) {
            (*slot)->value = value;
            return 0;
        }
        level++;
    }
    return -1;
}

/* 查找节点 */
void *radix_tree_lookup(radix_tree_t *tree, u64 key) {
    if (!tree || !tree->root) return NULL;
    
    radix_node_t *node = tree->root;
    u64 index;
    int level = 0;
    
    while (level < RADIX_MAX_LEVELS) {
        index = (key >> (RADIX_BITS * (RADIX_MAX_LEVELS - level - 1))) & RADIX_MASK;
        
        if (!node->children[index]) 
            return NULL;
            
        node = node->children[index];
        
        if (level == RADIX_MAX_LEVELS - 1) 
            return node->value;
            
        level++;
    }
    return NULL;
}

/* 递归释放节点 */
static void radix_node_free(radix_node_t *node) {
    if (!node) return;
    
    for (int i = 0; i < (1 << RADIX_BITS); i++) {
        if (node->children[i]) {
            radix_node_free(node->children[i]);
        }
    }
    kfree(node);
}

/* 删除节点 */
int radix_tree_delete(radix_tree_t *tree, u64 key) {
    if (!tree || !tree->root) return -1;
    
    radix_node_t **slot = &tree->root;
    radix_node_t *parent = NULL;
    u64 index;
    int level = 0;
    
    while (level < RADIX_MAX_LEVELS) {
        index = (key >> (RADIX_BITS * (RADIX_MAX_LEVELS - level - 1))) & RADIX_MASK;
        
        if (!(*slot)->children[index])
            return -1;
            
        parent = *slot;
        slot = &((*slot)->children[index]);
        
        if (level == RADIX_MAX_LEVELS - 1) {
            (*slot)->value = NULL;
            /* 如果节点没有子节点则释放 */
            int has_children = 0;
            for (int i = 0; i < (1 << RADIX_BITS); i++) {
                if ((*slot)->children[i]) {
                    has_children = 1;
                    break;
                }
            }
            if (!has_children) {
                kfree(*slot);
                parent->children[index] = NULL;
            }
            return 0;
        }
        level++;
    }
    return -1;
}

/* 线程安全的查找 */
void *radix_tree_lookup_safe(radix_tree_t *tree, u64 key) {
    void *value;
    spin_lock_acquire(&tree->lock);
    value = radix_tree_lookup(tree, key);
    spin_lock_release(&tree->lock);
    return value;
}

/* 线程安全的插入 */
int radix_tree_insert_safe(radix_tree_t *tree, u64 key, void *value) {
    int ret;
    spin_lock_acquire(&tree->lock);
    ret = radix_tree_insert(tree, key, value);
    spin_lock_release(&tree->lock);
    return ret;
}

/* 线程安全的删除 */
int radix_tree_delete_safe(radix_tree_t *tree, u64 key) {
    int ret;
    spin_lock_acquire(&tree->lock);
    ret = radix_tree_delete(tree, key);
    spin_lock_release(&tree->lock);
    return ret;
}

/* 初始化迭代器 */
void radix_tree_iter_init(radix_tree_iter_t *iter, radix_tree_t *tree) {
    iter->node = tree->root;
    iter->key = 0;
    iter->level = 0;
}

/* 获取下一个节点 */
void *radix_tree_iter_next(radix_tree_iter_t *iter) {
    if (!iter || !iter->node) return NULL;
    
    /* 深度优先遍历 */
    for (int i = 0; i < (1 << RADIX_BITS); i++) {
        if (iter->node->children[i]) {
            iter->node = iter->node->children[i];
            iter->level++;
            iter->key = (iter->key << RADIX_BITS) | i;
            
            if (iter->level == RADIX_MAX_LEVELS - 1) {
                return iter->node->value;
            }
            return radix_tree_iter_next(iter);
        }
    }
    
    /* 回溯到父节点 */
    if (iter->level > 0) {
        iter->node = NULL; // 标记需要回溯
        return NULL;
    }
    
    return NULL;
}
