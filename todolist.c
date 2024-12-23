#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#include "task.h"
#include "file_handler.h"

const char *path = "./database.txt";
static int currentProc = Proc_None;
static int shouldClearScreen = 0;


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

static char peekChar(char *c) {
    /* Read but not consume character */
    return *(++c);
}

static int getLastIndex()
{
    int lastIndex = -1;

    char buffer[100] = {0x00};
    char *buf_ptr = buffer;

    if (FILE_read(path, buffer, sizeof(buffer)) < 0) {
        return 0;
    }
    else if (buffer[0] == 0x00) {
        return 0;
    } 
    else {
        lastIndex = buf_ptr[0] - '0';
    }

    while(*(buf_ptr) != '\0')  {
        if (*(buf_ptr) == '\n' && peekChar(buf_ptr) != '\0') {
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

static void ChangeTaskContent(Task *task, char *editContent)
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
    char buffer[200];
    char *buffer_ptr = buffer;

    if (FILE_read(path, buffer, sizeof(buffer)) < 0) {
        return;
    }

    char id[2];
    char date[16]; 
    char taskDescription[48];
    char taskStatus[2];

    char *line = strtok(buffer_ptr, "\n");

    while (line != NULL)
    {
        int ret = sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|", id, taskDescription, taskStatus, date);
        if (ret == 4) {
            int status = taskStatus[0] - '0';

            if (status == IN_PROGRESS) {
                printf("  %s   [ ]", id);
            }
            else if (status == COMPLETED) {
                printf("  %s   [X]", id);
            }
            printf("   %s   ", date);
            printf("%s\n", taskDescription);
        } 
        else {
            printf("error: database error\n");
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
    }

    currentProc = Proc_None;
}


static void EditTask()
{
    char index[2];
    printf("Type the task index you want to edit: ");

    if (fgets(index, sizeof(index), stdin)) {
        // Remove trailing newline from fgets
        size_t len = strlen(index);
        if (len > 0 && index[len - 1] == '\n') {
            index[len - 1] = '\0';
        }
        printf("You choose task %c\n", index[0]);
    }
}

static void MainProcess()
{
    static int lastIndex = -1;
    int currentIndex = getLastIndex();  

    if (currentIndex != lastIndex) {
        shouldClearScreen = 1;
        lastIndex = currentIndex;
    }

    if (shouldClearScreen) {
        ClearScreen();
        DisplayTask();
        shouldClearScreen = 0;
    }

    switch (currentProc)
    {
        case Proc_InsertTask:
        {
            InsertTask();
            ClearScreen();
        } break;

        case Proc_DeleteTask:
        {
            ClearScreen();
        } break;
        
        case Proc_ChangeStatus:
        {
            ClearScreen();
        } break;

        case Proc_EditTask:
        {
            EditTask();
            ClearScreen();
        } break;

        default:
        {
            ;
        } break;
    }
}

static void WaitUserInput()
{
    if (currentProc != Proc_None) {
        return;
    }

    char key = getch();
    switch (key)
    {
        case INSERT:
        {
            currentProc = Proc_InsertTask;
        } break;

        case DELETE_TASK:
        {
            currentProc = Proc_DeleteTask;
        } break;

        case CHANGE_STATUS:
        {
            currentProc = Proc_ChangeStatus;
        } break;

        case EDIT_TASK:
        {
            currentProc = Proc_EditTask;
        } break;
    
        default:
        {
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
        MainProcess();
    }

    return 0;
}