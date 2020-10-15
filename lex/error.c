#include "error.h"
#include "scc.h"
#include "lex.h"
#include <stdio.h>

/**
 * handle_exception - 异常处理
 * @stage - 编译阶段还是链接阶段
 * @level - 错误级别
 * @fmt - 参数输出的格式
 * @ap - 可变参数列表
 * **/
void handle_exception(int stage, int level, char* fmt, va_list ap)
{
    char buf[1024];
    vsprintf(buf, fmt, ap);
    if(stage == STAGE_COMPILE) {
        if(level == LEVEL_WARNING) {
            printf("%s(第%d行)：编译警告：%s!\n", filename, line_num, buf);
        } else {
            printf("%s(第%d行)：编译错误：%s!\n", filename, line_num, buf);
            exit(-1);
        }
    } else {
        printf("链接错误：%s!\n", buf);
    }
}

/**
 * warning - 编译警告处理
 * @fmt - 参数输出的格式
 * @ap - 可变参数列表
 * **/
void warning(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    handle_exception(STAGE_COMPILE, LEVEL_WARNING, fmt, ap);
    va_end(ap);
}

/**
 * error - 参数输出的格式
 * @ap - 可变参数列表
 * **/
void error(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    handle_exception(STAGE_COMPILE, LEVEL_ERROR, fmt, ap);
    va_end(ap);
}

/**
 * expect - 提示错误，此处缺少某个语法成分
 * @msg - 需要什么语法成分
 * **/
void expect(char* msg)
{
    error("缺少%s", msg);
}

/**
 * skip - 跳过单词c，去下一单词，如果当前单词不是c，提示错误
 * @c - 要跳过的单词
 * **/
void skip(int c)
{
    if(token != c) {
        error("缺少'%s'", get_tkstr(c));
    }
}