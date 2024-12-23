#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>

#include "task.h"
#include "file_handler.h"

const char *path = "./database.txt";

static void ClearScreen() {
    system("cls");
}

static char *getDate()
{
    time_t t;
    struct tm *rawtime;
    char *date = calloc(50, sizeof(char));

    time(&t);
    rawtime = localtime(&t);
    strftime(date, 50, "%d %b %Y", rawtime);

    return date;
}

static int getLastIndex()
{
    int lastIndex = 0;

    char buffer[100];
    char *buf_ptr = buffer;

    if (FILE_read(path, buffer, sizeof(buffer)) < 0) {
        return 0;
    }
    else {
        lastIndex = buf_ptr[0] - '0';
    }

    while(*(buf_ptr) != '\0')  {
        if (*(buf_ptr) == '\n') {
            lastIndex = *(++buf_ptr) - '0';
        } 
        else {
            buf_ptr++;
        }
    }

    return lastIndex;
}

char *getStatus(char status)
{
    int key = status - '0';
    switch (key)
    {
        case PENDING:
            return "PENDING";

        case IN_PROGRESS:
            return "IN_PROGRESS";

        case COMPLETED:
            return "COMPLETED";

        default:
            return "UNKNOWN"; 
    }
}


static Task *createTask(char *task)
{
    Task *newTask = (Task*) malloc(sizeof(Task));
    if (newTask == NULL) {
        exit(1);
    }

    newTask->date = getDate();
    newTask->status = IN_PROGRESS;
    newTask->content = task;
    newTask->nextTask = NULL;

    return newTask;
}

static void EditTask(Task *task, char *editContent)
{
    task->content = editContent;
}

static void WriteTaskToMemory(Task *task)
{
    char buffer[100];

    int lastIdx = getLastIndex() + 1;

    snprintf(buffer, 100, "%c|%s|%d|%s|\n", lastIdx + '0', 
                           task->content, task->status, task->date);
    FILE_write(path, buffer);
}

static void DisplayTask()
{
    char buffer[100];
    char *buffer_ptr = buffer;

    if (FILE_read(path, buffer, sizeof(buffer)) < 0) {
        return;
    }

    char id[2];
    char date[16]; 
    char task[48];
    char status[2];

    char *line = strtok(buffer_ptr, "\n");

    while (line != NULL)
    {
        if (sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|", id, task, status, date) == 4) {
            printf("%s\n", id);
            printf(".Task: %s\n", task);
            printf(".Status: %s\n", getStatus(status[0]));
            printf(".Date: %s\n", date);
            printf("----------------------\n");
        } else {
            printf("error: database error");
        }

        line = strtok(NULL, "\n");
    }
}

static void InsertTask()
{
    char task[100];

    printf("Enter task: ");

    if (fgets(task, sizeof(task), stdin)) {
        // Remove trailing newline from fgets
        size_t len = strlen(task);
        if (len > 0 && task[len - 1] == '\n') {
            task[len - 1] = '\0';
        }

        Task* newtask = createTask(task);
        WriteTaskToMemory(newtask);
        ClearScreen();
    }
}

static void MainProcess(char key)
{
    switch (key)
    {
        case INSERT:
        {
            InsertTask();
        } break;

        case DELETE:
        {
            ;
        } break;
        
        case CHANGE_STATUS:
        {
            ;
        } break;

        case EDIT_TASK:
        {
            ;
        } break;

        default:
        {
            /* Should we told them? */
            exit(1);
        } break;
    }
}

DWORD WINAPI ThreadFunc(void* data) {
    while (1) {
        WaitUserInput(); 
    }
    return 0;
}


int main()
{

    HANDLE thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
    if (thread == NULL) {
        printf("Create thread failed");
        exit(1);
    }

    while(1) {
        DisplayTask();
        WaitUserInput();
    }
    return 0;
}