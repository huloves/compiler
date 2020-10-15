#include "scc.h"
#include <stdlib.h>

#define STR_INIT_SIZE 8

/**
 * dynstring_init - 初始化动态字符串存储容量
 * @pstr - 动态字符串存储结构
 * @initsize - 字符串初始化分配空间
 * **/
void dynstring_init(DynString* pstr, int initsize)
{
    if(pstr != NULL) {
        pstr->data = (char*)malloc(sizeof(char)*initsize);
        pstr->count = 0;
        pstr->capacity = initsize;
    }
}

/**
 * dynstring_free - 释放动态字符串使用的内存空间
 * @pstr - 动态字符串存储结构
 * **/
void dynstring_free(DynString* pstr)
{
    if(pstr != NULL) {
        if(pstr->data) {
            free(pstr->data);
        }
        pstr->count = 0;
        pstr->capacity = 0;
    }
}

/**
 * dynstring_reset - 重置动态字符串，先释放，重新初始化
 * @pstr - dynstring struct
 * **/
void dynstring_reset(DynString* pstr)
{
    dynstring_free(pstr);
    dynstring_init(pstr, STR_INIT_SIZE);   //字符串初始化空间STR_INIT_SIZE(8B)
}

/**
 * dynstring_realloc - 重新分配字符串容量
 * @pstr - 动态字符串存储结构
 * @new_size - 字符串新长度
 * **/
void dynstring_realloc(DynString* pstr, int new_size)
{
    int capacity;
    char* data;

    capacity = pstr->capacity;
    while(capacity < new_size) {
        capacity = capacity * 2;
    }
    data = realloc(pstr->data, capacity);
    if(!data) {
        perror("relloc error");
    }
    pstr->capacity = capacity;
    pstr->data = data;
}

/**
 * dynstring_chcat - 追加单个字符到动态字符串对象
 * @pstr - dynstring struct
 * @ch -要追加的字符
 * **/
void dynstring_chcat(DynString* pstr, int ch)
{
    int count;
    count = pstr->count + 1;
    if(count > pstr->capacity) {
        dynstring_realloc(pstr, count);
    }
    ((char*)pstr->data)[count - 1] = ch;
    pstr->count = count;
}
