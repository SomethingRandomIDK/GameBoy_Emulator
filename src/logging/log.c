#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

static FILE *logOut = NULL;
static LoggingLevel minLevel = TRACE;

int initLogger(const char *filename, LoggingLevel minimumLevel) {
    if (logOut) {
        printf("Logger has been initialized already.\n");
        return -1;
    }
    if (filename) {
        logOut = fopen(filename, "a");
        if (!logOut) {
            printf("File failed to open.\n");
            return -1;
        }
    } else
        logOut = stdout;

    minLevel = minimumLevel;

    return 0;
}

static const char* levelMsgs[] = {
    [TRACE] = "[TRACE] ",
    [DEBUG] = "[DEBUG] ",
    [INFO] = "[INFO] ",
    [WARNING] = "[WARNING] ",
    [ERROR] = "[ERROR] "
};

int logMessage(const char *msg, LoggingLevel level) {
    if (level < minLevel)
        return 0;
    if (!logOut){
        printf("Logger has not been initialized.\n");
        return -1;
    }

    size_t buffLen = strlen(levelMsgs[level]) + strlen(msg) + 2;
    char *buff = (char *)malloc(sizeof(char) * buffLen);
    if (!buff) {
        printf("Malloc failed.\n");
        return -1;
    }
    buff[0] = 0;

    strcpy(buff, levelMsgs[level]);
    strcat(buff, msg);
    buff[buffLen - 2] = '\n';
    buff[buffLen - 1] = 0;

    fputs(buff, logOut);
    fflush(logOut);

    free(buff);

    return 0;
}

int closeLogger(void) {
    if (!logOut) {
        printf("Logger has not been initialized.\n");
        return -1;
    }

    fflush(logOut);

    if (logOut != stdout)
        fclose(logOut);

    logOut = NULL;

    return 0;
}

