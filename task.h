/**
 *  task.h
 *
 *  Full Name:
 *  Course section:
 *  Representation of a task in the system.
 *  
 */


#ifndef TASK_H
#define TASK_H
#include "action.h"

// representation of a a process
typedef struct task {
    int taskNumber;
    int *claims; // points to array keeping track of the claim for a given resource
    int *currentAllocation; // points to an int array keeping track of currently allocated resources
    int blockState; // 0 = unblocked, 1 = blocked
    int terminationState;  // 0 = active, 1 = terminated
    int terminationCycle;
    int abortState; // 0 = active, 1 = aborted
    Action *head; // points to the first element of the actions list
    Action *currentAction; // points to the current action being performed by the task
    struct task *next;
} Task;

#endif