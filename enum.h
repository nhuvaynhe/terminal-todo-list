typedef enum KeyProcess {
    INSERT = 'i',
    DELETE = 'd',
    CHANGE_STATUS = 'c',
    EDIT_TASK = 'e'
} KeyProcess;

typedef enum TaskStatus {
    PENDING,
    IN_PROGRESS,
    DONE,
} TaskStatus;
