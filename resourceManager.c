#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "blockedQueue.h"

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
    int numberOfResources;
    int *availableResources; // used to dynamially allocated an array to keep track of the number of available resources
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
            availableResources[resourceIndex] = inputValue;
            resourceIndex++;
            }
            else if(i == 0){
                numberOfTasks = inputValue;
            }
            else if(i == 2){
                numberOfResources = inputValue;
                availableResources = malloc(numberOfResources * sizeof(int));
            }
        }
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

                    if(value <= availableResources[resourceIndex]){
                        newTask->claims = malloc(numberOfResources * sizeof(int));
                        newTask->currentAllocation = malloc(numberOfResources * sizeof(int));
                        newTask->taskNumber = taskNumber;
                        newTask->blockState = 0;
                        newTask->terminationState = 0;
                        newTask->abortState = 0;
                        newTask->head = NULL;
                        newTask->currentAction = NULL;
                        newTask->next = NULL;
                    } else{
                        newTask->abortState = 1;
                    }

                    currentProcessExists = 1;

                } else if(currentProcessExists && value > availableResources[resourceIndex]){
                    tasks[taskIndex]->abortState = 1;
                }

                if(tasks[taskIndex]->abortState == 0){
                    tasks[taskIndex]->claims[resourceIndex] = value; 
                } else{
                    currentTaskAborted = 1;
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




    int numberOfActiveProcesses = numberOfTasks;
    int cycle = 0;
    while(numberOfActiveProcesses > 0){
 
    }

}