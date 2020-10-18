#include "dynarray.h"
#include <stdlib.h>

/**
 * dynarray_realloc - 重新分配数组容量
 * @parr - dynarray struct
 * @new_size - 动态数组最新元素个数
 * **/
void dynarray_realloc(DynArray* parr, int new_size)
{
    int capacity;
    void* data;

    capacity = parr->capacity;
    while(capacity < new_size) {
        capacity = capacity * 2;
    }
    data = realloc(parr->data, capacity);
    if(!data) {
        perror("relloc failed\n");
    }
    parr->capacity = capacity;
    parr->data = data;
}

/**
 * dynarray_add - 追加动态数组元素
 * @parr - dynarray struct
 * @data - 要追加的新元素
 * **/
void dynarray_add(DynArray* parr, void* data)
{
    int count;
    count = parr->count + 1;
    if(count * sizeof(void*) > parr->capacity) {
        dynarray_realloc(parr, count * sizeof(void*));
    }
    parr->data[count - 1] = data;
    // printf("%s\n", ((TkWord*)data)->spelling);
    parr->count = count;
}

/**
 * dynarray_init - 初始化动态数组存储容量
 * @parr - dynarray strucr
 * @initsize - 动态数组初始化分配空间
 * **/
void dynarray_init(DynArray* parr, int initsize)
{
    if(parr != NULL) {
        parr->data = (void**)malloc(sizeof(char*) * initsize);
        parr->count = 0;
        parr->capacity = initsize;
    }
}

/**
 * dynarray_free - 释放动态数组使用的内存空间
 * @parr - dynarray struct
 * **/
void dynarray_free(DynArray* parr)
{
    void** p;;
    for(p=parr->data; parr->count; ++p, --parr->count) {
        if(*p) {
            free(*p);
        }
    }
    free(parr->data);
    parr->data = NULL;
}

/**
 * dynarray_search - 动态数组元素查找
 * @parr - dynarray struct
 * @key - 要查找的元素
 * **/
int dynarray_search(DynArray* parr, int key)
{
    int i;
    int** p;
    p = (int**)parr->data;
    for(i=0; i<parr->count; ++i, ++p) {
        if(key == **p) {
            return i;
        }
    }

    return -1;
}
