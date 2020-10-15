#ifndef __LEX_DYNSTRING_H
#define __LEX_DYNSTRING_H

#include "scc.h"

#define STR_INIT_SIZE 8   //初始化字符串大小

/* 动态字符串 */
typedef struct DynString
{
    int count;
    int capacity;   //包含该字符串的缓冲的缓冲区长度
    char* data;   //指向字符串的指针
}DynString;

/**
 * dynstring_init - 初始化动态字符串存储容量
 * @pstr - 动态字符串存储结构
 * @initsize - 字符串初始化分配空间
 * **/
void dynstring_init(DynString* pstr, int initsize);

/**
 * dynstring_free - 释放动态字符串使用的内存空间
 * @pstr - 动态字符串存储结构
 * **/
void dynstring_free(DynString* pstr);

/**
 * dynstring_reset - 重置动态字符串，先释放，重新初始化
 * @pstr - dynstring struct
 * **/
void dynstring_reset(DynString* pstr);

/**
 * dynstring_realloc - 重新分配字符串容量
 * @pstr - 动态字符串存储结构
 * @new_size - 字符串新长度
 * **/
void dynstring_realloc(DynString* pstr, int new_size);

/**
 * dynstring_chcat - 追加单个字符到动态字符串对象
 * @pstr - dynstring struct
 * @ch -要追加的字符
 * **/
void dynstring_chcat(DynString* pstr, int ch);


#endif
