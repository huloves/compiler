#ifndef __LEX_LEX_H
#define __LEX_LEX_H

#include "scc.h"
#include "dynarray.h"

// extern DynArray tktable;			// 单词动态数组
extern TkWord* tk_hashtable[MAXKEY];// 单词哈希表
extern DynArray tktable;			// 单词动态数组
extern char ch;

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

/**
 * constant_table_insert - 将单词加入常量表
 * @p: 要加入的单词
 * **/
TkWord* constant_table_insert(char* p);

/**
 * get_tkstr - 取得单词v所代表的源码字符串，错误提示用
 * @v: 单词编号
 **/
char *get_tkstr(int v);

/**
 * getch - 从文件中读取一个字节
 * **/
void getch();

/**
 * print_tktable - 打印单词表
 * **/
void print_tktable();

/**
 * lex - 词法分析
 * **/
void lex();

#endif
