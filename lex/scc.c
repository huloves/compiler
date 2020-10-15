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
