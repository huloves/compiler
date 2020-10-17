/***********************************************************
 * hello.c源文件
 **********************************************************/
int main()
{
int a = 1 + 3;
int b = 12 * 21;
int c;
if(a == b) {
    c = 66;
}
printf("Hello World!\n");
return 0;
}

void entry()
{
int ret;
ret = main();
exit(ret);
}