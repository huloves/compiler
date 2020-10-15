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

#endif
