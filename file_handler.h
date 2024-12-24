#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

int FILE_write(const char *path, char *content, enumbool isWriteWholeFile);
int FILE_read(const char *path, char *buffer, size_t size);

#endif