#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logging/log.h"

static FILE *logOut = NULL;
static LoggingLevel minLevel = TRACE;
static pthread_mutex_t lock;

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

    if(pthread_mutex_init(&lock, NULL) < 0) {
        printf("Mutex initialization failed.\n");
        return -1;
    }
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

    if (pthread_mutex_lock(&lock) < 0) {
        printf("Mutex lock failed\n");
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

    if (pthread_mutex_unlock(&lock) < 0) {
        printf("Mutex unlock failed.\n");
        return -1;
    }
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

    if (pthread_mutex_destroy(&lock) < 0) {
        printf("Mutex destroy failed.\n");
        return -1;
    }
    return 0;
}

