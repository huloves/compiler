#ifndef __LEX_ERROR_H
#define __LEX_ERROR_H

#include <stdarg.h>

enum e_ErrorLevel
{
    LEVEL_WARNING,
    LEVEL_ERROR,
};

enum e_WorkStage
{
    STAGE_COMPILE,
    STAGE_LINK,
};

/**
 * handle_exception - 异常处理
 * @stage - 编译阶段还是链接阶段
 * @level - 错误级别
 * @fmt - 参数输出的格式
 * @ap - 可变参数列表
 * **/
void handle_exception(int stage, int level, char* fmt, va_list ap);

/**
 * warning - 编译警告处理
 * @fmt - 参数输出的格式
 * @ap - 可变参数列表
 * **/
void warning(char* fmt, ...);

/**
 * error - 参数输出的格式
 * @ap - 可变参数列表
 * **/
void error(char* fmt, ...);

/**
 * expect - 提示错误，此处缺少某个语法成分
 * @msg - 需要什么语法成分
 * **/
void expect(char* msg);

/**
 * skip - 跳过单词c，去下一单词，如果当前单词不是c，提示错误
 * @c - 要跳过的单词
 * **/
void skip(int c);

#endif
