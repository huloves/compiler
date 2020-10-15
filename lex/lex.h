#ifndef __LEX_LEX_H
#define __LEX_LEX_H

#include "scc.h"

/**
 * tkwork_direct_insert - 运算符、关键字、常亮直接放入单词表
 * **/
TkWord* tkword_direct_insert(TkWord* tp);

/**
 * tkwork_find - 在单词表中查找单词
 * @p - 要查找的单词
 * @keyno - 要查找单词的哈希值
 * **/
TkWord* tkword_find(char* p, int keyno);

/**
 * tkword_insert - 标识符插入单词表，先查找，查不到再插入单词表
 * **/
TkWord* tkword_insert(char* p);

#endif
