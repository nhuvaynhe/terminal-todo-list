#include <stdio.h>

int FILE_write(const char *path, char *content)
{
    FILE *fptr;

    fptr = fopen(path, "w");
    if (fptr == NULL) {
        printf("FILE: file isn't exist.\n");
        return -1;
    }

    fprintf(fptr, "%s", content);
    fclose(fptr);

    return 0;
}

int FILE_read(const char *path, char *buffer, size_t size)
{
    FILE *fptr;
    fptr = fopen(path, "r");
    if (fptr == NULL) {
        printf("FILE: file doesn't exist.\n");
        return -1;
    }

    size_t bytes_read = fread(buffer, 1, size - 1, fptr); 
    buffer[bytes_read] = '\0'; 

    fclose(fptr);
    return 0;
}