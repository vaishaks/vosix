#ifndef _LOGGER_H
#define _LOGGER_H

#include "printf.h"

typedef enum
{
    ERROR = 1,
    WARN,
    DEBUG,
    INFO
} log_level;

#define CURRENT_LOG_LEVEL 3

void log(char* msg, log_level lvl);

#endif