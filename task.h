#include "enum.h"

typedef struct _Task Task;

typedef struct _Task {
    char *date;
    TaskStatus status;
    char *content;
    Task *nextTask;
} Task;
