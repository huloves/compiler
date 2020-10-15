#ifndef __LEX_DYNARRAY_H
#define __LEX_DYNARRAY_H

#include "scc.h"

/* 动态数组 */
typedef struct DynArray
{
    int count;
    int capacity;
    void** data;   //指向数据指针数组
}DynArray;

/**
 * dynarray_realloc - 重新分配数组容量
 * @parr - dynarray struct
 * @new_size - 动态数组最新元素个数
 * **/
void dynarray_realloc(DynArray* parr, int new_size);

/**
 * dynarray_add - 追加动态数组元素
 * @parr - dynarray struct
 * @data - 要追加的新元素
 * **/
void dynarray_add(DynArray* parr, void* data);

/**
 * dynarray_init - 初始化动态数组存储容量
 * @parr - dynarray strucr
 * @initsize - 动态数组初始化分配空间
 * **/
void dynarray_init(DynArray* parr, int initsize);

/**
 * dynarray_free - 释放动态数组使用的内存空间
 * @parr - dynarray struct
 * **/
void dynarray_free(DynArray* parr);

/**
 * dynarray_search - 动态数组元素查找
 * @parr - dynarray struct
 * @key - 要查找的元素
 * **/
int dynarray_search(DynArray* parr, int key);


#endif
