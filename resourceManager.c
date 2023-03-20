#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "blockedQueue.h"

void updateCycleResources(int *availableResources, int *availableResourcesCycle, int numberOfResources);
void updateResourceTable(int *** resourceTable, int *** subtractTable, int numberOfResources, int numberOfTasks);
int requestIsSafe(int ** array, int requestAmmount, int freeAmmount, int numberOfTasks);

int main(){

    char * input[] = {
        "2 1 4",
        "initiate  1 0 1 4",
        "request   1 0 1 1",
        "release   1 0 1 1",
        "terminate 1 0 0 0",
        "initiate  2 0 1 4",
        "request   2 0 1 1",
        "release   2 0 1 1",
        "terminate 2 0 0 0"
    };

    /////////////////////////////////////////////////////////////////////////////////

    printf("running resource manager...\n");

    int numberOfTasks;
    int numberOfActiveTasks = numberOfTasks;
    int numberOfResources;
    int *reserveResources; // used to dynamially allocated an array to keep track of the number of resources in reserve
    int *availableResources; // used to dynamially allocated an array to keep track of the number of resources available during a cycle
    int resourceIndex; // index of the resource's ammount in the availableResourcesArray (note that resource-type numbering starts from 1, but indexing starts from 0)

    int firstInputLenght = strlen(input[0]);
    char currentChar;
    resourceIndex = 0;
    int inputValue;
    for(int i = 0; i < firstInputLenght; i++){
        currentChar = input[0][i];
        if(currentChar != ' '){
            inputValue = atoi(&currentChar);
            if(i > 2){
            //*(availableResources + resourceIndex) = inputValue;
            reserveResources[resourceIndex] = inputValue;
            resourceIndex++;
            }
            else if(i == 0){
                numberOfTasks = inputValue;
            }
            else if(i == 2){
                numberOfResources = inputValue;
                reserveResources = malloc(numberOfResources * sizeof(int));
                availableResources = malloc(numberOfResources * sizeof(int));
                for(int i = 0; i < numberOfResources; i++){
                    availableResources[i] = 0;
                }
            }
        }
    }

    // creating a table to keep track of the allocation states for every resource (essentially an array of pointers to 2d arrays)
    // for each resource, each table/2d array consisters of multiple rows (each row corresponding to a task), and 2 columns (column 1 holds the number of allocated resources for the task, 
    // while column 2 holds the task's claim)
    int *** resourceTable = malloc(numberOfResources * sizeof(int **));
    int *** subtractTable = malloc(numberOfResources * sizeof(int **));
    int ** allocationState;
    int ** subtractState;
    for(int i = 0; i < numberOfResources; i++){
        allocationState = malloc(numberOfTasks * sizeof(int *));
        subtractState = malloc(numberOfTasks * sizeof(int *));
        for(int k = 0; k < numberOfTasks; k++){
            allocationState[k] = malloc(2 * sizeof(int));
            subtractState[k] = malloc(sizeof(int));
        }
        resourceTable[i] = allocationState;
        subtractTable[i] = subtractState;
    }

    Task **tasks = malloc(numberOfTasks * sizeof(Task*));   //will allocate a buffer big enough to hold n pointers to Process structs
    int taskIndex; // index of the task in the tasks array (note that task-number starts from 1, but indexing starts from 0)

    int numberOfInputs = sizeof(input)/sizeof(input[0]) - 1;

    char activity[10];
    int taskNumber;
    int delay;
    int resourceType;
    int value;

    Task *newTask;
    Action *newAction;
    int currentProcessExists = 0;
    int currentTaskAborted = 0;

    for(int i = 0; i < numberOfInputs; i++){
        sscanf(input[i+1], "%s %d %d %d %d", activity, &taskNumber, &delay, &resourceType, &value);
        taskIndex = taskNumber - 1;
        resourceIndex = resourceType - 1;

        if(!strcmp(activity, "terminate")){
            tasks[taskIndex]->currentAction = tasks[taskIndex]->head;
            currentProcessExists = 0;
            currentTaskAborted = 0;
        }
        else if(!currentTaskAborted){

            if(!strcmp(activity, "initiate")){

                if(!currentProcessExists){

                    newTask = malloc(sizeof(Task));
                    *(tasks + taskIndex) = newTask;

                    if(value <= reserveResources[resourceIndex]){
                        newTask->claims = malloc(numberOfResources * sizeof(int));
                        //newTask->currentAllocation = malloc(numberOfResources * sizeof(int));
                        newTask->taskNumber = taskNumber;
                        newTask->blockState = 0;
                        newTask->terminationState = 0;
                        newTask->abortState = 0;
                        newTask->head = NULL;
                        newTask->currentAction = NULL;
                        newTask->next = NULL;
                        for(int i = 0; i < numberOfResources; i++){

                            //newTask->currentAllocation[i] = 0;

                            for(int k = 0; k < numberOfTasks; k++){
                                resourceTable[i][k][0] = 0;
                                subtractTable[i][k][0] = 0;
                            }
                        }
                    } else{
                        newTask->abortState = 1;
                    }

                    currentProcessExists = 1;

                } else if(currentProcessExists && value > reserveResources[resourceIndex]){
                    tasks[taskIndex]->abortState = 1;
                }

                if(tasks[taskIndex]->abortState == 0){
                    tasks[taskIndex]->claims[resourceIndex] = value; 
                    resourceTable[resourceIndex][taskIndex][1] = value;
                } else{
                    currentTaskAborted = 1;
                    numberOfActiveTasks--;
                }

            } else {

                newAction = malloc(sizeof(Action));

                if(!strcmp(activity, "request")){
                    newAction->actionType = 0;
                } else if (!strcmp(activity, "release")){
                    newAction->actionType = 1;
                }
                newAction->delay = delay;
                newAction->resourceType = resourceType;
                newAction->resourceAmmount = value;
                newAction->next = NULL;
                if(tasks[taskIndex]->head == NULL){
                    tasks[taskIndex]->head = newAction;
                    tasks[taskIndex]->currentAction = newAction;
                } else {
                    tasks[taskIndex]->currentAction->next = newAction;  //(*(*processes[taskIndex]).currentAction).next = newAction;
                    tasks[taskIndex]->currentAction = newAction;
                }
            }
        }
    }

    printf("system ready\n");

    //these 2 pointers are used to maintain a queue of blocked states
    Task *head = NULL;
    Task *tail = NULL;
    int blockedProcesses;

    Task *currentTask;
    Action *currentAction;
    int actionType;
    int actionDelay;
    int actionResourceType;
    int actionResourceAmmount;

    int cycle = 0;
    while(numberOfActiveTasks > 0){
        //updating the resources available for this cycle to account for any released resources during the previous cycle
        updateCycleResources(reserveResources, availableResources, numberOfResources);
        //updating the resource allocation table to account for any released resouces during the previous cycle
        updateResourceTable(resourceTable, subtractTable, numberOfResources, numberOfTasks);

        blockedProcesses = getSize(&head);

        for(int i = 0; i < numberOfTasks; i++){

            //handle any blocked tasks first
            if(blockedProcesses > 0){
                for(int i = 0; i < blockedProcesses; i++){

                    currentTask = dequeue(&head);
                    currentAction = currentTask->currentAction;
                    actionResourceType = currentAction->resourceType;
                    actionResourceAmmount = currentAction->resourceAmmount;

                    int ** array = resourceTable[actionResourceType - 1];

                    if(requestIsSafe(array, actionResourceAmmount, availableResources[actionResourceType-1], numberOfTasks)){
                        // increase the number of the given resource allocated to the task
                        resourceTable[actionResourceType-1][currentTask->taskNumber-1][0] += actionResourceAmmount;
                        // decrease the number of the given resource available for this cycle
                        availableResources[actionResourceType-1] -= actionResourceAmmount;
                        currentAction = currentAction->next;
                        currentTask->blockState = 0;
                        currentTask->unblockedThisCycle = 1;
                    } else{
                        enqueue(&head, &tail, currentTask);
                    }
                }
            }

            currentTask = tasks[i];
            currentAction = currentTask->currentAction;
            actionType = currentAction->actionType;
            actionResourceType = currentAction->resourceType;
            actionResourceAmmount = currentAction->resourceAmmount;

            if(currentTask->terminationState != 1 && currentTask->blockState != 1){

                if(currentTask->unblockedThisCycle != 1){
                    if(actionType == 0){

                        int ** array = resourceTable[actionResourceType - 1];

                        if(requestIsSafe(array, actionResourceAmmount, availableResources[actionResourceType-1], numberOfTasks)){
                            // increase the number of the given resource allocated to the task
                            //currentTask->currentAllocation[actionResourceType-1] += actionResourceAmmount;
                            resourceTable[actionResourceType-1][currentTask->taskNumber-1][0] += actionResourceAmmount;
                            // decrease the number of the given resource available for this cycle
                            availableResources[actionResourceType-1] -= actionResourceAmmount;
                            currentAction = currentAction->next;

                        } else{
                            //block the task and add it to the queue of blocked processes
                            enqueue(&head, &tail, currentTask);
                            currentTask->blockState = 1;
                        }
                    } else if(actionType == 1){
                        // decrease the number of the current resource allocated to the task
                        //currentTask->currentAllocation[actionResourceType-1] -= actionResourceAmmount;

                        // increase the number of the given resource available
                        reserveResources[actionResourceType-1] += actionResourceAmmount;
                        subtractTable[actionResourceType-1][currentTask->taskNumber-1][0] += actionResourceAmmount;
                        currentAction = currentAction->next;

                        if(currentAction == NULL){
                            currentTask->terminationState = 1;
                            currentTask->terminationCycle = cycle + 1;
                            numberOfActiveTasks--;
                        }
                    }
                } else{
                    currentTask->unblockedThisCycle = 0;
                }
            }

        }

        cycle++;
    }

    //print stuff out here once the run has ended

}

void updateCycleResources(int *reserveResources, int *availableResources, int numberOfResources){
    int value;
    for(int i = 0; i < numberOfResources; i++){
        availableResources[i] += reserveResources[i];
        reserveResources[i] = 0;
    }
}

void updateResourceTable(int *** resourceTable, int *** subtractTable, int numberOfResources, int numberOfTasks){
    for(int i = 0; i < numberOfResources; i++){
        for(int k = 0; k < numberOfTasks; k++){
            if(subtractTable[i][k][0] != 0){
                resourceTable[i][k][0] -= subtractTable[i][k][0];
                subtractTable[i][k][0] = 0;
            }
        }
    }
}

int requestIsSafe(int ** array, int requestAmmount, int freeAmmount, int numberOfTasks){

    int ** allocated = malloc(numberOfTasks * sizeof(int *));
    for(int i = 0; i < numberOfTasks; i++){
        allocated[i] = malloc(2 * sizeof(int));
        allocated[i][0] = array[i][0];
        allocated[i][1] = 1;
    }

    int needed;
    int total = numberOfTasks;

    for(int i = 0; i < numberOfTasks; i++){


        
        needed = array[i][1] - allocated[i][0];
        if (needed <= freeAmmount){
            allocated[i][1] = 0;
            freeAmmount += allocated[i][0];
            total--;
        }
    }

    
    
    return 1;
}