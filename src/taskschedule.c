#ifndef STDIO_INCL
#define STDIO_INCL
#include <stdio.h>
#endif

#ifndef STRING_INCL
#define STRING_INCL
#include <string.h>
#endif

#ifndef TASKSCHEDULE_INCL
#define TASKSCHEDULE_INCL
#include <taskschedule.h>
#endif

int taskCount = 0;
int taskIndex = 0;
int taskCurrent = 0;
int taskNext = 0;
int taskLast = 0;

int highCount = 0;
int highIndex = 0;
int highCurrent = 0;
int highNext = 0;
int highLast = 0;

int normalCount = 0;
int normalIndex = 0;
int normalCurrent = 0;
int normalNext = 0;
int normalLast = 0;

int lowCount = 0;
int lowIndex = 0;
int lowCurrent = 0;
int lowNext = 0;
int lowLast = 0;

task_t taskList[65536];
task_t highPriorityTaskList[21845];
task_t normalPriorityTaskList[21845];
task_t lowPriorityTaskList[21845];

void taskInit(){
    for(int i = 0; i < 65536; i++){
        taskList[i].name = "";
        taskList[i].pid = i;
        taskList[i].priority = TASK_NORMAL;
        taskList[i].state = TASK_STOPPED;
        taskList[i].task = 0;
        taskList[i].taskArgs = 0;
        taskList[i].taskReturn = 0;
    }
}

void taskAdd(char* name, taskPriority_t priority, void (*task)(), void (*taskArgs)(), void (*taskReturn)()){
    taskList[taskCount].name = name;
    taskList[taskCount].priority = priority;
    taskList[taskCount].state = TASK_RUNNING;
    taskList[taskCount].task = task;
    taskList[taskCount].taskArgs = taskArgs;
    taskList[taskCount].taskReturn = taskReturn;
    taskCount++;
    switch(priority){
        case TASK_HIGH:
            highPriorityTaskList[highCount] = taskList[taskCount];
            highCount++;
            break;
        case TASK_NORMAL:
            normalPriorityTaskList[normalCount] = taskList[taskCount];
            normalCount++;
            break;
        case TASK_LOW:
            lowPriorityTaskList[lowCount] = taskList[taskCount];
            lowCount++;
            break;
    }
}

void taskStart(){
    taskCurrent = 0;
    taskNext = 0;
    taskLast = 0;
    taskList[taskCurrent].task();
}

/*cycle task with priority based round robin*/
void cycleTask(registers_t* registers){
    if(highCurrent < highCount){
        highPriorityTaskList[highCurrent].task();
        highCurrent++;
    }
    else if(normalCurrent < normalCount){
        normalPriorityTaskList[normalCurrent].task();
        normalCurrent++;
    }
    else if(lowCurrent < lowCount){
        lowPriorityTaskList[lowCurrent].task();
        lowCurrent++;
    }
    if(highCurrent >= highCount){
        highCurrent = 0;
    }
    if(normalCurrent >= normalCount){
        normalCurrent = 0;
    }
    if(lowCurrent >= lowCount){
        lowCurrent = 0;
    }
}