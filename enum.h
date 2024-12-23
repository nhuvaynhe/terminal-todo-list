typedef int enumbool;

typedef enum KeyProcess {
    INSERT = 'i',
    DELETE_TASK = 'd',
    CHANGE_STATUS = 'c',
    EDIT_TASK = 'e'
} KeyProcess;

typedef enum MainProc {
    Proc_InsertTask,
    Proc_DeleteTask,
    Proc_ChangeStatus,
    Proc_EditTask,

    Proc_None
} MainProc;

typedef enum ProcCommand {
    PROC_CMD_INIT,
    PROC_CMD_PROCESS,
    PROC_CMD_END,

    PROC_CMD_NONE,
} ProcCommand;

typedef enum gfBoolean {
    eTRUE,
    eFALSE,
}  gfBoolean;

typedef enum TaskStatus {
    PENDING,
    IN_PROGRESS,
    COMPLETED,
} TaskStatus;


