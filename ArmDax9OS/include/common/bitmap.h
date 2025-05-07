#pragma once
#include <common/types.h>
#include <common/macro.h>

/**
 * @brief 位图操作函数集合
 * 提供对位图的各种操作，包括设置、清除、测试位等
 */

// 位操作基础类型
typedef u32 bitmap_word_t;

// 每个字的位数
#define BIT_PER_WORD (sizeof(bitmap_word_t) * 8)

/**
 * @brief 位图结构
 * @param bits 位数据数组
 * @param size 位图大小(位数)
 */
typedef struct {
    bitmap_word_t *bits;
    size_t size;
} bitmap_t;

/* 基本位操作 */

/**
 * @brief 设置指定位
 * @param word 位图字
 * @param bit 位偏移(0-31)
 */
static inline void bitmap_set_bit(bitmap_word_t *word, u32 bit)
{
    *word |= (1UL << bit);
}

/**
 * @brief 清除指定位
 * @param word 位图字
 * @param bit 位偏移(0-31)
 */
static inline void bitmap_clear_bit(bitmap_word_t *word, u32 bit)
{
    *word &= ~(1UL << bit);
}

/**
 * @brief 测试指定位
 * @param word 位图字
 * @param bit 位偏移(0-31)
 * @return 1-置位 0-未置位
 */
static inline int bitmap_test_bit(bitmap_word_t *word, u32 bit)
{
    return (*word >> bit) & 1UL;
}

/**
 * @brief 查找第一个置位
 * @param word 位图字
 * @return 置位的位索引(0-31)，若无则返回32
 */
static inline u32 bitmap_find_first_set(bitmap_word_t word)
{
    return __builtin_ffs(word) - 1;
}

/**
 * @brief 查找第一个清零位
 * @param word 位图字
 * @return 清零的位索引(0-31)，若无则返回32
 */
static inline u32 bitmap_find_first_zero(bitmap_word_t word)
{
    return bitmap_find_first_set(~word);
}

/**
 * @brief 计算置位数量
 * @param word 位图字
 * @return 置位的数量
 */
static inline u32 bitmap_popcount(bitmap_word_t word)
{
    return __builtin_popcount(word);
}

/* 位图操作 */

/**
 * @brief 初始化位图
 * @param bitmap 位图指针
 * @param bits 存储数组
 * @param size 位图大小(位数)
 */
static inline void bitmap_init(bitmap_t *bitmap, bitmap_word_t *bits, size_t size)
{
    bitmap->bits = bits;
    bitmap->size = size;
}

/**
 * @brief 设置位图中的位
 * @param bitmap 位图指针
 * @param bit 位偏移
 */
static inline void bitmap_set(bitmap_t *bitmap, u32 bit)
{
    bitmap_set_bit(&bitmap->bits[bit / BIT_PER_WORD], bit % BIT_PER_WORD);
}

/**
 * @brief 清除位图中的位
 * @param bitmap 位图指针
 * @param bit 位偏移
 */
static inline void bitmap_clear(bitmap_t *bitmap, u32 bit)
{
    bitmap_clear_bit(&bitmap->bits[bit / BIT_PER_WORD], bit % BIT_PER_WORD);
}

/**
 * @brief 测试位图中的位
 * @param bitmap 位图指针
 * @param bit 位偏移
 * @return 1-置位 0-未置位
 */
static inline int bitmap_test(bitmap_t *bitmap, u32 bit)
{
    return bitmap_test_bit(&bitmap->bits[bit / BIT_PER_WORD], bit % BIT_PER_WORD);
}
/**
 * @brief 查找第一个清零位
 * @param bitmap 位图指针
 * @return 清零的位索引，若无则返回bitmap->size
 */
static inline u32 bitmap_find_first_zero_bit(bitmap_t *bitmap)
{
    for (u32 i = 0; i < bitmap->size / BIT_PER_WORD; i++) {
        if (~bitmap->bits[i] != 0) {
            u32 bit = bitmap_find_first_zero(bitmap->bits[i]);
            u32 pos = i * BIT_PER_WORD + bit;
            return pos < bitmap->size ? pos : bitmap->size;
        }
    }
    return bitmap->size;
}

/**
 * @brief 查找第一个置位
 * @param bitmap 位图指针
 * @return 置位的位索引，若无则返回bitmap->size
 */
static inline u32 bitmap_find_first_set_bit(bitmap_t *bitmap)
{
    for (u32 i = 0; i < bitmap->size / BIT_PER_WORD; i++) {
        if (bitmap->bits[i] != 0) {
            u32 bit = bitmap_find_first_set(bitmap->bits[i]);
            u32 pos = i * BIT_PER_WORD + bit;
            return pos < bitmap->size ? pos : bitmap->size;
        }
    }
    return bitmap->size;
}


