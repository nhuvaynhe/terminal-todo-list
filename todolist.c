#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
    strftime(date, 50, "\%d%b %Y", rawtime);
    return date;
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

    snprintf(buffer, 100, "%s\n%s\n%d\n", 
                           task->date, task->content, task->status);
    FILE_write(path, buffer);
}

static void DisplayTask()
{
    char buffer[100];

    FILE_read(path, buffer, 100);
    printf("%s", buffer);
}

static void InsertTask()
{
    char task[100];

    printf("Enter task: ");
    // Clear leftover newline in buffer before using fgets
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

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

static void WaitUserInput()
{
    printf("\nEnter command: ");
    int ch = fgetc(stdin);
    while (ch == '\n') {
        ch = fgetc(stdin); // Consume stray newline characters
    }

    if (ch != EOF) {
        char key = (char)ch;
        MainProcess(key);
    }
}


int main()
{
    ClearScreen();

    while(1) {
        DisplayTask();
        WaitUserInput();
    }
    return 0;
}