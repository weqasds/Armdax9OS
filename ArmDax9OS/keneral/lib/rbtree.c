#include <common/rbtree.h>
#include <common/lock.h>
#include <common/list.h>
#include <common/types.h>
void rb_insert(rb_root *root, rb_node *node)
{
    spin_lock_acquire(&root->lock);
    rb_node **new = &root->rb_node, *parent = NULL;
    
    // 查找插入位置
    while (*new) {
        parent = *new;
        new = (node < *new) ? &(*new)->left : &(*new)->right;
    }

    // 设置新节点关系
    rb_set_parent(node, parent);
    node->left = node->right = NULL;
    rb_set_color(node, RB_RED);
    *new = node;

    // 平衡红黑树
    while (node != root->rb_node && rb_get_color(rb_get_parent(node)) == RB_RED) {
        if (rb_get_parent(node) == rb_get_parent(rb_get_parent(node))->left) {
            rb_node *uncle = rb_get_parent(rb_get_parent(node))->right;
            if (uncle && rb_get_color(uncle) == RB_RED) {
                rb_set_color(rb_get_parent(node), RB_BLACK);
                rb_set_color(uncle, RB_BLACK);
                rb_set_color(rb_get_parent(rb_get_parent(node)), RB_RED);
                node = rb_get_parent(rb_get_parent(node));
            } else {
                if (node == rb_get_parent(node)->right) {
                    node = rb_get_parent(node);
                    rb_rotate_left(root, node);
                }
                rb_set_color(rb_get_parent(node), RB_BLACK);
                rb_set_color(rb_get_parent(rb_get_parent(node)), RB_RED);
                rb_rotate_right(root, rb_get_parent(rb_get_parent(node)));
            }
        } else {
            // 对称情况
            rb_node *uncle = rb_get_parent(rb_get_parent(node))->left;
            if (uncle && rb_get_color(uncle) == RB_RED) {
                rb_set_color(rb_get_parent(node), RB_BLACK);
                rb_set_color(uncle, RB_BLACK);
                rb_set_color(rb_get_parent(rb_get_parent(node)), RB_RED);
                node = rb_get_parent(rb_get_parent(node));
            } else {
                if (node == rb_get_parent(node)->left) {
                    node = rb_get_parent(node);
                    rb_rotate_right(root, node);
                }
                rb_set_color(rb_get_parent(node), RB_BLACK);
                rb_set_color(rb_get_parent(rb_get_parent(node)), RB_RED);
                rb_rotate_left(root, rb_get_parent(rb_get_parent(node)));
            }
        }
    }
    rb_set_color(root->rb_node, RB_BLACK);
    spin_lock_release(&root->lock);
}
void rb_erase(rb_root *root, rb_node *node)
{
    spin_lock_acquire(&root->lock);
    rb_node *child, *parent;
    int color;

    if (!node->left) {
        child = node->right;
    } else if (!node->right) {
        child = node->left;
    } else {
        rb_node *old = node, *left;
        node = node->right;
        while ((left = node->left) != NULL)
            node = left;
        child = node->right;
        parent = rb_get_parent(node);
        color = rb_get_color(node);

        if (child)
            rb_set_parent(child, parent);
        if (parent == old) {
            parent->right = child;
        } else {
            parent->left = child;
        }

        node->__parent_color = old->__parent_color;
        node->right = old->right;
        node->left = old->left;

        rb_set_parent(old->left, node);
        if (old->right)
            rb_set_parent(old->right, node);

        goto color;
    }

    parent = rb_get_parent(node);
    color = rb_get_color(node);

    if (child)
        rb_set_parent(child, parent);
    if (parent) {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    } else {
        root->rb_node = child;
    }

color:
    if (color == RB_BLACK)
        rb_erase_color(root, child, parent);
    spin_lock_release(&root->lock);
}

static void rb_erase_color(rb_root *root, rb_node *node, rb_node *parent)
{
    while (node != root->rb_node && (!node || rb_get_color(node) == RB_BLACK)) {
        if (node == parent->left) {
            rb_node *sibling = parent->right;
            if (rb_get_color(sibling) == RB_RED) {
                rb_set_color(sibling, RB_BLACK);
                rb_set_color(parent, RB_RED);
                rb_rotate_left(root, parent);
                sibling = parent->right;
            }
            if ((!sibling->left || rb_get_color(sibling->left) == RB_BLACK) &&
                (!sibling->right || rb_get_color(sibling->right) == RB_BLACK)) {
                rb_set_color(sibling, RB_RED);
                node = parent;
                parent = rb_get_parent(parent);
            } else {
                if (!sibling->right || rb_get_color(sibling->right) == RB_BLACK) {
                    rb_set_color(sibling->left, RB_BLACK);
                    rb_set_color(sibling, RB_RED);
                    rb_rotate_right(root, sibling);
                    sibling = parent->right;
                }
                rb_set_color(sibling, rb_get_color(parent));
                rb_set_color(parent, RB_BLACK);
                rb_set_color(sibling->right, RB_BLACK);
                rb_rotate_left(root, parent);
                node = root->rb_node;
                break;
            }
        } else {
            // 对称情况
            rb_node *sibling = parent->left;
            if (rb_get_color(sibling) == RB_RED) {
                rb_set_color(sibling, RB_BLACK);
                rb_set_color(parent, RB_RED);
                rb_rotate_right(root, parent);
                sibling = parent->left;
            }
            if ((!sibling->right || rb_get_color(sibling->right) == RB_BLACK) &&
                (!sibling->left || rb_get_color(sibling->left) == RB_BLACK)) {
                rb_set_color(sibling, RB_RED);
                node = parent;
                parent = rb_get_parent(parent);
            } else {
                if (!sibling->left || rb_get_color(sibling->left) == RB_BLACK) {
                    rb_set_color(sibling->right, RB_BLACK);
                    rb_set_color(sibling, RB_RED);
                    rb_rotate_left(root, sibling);
                    sibling = parent->left;
                }
                rb_set_color(sibling, rb_get_color(parent));
                rb_set_color(parent, RB_BLACK);
                rb_set_color(sibling->left, RB_BLACK);
                rb_rotate_right(root, parent);
                node = root->rb_node;
                break;
            }
        }
    }
    if (node)
        rb_set_color(node, RB_BLACK);
}
rb_node *rb_first(rb_root *root)
{
    rb_node *n = root->rb_node;
    if (!n)
        return NULL;
    while (n->left)
        n = n->left;
    return n;
}
rb_node *rb_next(rb_node *node)
{
    if (node->right) {
        node = node->right;
        while (node->left)
            node = node->left;
        return node;
    }

    rb_node *parent = rb_get_parent(node);
    while (parent && node == parent->right) {
        node = parent;
        parent = rb_get_parent(parent);
    }
    return parent;
}
rb_node *rb_last(rb_root *root)
{
    rb_node *n = root->rb_node;
    if (!n)
        return NULL;
    while (n->right)
        n = n->right;
    return n;
}
rb_node *rb_prev(rb_node *node)
{
    if (node->left) {
        node = node->left;
        while (node->right)
            node = node->right;
        return node;
    }

    rb_node *parent = rb_get_parent(node);
    while (parent && node == parent->left) {
        node = parent;
        parent = rb_get_parent(parent);
    }
    return parent;
}

rb_node *rb_find(rb_root *root, unsigned long key)
{
    spin_lock_acquire(&root->lock);
    rb_node *node = root->rb_node;
    
    while (node) {
        if (key == (unsigned long)node) {
            spin_lock_release(&root->lock);
            return node;
        }
        node = (key < (unsigned long)node) ? node->left : node->right;
    }
    
    spin_lock_release(&root->lock);
    return NULL;
}

rb_node *rb_find_ge(rb_root *root, unsigned long key)
{
    spin_lock_acquire(&root->lock);
    rb_node *node = root->rb_node;
    rb_node *candidate = NULL;
    
    while (node) {
        if (key == (unsigned long)node) {
            spin_lock_release(&root->lock);
            return node;
        }
        if (key < (unsigned long)node) {
            candidate = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    
    spin_lock_release(&root->lock);
    return candidate;
}

rb_node *rb_find_le(rb_root *root, unsigned long key)
{
    spin_lock_acquire(&root->lock);
    rb_node *node = root->rb_node;
    rb_node *candidate = NULL;
    
    while (node) {
        if (key == (unsigned long)node) {
            spin_lock_release(&root->lock);
            return node;
        }
        if (key > (unsigned long)node) {
            candidate = node;
            node = node->right;
        } else {
            node = node->left;
        }
    }
    
    spin_lock_release(&root->lock);
    return candidate;
}

/* 旋转操作 */
void rb_rotate_left(rb_root *root, rb_node *node)
{
    rb_node *right = node->right;
    if ((node->right = right->left))
        rb_set_parent(right->left, node);
    right->left = node;

    rb_set_parent(right, rb_get_parent(node));
    if (rb_get_parent(node)) {
        if (node == rb_get_parent(node)->left)
            rb_get_parent(node)->left = right;
        else
            rb_get_parent(node)->right = right;
    } else {
        root->rb_node = right;
    }
    rb_set_parent(node, right);
}
void rb_rotate_right(rb_root *root, rb_node *node)
{
    rb_node *left = node->left;
    if ((node->left = left->right))
        rb_set_parent(left->right, node);
    left->right = node;

    rb_set_parent(left, rb_get_parent(node));
    if (rb_get_parent(node)) {
        if (node == rb_get_parent(node)->right)
            rb_get_parent(node)->right = left;
        else
            rb_get_parent(node)->left = left;
    } else {
        root->rb_node = left;
    }
    rb_set_parent(node, left);
}

/* 颜色操作 */
void rb_set_color(rb_node *node, int color)
{
    node->__parent_color = (rb_node *)((unsigned long)rb_get_parent(node) | color);
}
int rb_get_color(rb_node *node)
{
    return (int)((unsigned long)node->__parent_color & 1);
}
void rb_set_parent(rb_node *node, rb_node *parent)
{
    node->__parent_color = (rb_node *)((unsigned long)parent | rb_get_color(node));
}
rb_node *rb_get_parent(rb_node *node)
{
    return (rb_node *)((unsigned long)node->__parent_color & ~1);
}

/* 替换操作 */
void rb_replace_node(rb_root *root, rb_node *victim, rb_node *new)
{
    rb_node *parent = rb_get_parent(victim);

    if (parent) {
        if (victim == parent->left)
            parent->left = new;
        else
            parent->right = new;
    } else {
        root->rb_node = new;
    }

    if (victim->left)
        rb_set_parent(victim->left, new);
    if (victim->right)
        rb_set_parent(victim->right, new);

    *new = *victim;
}

/* 验证函数 */
void rb_verify(rb_root *root)
{
    if (!root->rb_node)
        return;

    // 验证根节点是黑色
    if (rb_get_color(root->rb_node) != RB_BLACK)
        panic("RB verify: root not black");

    // 递归验证红黑树性质
    rb_verify_node(root->rb_node);
}

static int rb_verify_node(rb_node *node)
{
    int left_black, right_black;

    if (!node)
        return 1;

    // 验证红色节点的子节点必须是黑色
    if (rb_get_color(node) == RB_RED) {
        if ((node->left && rb_get_color(node->left) != RB_BLACK) ||
            (node->right && rb_get_color(node->right) != RB_BLACK))
            panic("RB verify: red node with red child");
    }

    left_black = rb_verify_node(node->left);
    right_black = rb_verify_node(node->right);

    // 验证每条路径黑色节点数相同
    if (left_black != right_black)
        panic("RB verify: unequal black height");

    return left_black + (rb_get_color(node) == RB_BLACK ? 1 : 0);
}