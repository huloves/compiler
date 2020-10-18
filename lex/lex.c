#include "lex.h"
#include "scc.h"
#include "error.h"
#include "dynarray.h"
#include "dynstring.h"
#include <string.h>
#include <stdlib.h>

TkWord*     tk_hashtable[MAXKEY];   //哈希表容量
int         token;
DynString   sourcestr;
char        ch;
DynArray    tktable;		// 单词表
DynArray    constant_table;   //常量表
TkWord*     constant_hashtable[MAXKEY];   //常量哈希表
DynArray    reserved_table;   //保留字表
TkWord*     reserved_hashtable[MAXKEY];   //保留字哈希表
DynArray    char_table;
DynString   tkstr; 
int         tkvalue;
DynString   tkstr;
int         tkvalue;

/**
 * getch - 从文件中读取一个字节
 * **/
void getch()
{
    ch = getc(fin);
    // printf("%c", ch);
}

/**
 * is_nodigit - 判断c是否为字母或下划线
 * @c: 要判断的字符
 * **/
static int is_nodigit(char c)
{
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
}

/**
 * is_digit - 判断c是否为数字
 * @c: 要判断的字符
 * **/
static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

/**
 * parse_identifier - 解析标识符
 * **/
void parse_identifier()
{
    dynstring_reset(&tkstr);
    dynstring_chcat(&tkstr, ch);
    getch();
    while(is_nodigit(ch) || is_digit(ch)) {
        dynstring_chcat(&tkstr, ch);
        getch();
    }
    dynstring_chcat(&tkstr, '\0');
    // return tkword_insert(tkstr.data);
}

/**
 * parse_num - 解析整型常量
 * **/
void parse_num()
{
    dynstring_reset(&tkstr);
    dynstring_reset(&sourcestr);
    do {
        dynstring_chcat(&tkstr, ch);
        dynstring_chcat(&sourcestr, ch);
        getch();
    } while(is_digit(ch));
    if(ch == '.') {
        do {
            dynstring_chcat(&tkstr, ch);
            dynstring_chcat(&sourcestr, ch);
            getch();
        } while(is_digit(ch));
    }
    dynstring_chcat(&tkstr, '\0');
    dynstring_chcat(&sourcestr, '\0');
    // tkword_insert(tkstr.data);   //将整型常量加入单词表
    constant_table_insert(tkstr.data);
    tkvalue = atoi(tkstr.data);
}

/**
 * parse_string - 解析字符常量、字符串常量
 * @sep: 字符常量界标识为单引号(')，字符串常量标识为双引号(")
 * **/
void parse_string(char sep)
{
    char c;
    dynstring_reset(&tkstr);
    dynstring_reset(&sourcestr);
    dynstring_chcat(&sourcestr, sep);
    getch();
    for(;;) {
        if(ch == sep) {
            break;
        } else if(ch == '\\') {
            dynstring_chcat(&sourcestr, ch);
            getch();
            switch(ch) {
                case '0':
                {
                    c = '\0';
                    break;
                }
                case 'a':
                {
                    c = '\a';
                    break;
                }
                case 'b':
                {
                    c = '\b';
                    break;
                }
                case 'n':
                {
                    c = '\n';
                    break;
                }
                case 'v':
                {
                    c = '\v';
                    break;
                }
                case 'f':
                {
                    c = '\f';
                    break;
                }
                case 'r':
                {
                    c = '\r';
                    break;
                }
                case '\"':
                {
                    c = '\"';
                    break;
                }
                case '\'':
                {
                    c = '\'';
                    break;
                }
                case '\\':
                {
                    c = '\\';
                    break;
                }
                default:
                {
                    c = ch;
                    if(c >= '!' && c <= '~') {
                        warning("非法转义字符: \'\\%c\'", c);
                    } else {
                        warning("非法转义字符: \'\\0x%x\'", c);
                    }
                    break;
                }
                dynstring_chcat(&tkstr, c);
                dynstring_chcat(&sourcestr, ch);
                getch();
            }
        } else {
            dynstring_chcat(&tkstr, ch);
            dynstring_chcat(&sourcestr, ch);
            getch();
        }
    }
    dynstring_chcat(&tkstr, '\0');
    dynstring_chcat(&sourcestr, sep);
    dynstring_chcat(&sourcestr, '\0');
    // tkword_insert(tkstr.data);   //将字符串常量加入单词表
    constant_table_insert(tkstr.data);
    getch();
}

/**
 * tkwork_direct_insert - 运算符、关键字、常量直接放入单词表
 * @tp: 单词指针
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
 * @p: 要查找的单词
 * @keyno: 要查找单词的哈希值
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
 * hashtable_find - 在hashtable中查找单词
 * @p: 要查找的单词
 * @keyno: 要查找单词的哈希值
 * @hashtable: 要查找的哈希表
 * **/
TkWord* hashtable_find(char* p, int keyno, TkWord* hashtable[])
// TkWord* hashtable_find(char* p, int keyno)
{
    TkWord* tp = NULL, * tp1;
    for(tp1 = hashtable[keyno]; tp1; tp1 = tp1->next) {
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
        // dynarray_add(&tktable, tp);
        tp->tkcode = tktable.count - 1;
        s = (char*)tp + sizeof(TkWord);
        tp->spelling = (char*)s;
        for(end = p + length; p < end;) {
            *s++ = *p++;
        }
        *s = (char)'\0';

        tp->sym_identifier = NULL;
        tp->sym_struct = NULL;
        dynarray_add(&tktable, tp);
    }
    return tp;
}

/**
 * constant_table_insert - 将单词加入常量表
 * @p: 要加入的单词
 * **/
TkWord* constant_table_insert(char* p) {
    // printf("%s\n", p);
    TkWord* tp;
    int keyno;
    char* s;
    char* end;
    int length;

    keyno = elf_hash(p);
    tp = hashtable_find(p, keyno, constant_hashtable);
    // tp = hashtable_find(p, keyno);
    if(tp == NULL) {
        length = strlen(p);
        tp = (TkWord*)mallocz(sizeof(TkWord) + length + 1);
        tp->next = constant_hashtable[keyno];
        constant_hashtable[keyno] = tp;
        // dynarray_add(&constant_table, tp);
        tp->tkcode = constant_table.count - 1;
        s = (char*)tp + sizeof(TkWord);
        tp->spelling = (char*)s;
        for(end = p + length; p < end;) {
            *s++ = *p++;
        }
        *s = (char)'\0';

        tp->sym_identifier = NULL;
        tp->sym_struct = NULL;
        dynarray_add(&constant_table, tp);
    }
    // print_consttable();
    return tp;
}

/**
 * reserved_table_insert - 将单词加入保留字表
 * @p: 要加入的单词
 * **/
TkWord* reserved_table_insert(char* p) {
    TkWord* tp;
    int keyno;
    char* s;
    char* end;
    int length;

    keyno = elf_hash(p);
    // tp = hashtable_find(p, keyno, reserved_hashtable);
    if(tp == NULL) {
        length = strlen(p);
        tp = (TkWord*)mallocz(sizeof(TkWord) + length + 1);
        tp->next = reserved_hashtable[keyno];
        reserved_hashtable[keyno] = tp;
        dynarray_add(&reserved_table, tp);
        tp->tkcode = reserved_table.count - 1;
        s = (char*)tp + sizeof(TkWord);
        tp->spelling = (char*)s;
        for(end = p + length; p < end;) {
            *s++ = *p++;
        }
        *s = (char)'\0';

        tp->sym_identifier = NULL;
        tp->sym_struct = NULL;
    }
    return tp;
}

/**
 * init_lex - 词法分析初始化
 * **/
void init_lex()
{
    TkWord* tp;
    static TkWord keywords[] = {
        {TK_PLUS,       NULL,       "+",    NULL,   NULL},
        {TK_MINUS,      NULL,       "-",    NULL,   NULL},
        {TK_STAR,       NULL,       "*",    NULL,   NULL},
        {TK_DIVIDE,     NULL,       "/",    NULL,   NULL},
        {TK_MOD,        NULL,       "%",    NULL,   NULL},
        {TK_EQ,         NULL,       "==",   NULL,   NULL},
        {TK_NEQ,        NULL,       "!=",   NULL,   NULL},
        {TK_LT,         NULL,       "<",    NULL,   NULL},
    	{TK_LEQ,		NULL,	    "<=",	NULL,	NULL},
    	{TK_GT,			NULL,	    ">",	NULL,	NULL},
    	{TK_GEQ,		NULL,	    ">=",	NULL,	NULL},
    	{TK_ASSIGN,		NULL,   	"=",	NULL,	NULL},
    	{TK_POINTSTO,	NULL,	    "->",	NULL,	NULL},
    	{TK_DOT,		NULL,	    ".",	NULL,	NULL},
    	{TK_AND,		NULL,	    "&",	NULL,	NULL},
    	{TK_OPENPA,		NULL,	    "(",	NULL,	NULL},
    	{TK_CLOSEPA,	NULL,	    ")",	NULL,	NULL},
    	{TK_OPENBR,		NULL,	    "[",	NULL,	NULL},
    	{TK_CLOSEBR,	NULL,	    "]",	NULL,	NULL},	
    	{TK_BEGIN,		NULL,	    "{",	NULL,	NULL},
    	{TK_END,		NULL,	    "}",	NULL,	NULL},
    	{TK_SEMICOLON,	NULL,	    ";",	NULL,	NULL},
    	{TK_COMMA,		NULL,	    ",",	NULL,	NULL},
    	{TK_ELLIPSIS,	NULL,	    "...",	NULL,	NULL},
    	{TK_EOF,		NULL,	    "End_Of_File",	NULL,	NULL},

    	{TK_CINT,		NULL,	 	"整型常量",     NULL,	NULL},        
    	{TK_CCHAR,		NULL,		"字符常量",	    NULL,	NULL},        
    	{TK_CSTR,		NULL,		"字符串常量",	NULL,	NULL},	      

    	{KW_CHAR,		NULL,		"char",	        NULL,	NULL},
    	{KW_SHORT,		NULL,		"short",        NULL,	NULL},
    	{KW_INT,		NULL,		"int",	        NULL,	NULL},
    	{KW_VOID,		NULL,		"void",	        NULL,	NULL},	
    	{KW_STRUCT,		NULL,		"struct",	    NULL,	NULL},

    	{KW_IF,			NULL,		"if"	,	    NULL,	NULL},  
    	{KW_ELSE,		NULL,		"else",	        NULL,	NULL},  
    	{KW_FOR,		NULL,		"for",	        NULL,	NULL},  
    	{KW_CONTINUE,	NULL,		"continue",	    NULL,	NULL},
    	{KW_BREAK,		NULL,		"break",	    NULL,	NULL},    
    	{KW_RETURN,		NULL,		"return",	    NULL,	NULL},
    	{KW_SIZEOF,		NULL,		"sizeof",	    NULL,	NULL},
    	{KW_ALIGN,		NULL,		"__align",	    NULL,	NULL},
    	{KW_CDECL,		NULL,		"__cdecl",	    NULL,	NULL},
    	{KW_STDCALL,	NULL,		"__stdcall",	NULL,	NULL},
        {0,             NULL,       NULL,           NULL,   NULL}
    };

    dynarray_init(&tktable, 8);
    dynarray_init(&constant_table, 8);
    dynarray_init(&reserved_table, 8);
    for(tp = &keywords[0]; tp->spelling != NULL; tp++) {
        tkword_direct_insert(tp);
    }
}

/**
 * print_tktable - 打印单词表tktable
 * **/
void print_tktable()
{
    for(int i=0; i<tktable.count; i++) {
        printf("%s\n", ((TkWord*)tktable.data[i])->spelling);
    }
}

void print_consttable()
{
    for(int i=0; i<constant_table.count; i++) {
        printf("%s\n", ((TkWord*)constant_table.data[i])->spelling);
    }
}

void tktable_print2file()
{
    char buf[1024];
    for(int i=0; i<tktable.count; i++) {
        sprintf(buf, "%s\n", ((TkWord*)tktable.data[i])->spelling);
        // printf("%s\n", ((TkWord*)tktable.data[i])->spelling);
        fwrite(buf, strlen(buf), 1, fout);
    }
}

void consttable_print2file()
{
    char buf[1024];
    for(int i=0; i<constant_table.count; i++) {
        sprintf(buf, "%s\n", ((TkWord*)constant_table.data[i])->spelling);
        // printf("%s\n", buf);
        fwrite(buf, strlen(buf), 1, consttable_file);
    }
}

/**
 * skip_white_space - 忽略空格
 * **/
void skip_white_space()
{
    while(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
        if(ch == '\r') {
            getch();
            printf("----%c----", ch);
            if(ch != '\n') {
                return;
            }
            line_num++;
        } else if(ch == '\n') {
            line_num++;
            // getch();
        }
        // printf("%c", ch);
        getch();
    }
}

/**
 * parse_comment - 解析注释
 * **/
void parse_comment()
{
    getch();
    do {
        do {
            if(ch == '\n' || ch == '*' || ch == TK_EOF) {
            // if(ch == '\n' || ch == '*' || ch == EOF) {
                break;
            } else {
                getch();
            }
        } while(1);
        if(ch == '\n') {
            line_num++;
            getch();
        } else if(ch == '*') {
            getch();
            if(ch == '/') {
                getch();
                return;
            }
        } else {
            error("一直到文件尾未发现配对的注释结束符");
            return;
        }
    } while(1);
}

/**
 * preprocess - 预处理，忽略空白字符及注释
 * **/
void preprocess()
{
    while(1) {
        if(ch == '\r') {
            printf("1\n");
        }
        if(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
            skip_white_space();
        } 
        else if(ch == '/') {
            getch();
            if(ch == '*') {
                parse_comment();
            } else {
                ungetc(ch, fin);   //把一个字符退回到输入流中
                ch = '/';
                break;
            }
        } else {
            break;
        }
    }
}

/**
 * get_token - 取单词
 * **/
void get_token()
{
    preprocess();
    switch(ch) {
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case '_':
        {
            TkWord* tp;
            // tp = parse_identifier();
            parse_identifier();
            tp = tkword_insert(tkstr.data);
            token = tp->tkcode;
            break;
        }
        case '0': case '1': case '2': case '3': case '4': case '5': case '6':
        case '7': case '8': case '9':
        {
            parse_num();
            token = TK_CINT;
            break;
        }
        case '+':
        {
            getch();
            token = TK_PLUS;
            break;
        }
        case '-':
        {
            getch();
            if(ch == '>') {
                token = TK_POINTSTO;
                getch();
            } else {
                token = TK_MINUS;
            }
            break;
        }
        case '/':
        {
            getch();
            token = TK_DIVIDE;
            break;
        }
        case '%':
        {
            getch();
            token = TK_MOD;
            break;
        }
        case '=':
        {
            getch();
            if(ch == '=') {
                token = TK_EQ;
                getch();
            } else {
                token = TK_ASSIGN;
            }
            break;
        }
        case '!':
        {
            getch();
            if(ch == '=') {
                token = TK_NEQ;
                getch();
            } else {
                error("don't support '!'");
            }
            break;
        }
        case '<':
        {
            getch();
            if(ch == '=') {
                token = TK_LEQ;
                getch();
            } else {
                TK_LT;
            }
            break;
        }
        case '>':
        {
            getch();
            if(ch == '=') {
                token = TK_GEQ;
                getch();
            } else {
                token = TK_LT;
            }
            break;
        }
        case '.':
        {
            getch();
            if(ch == '.') {
                getch();
                if(ch != '.') {
                    error("省略号拼写错误");
                } else {
                    token = TK_ELLIPSIS;
                }
                getch();
            } else {
                token = TK_DOT;
            }
            break;
        }
        case '&':
        {
            getch();
            token = TK_AND;
            break;
        }
        case ';':
        {
            getch();
            token = TK_SEMICOLON;
            break;
        }
        case ']':
        {
            getch();
            token = TK_CLOSEBR;
            break;
        }
        case '}':
        {
            getch();
            token = TK_END;
            break;
        }
        case ')':
        {
            getch();
            token = TK_CLOSEPA;
            break;
        }
        case '[':
        {
            getch();
            token = TK_OPENBR;
            break;
        }
        case '{':
        {
            getch();
            token = TK_BEGIN;
            break;
        }
        case ',':
        {
            getch();
            token = TK_COMMA;
            break;
        }
        case '(':
        {
            getch();
            token = TK_OPENPA;
            break;
        }
        case '*':
        {
            getch();
            token = TK_STAR;
            break;
        }
        case '\'':
        {
            parse_string(ch);
            token = TK_CCHAR;
            tkvalue = *(char*)tkstr.data;
            break;
        }
        case '\"':
        {
            parse_string(ch);
            token = TK_CSTR;
            break;
        }
        case EOF:
        {
            token = TK_EOF;
            break;
        }
        default:
        {
            error("无法识别字符: \\x%02x --- ___%c___", ch, ch);
            getch();
            break;
        }
    }
}

/**
 * get_tkstr - 取得单词v所代表的源码字符串，错误提示用
 * @v: 单词编号
 **/
char *get_tkstr(int v)
{
	if(v > tktable.count)
		return NULL;
	else if(v >= TK_CINT && v <= TK_CSTR)
		return sourcestr.data;
	else
		return ((TkWord*)tktable.data[v])->spelling;
}

/**
 * lex - 词法分析
 * **/
void lex()
{
    do {
        get_token();   //取单词
        // print_tktable();
        // tktable_print2file();
        // fwrite(&ch, 1, 1, fout);
        getch();
    } while(token != TK_EOF);
    tktable_print2file();
    consttable_print2file();
    printf("code row = %d\n", line_num);
}
