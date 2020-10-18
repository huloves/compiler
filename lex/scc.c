#include "scc.h"
#include "lex.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE* fin = NULL;
FILE* fout = NULL;
FILE* consttable_file = NULL;
char* filename;
char* outfile;
int line_num;

/**
 * elf_hash - 计算哈希地址
 * @key - 哈希关键字
 * @MAXKEY - 哈希表长度
 * **/
int elf_hash(char* key)
{
    int h = 0, g;
    while(*key) {
        h = (h << 4) + *key++;
        g = h & 0xf0000000;
        if(g) {
            h ^= g >> 24;
        }
        h &= ~g;
    }
    return h % MAXKEY;
}

/**
 * mallocz - 分配堆内存并将数据初始化为'0'
 * @size - 分配内存大小
 **/
void *mallocz(int size)
{
    void *ptr;
	ptr = malloc(size);
	if (!ptr && size)
        perror("内存分配失败");
    memset(ptr, 0, size); 
    return ptr;
}

/**
 * init - 初始化
 * **/
void init()
{
    line_num = 1;
    init_lex();
    // print_tktable();
    printf("lex init done!\n");
}

/**
 * clean_up - 释放单词表空间
 * **/
static void clean_up()
{
    for(int i = TK_IDENT; i< tktable.count; i++) {
        free(tktable.data[i]);
    }
    free(tktable.data);
}

/**
 * main - 主函数
 * **/
int main(int argc, char** argv)
{
    fin = fopen(argv[1], "rb");
    if(!fin) {
        printf("can't open source file: %s\n", argv[1]);
        return 0;
    } else {
        printf("file open success!\n");
    }
    filename = argv[1];
    fout = fopen("outfile", "wt");
    consttable_file = fopen("constant_table", "wt");
    init();   //初始化单词表和行数
    getch();
    lex();
    clean_up();
    fclose(fin);
    fclose(fout);
    fclose(consttable_file);
    printf("finish!\n");
    return 1;
}
