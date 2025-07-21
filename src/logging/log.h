#ifndef _LOGGING_H_
#define _LOGGING_H_

typedef enum{
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARNING = 3,
    ERROR = 4
} LoggingLevel;

int initLogger(const char *, LoggingLevel);
int logMessage(const char *, LoggingLevel);
int closeLogger(void);

#endif

