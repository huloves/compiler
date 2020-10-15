#ifndef __LEX_SCC_H
#define __LEX_SCC_H

#include <stdio.h>

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

/* 动态字符串 */
typedef struct DynString
{
    int count;
    int capacity;   //包含该字符串的缓冲的缓冲区长度
    char* data;   //指向字符串的指针
}DynString;

/* 动态数组 */
typedef struct DynArray
{
    int count;
    int capacity;
    void** data;   //指向数据指针数组
}DynArray;

#endif
