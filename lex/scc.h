#ifndef __LEX_SCC_H
#define __LEX_SCC_H

#include <stdio.h>
#include "dynstring.h"

#define STR_INIT_SIZE 8
#define MAXKEY 1024

//单词编码
enum e_TokenCode
{
    //运算符及分隔符
    TK_PLUS,   //+
    TK_MINUS,   //-
    TK_STAR,   //*
    TK_DIVIDE,   // /
    TK_MOD,   //%
    TK_EQ,   //==
    TK_NEQ,   //!=
    TK_LT,   //<
    TK_LEQ,   //<=
    TK_GT,   //>
    TK_GEQ,   //>=
    TK_ASSIGN,   //=
    TK_POINTSTO,   //->
    TK_DOT,   //.
    TK_AND,   //& 地址 与运算符
    TK_OPENPA,   //(
    TK_CLOSEPA,   //)
    TK_OPENBR,   //[
    TK_CLOSEBR,   //]
    TK_BEGIN,   //{
    TK_END,   //}
    TK_SEMICOLON,   //;
    TK_COMMA,   //,
    TK_ELLIPSIS,   //...
    TK_EOF,   //文件结束符

    //常量
    TK_CINT,   //整型常量
    TK_CCHAR,   //字符常量
    TK_CSTR,   //字符串常量

    //关键字
    KW_CHAR,   //char关键字
    KW_SHORT,   //short关键字
    KW_INT,   //int
    KW_VOID,   //void
    KW_STRUCT,   //struct
    KW_IF,   //if
    KW_ELSE,   //else
    KW_FOR,   //for
    KW_CONTINUE,   //continue
    KW_BREAK,   //break
    KW_RETURN,   //return
    KW_SIZEOF,   //sizeof

    KW_ALIGN,   //__align
    KW_CDECL,   //__cdecl
    KW_STDCALL,   //__stdcall

    //标识符
    TK_IDENT
};

typedef struct TkWord
{
    int tkcode;   //单词编码
    struct TkWord* next;   //指向哈希冲突的同义词
    char* spelling;   //单词字符串
    struct Symbol* sym_struct;   //指向单词表所表示的结构定义
    struct Symbol* sym_identifier;   //指向单词所表示的标识符
}TkWord;

extern DynString tkstr;
extern FILE *fin;
extern char *filename;
extern int token;
extern int tkvalue;
extern int line_num;

/**
 * elf_hash - 计算哈希地址
 * @key - 哈希关键字
 * @MAXKEY - 哈希表长度
 * **/
int elf_hash(char* key);

/**
 * mallocz - 分配堆内存并将数据初始化为'0'
 * @size - 分配内存大小
 **/
void *mallocz(int size);

/**
 * init_lex - 词法分析初始化
 * **/
void init_lex();

#endif
