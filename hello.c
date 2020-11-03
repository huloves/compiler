/**
 * hello.c源文件
 **/

struct point
{
    int x;
    int y;
};

struct point2
{
    int a;
    int b;
};

int main()
{
    int a = 1 + 3;
    int b = 12 * 21;
    int c;
    if(a == b) {
        c = 66;
    }
    printf("Hello World!");
    scanf("%d\n", a);
    return 0;
}

void _entry()
{
    int ret;
    ret = main();
    exit(ret);
}

