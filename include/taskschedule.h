#ifndef VARIABLES_INCL
#define VARIABLES_INCL
#include <variables.h>
#endif

extern int taskCount;
extern int taskIndex;
extern int taskCurrent;
extern int taskNext;
extern int taskLast;

extern int highCount;
extern int highIndex;
extern int highCurrent;
extern int highNext;
extern int highLast;

extern int normalCount;
extern int normalIndex;
extern int normalCurrent;
extern int normalNext;
extern int normalLast;

extern int lowCount;
extern int lowIndex;
extern int lowCurrent;
extern int lowNext;
extern int lowLast;


typedef enum taskState{
    TASK_RUNNING,
    TASK_SLEEPING,
    TASK_WAITING,
    TASK_STOPPED
}taskState_t;

typedef enum taskPriority{
    TASK_HIGH,
    TASK_NORMAL,
    TASK_LOW
}taskPriority_t;

typedef struct task{
    char* name;
    int pid;
    taskPriority_t priority;
    taskState_t state;
    void (*task)();
    void (*taskArgs)();
    void (*taskReturn)();
}task_t;

extern task_t taskList[65536];
extern task_t highPriorityTaskList[21845];
extern task_t normalPriorityTaskList[21845];
extern task_t lowPriorityTaskList[21845];


extern void taskInit();

extern void taskAdd(char* name, taskPriority_t priority, void (*task)(), void (*taskArgs)(), void (*taskReturn)());

/*cycle task with priority based round robin*/
extern void cycleTask(registers_t* registers);