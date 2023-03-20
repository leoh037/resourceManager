#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(){

    int resources = 2;
    int tasks = 2;
    int columns = 2;

    int *** resourcesTable = malloc(resources * sizeof(int **));
    int ** allocationState;
    for(int i = 0; i < resources; i++){
        if(i == 0){
            allocationState = malloc(tasks * sizeof(int *));
            resourcesTable[i] = allocationState;
        }
        allocationState[i] = malloc(2 * sizeof(int));
    }

    resourcesTable[0][0][0] = 0;
    resourcesTable[0][0][1] = 6;
    resourcesTable[0][1][0] = 0;
    resourcesTable[0][1][1] = 5;
    resourcesTable[1][0][0] = 0;
    resourcesTable[1][0][1] = 4;
    resourcesTable[1][1][0] = 0;
    resourcesTable[1][1][1] = 4;

    printf("value = %d\n", resourcesTable[1][0][1]);

    //////////////////////////////////////////////////////////////////////////////////
    int *** ptptp = malloc(resources*sizeof(int **));

    int ** array = malloc(tasks*sizeof(int *));

    int * r1 = malloc(columns*sizeof(int));
    int * r2 = malloc(columns*sizeof(int));

    r1[0] = 5;
    r1[1] = 2;
    r2[0] = 3;
    r2[1] = 4;

    array[0] = r1; // *array = r1;
    array[1] = r2; // *(array + 1) = r2;

    // printf("array = %p\n", array);
    // printf("r1 = %p, array[0] = %p, *array = %p\n", r1, array[0], *array);
    // printf("r1[0] = %d, array[0][0] = %d, *array[0] = %d, **array = %d\n", r1[0], array[0][0], *array[0], **array);

    ptptp[0] = array;

    // printf("ptptp[0] = %p, array = %p\n", ptptp[0], array);
    // printf("ptptp[0][0][0] = %d\n", ptptp[0][1][0]);





    


   













}