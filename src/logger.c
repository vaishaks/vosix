#include "logger.h"

void log(char* msg, log_level level)
{
    if (level <= CURRENT_LOG_LEVEL)
        printf(msg);
}