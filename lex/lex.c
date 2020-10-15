#include "lex.h"
#include "dynarray.h"
#include "string.h"

TkWord* tk_hashtable[MAXKEY];   //哈希表容量
DynArray tktable;   //单词表
int token;

/**
 * tkwork_direct_insert - 运算符、关键字、常亮直接放入单词表
 * **/
TkWord* tkword_direct_insert(TkWord* tp)
{
    int keyno;
    dynarray_add(&tktable, tp);
    keyno = elf_hash(tp->spelling);
    tp->next = tk_hashtable[keyno];
    tk_hashtable[keyno] = tp;
    return tp;
}

/**
 * tkwork_find - 在单词表中查找单词
 * @p - 要查找的单词
 * @keyno - 要查找单词的哈希值
 * **/
TkWord* tkword_find(char* p, int keyno)
{
    TkWord* tp = NULL, * tp1;
    for(tp1 = tk_hashtable[keyno]; tp1; tp1 = tp1->next) {
        if(!strcmp(p, tp1->spelling)) {
            token = tp1->tkcode;
            tp = tp1;
        }
    }
    return tp;
}

/**
 * tkword_insert - 标识符插入单词表，先查找，查不到再插入单词表
 * **/
TkWord* tkword_insert(char* p)
{
    TkWord* tp;
    int keyno;
    char* s;
    char* end;
    int length;

    keyno = elf_hash(p);
    tp = tkword_find(p, keyno);
    if(tp == NULL) {
        length = strlen(p);
        tp = (TkWord*)mallocz(sizeof(TkWord) + length + 1);
        tp->next = tk_hashtable[keyno];
        tk_hashtable[keyno] = tp;
        dynarray_add(&tktable, tp);
        tp->tkcode = tktable.count - 1;
        s = (char*)tp + sizeof(TkWord);
        tp->spelling = (char*)s;
        for(end = p + length; p < end;) {
            *s++ = *p++;
        }
        *s = (char*)'\0';
    }
    return tp;
}
