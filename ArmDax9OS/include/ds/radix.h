//radix tree定义
#pragma once
#include <common/types.h>
#include <common/macro.h>
#include <common/lock.h>

struct radix_node
{
    /* data */
    union  {
        /* data */
    };
    
};
struct radix_tree
{
    /* data */
    struct radix_node *root;
    struct spin_lock radix_lock;
};

