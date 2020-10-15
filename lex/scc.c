#include "scc.h"

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
