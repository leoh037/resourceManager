#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(){

    int resources = 2;
    int tasks = 2;
    int columns = 2;

    // int *** resourcesTable = malloc(resources * sizeof(int **));
    // int ** allocationState;
    // for(int i = 0; i < resources; i++){
    //     allocationState = malloc(tasks * sizeof(int *));
    //     for(int k = 0; k < tasks; k++){
    //         allocationState[k] = malloc(2 * sizeof(int));
    //     }
    //     resourcesTable[i] = allocationState;
    // }

    // resourcesTable[0][0][0] = 0;
    // resourcesTable[0][0][1] = 6;
    // resourcesTable[0][1][0] = 0;
    // resourcesTable[0][1][1] = 5;
    // resourcesTable[1][0][0] = 0;
    // resourcesTable[1][0][1] = 4;
    // resourcesTable[1][1][0] = 0;
    // resourcesTable[1][1][1] = 4;

    // int ** array = resourcesTable[1];

    // printf("value = %d\n", array[1][1]);
    //printf("value = %d\n", resourcesTable[0][0][1]);

    //////////////////////////////////////////////////////////////////////////////////
    // int *** ptptp = malloc(resources*sizeof(int **));

    int ** arraytest = malloc(tasks*sizeof(int *));
    int * r1 = malloc(columns*sizeof(int));
    int * r2 = malloc(columns*sizeof(int));

    r1[0] = 5;
    r1[1] = 2;
    r2[0] = 3;
    r2[1] = 4;

    arraytest[0] = r1; // *array = r1;
    arraytest[1] = r2; // *(array + 1) = r2;

    printf("array = %p\n", arraytest);
    printf("r1 = %p, array[0] = %p, *array = %p\n", r1, arraytest[0], *arraytest);
    printf("r1[0] = %d, array[0][0] = %d, *array[0] = %d, **array = %d\n", r1[0], arraytest[0][0], *arraytest[0], **arraytest);

    // ptptp[0] = array;

    // printf("ptptp[0] = %p, array = %p\n", ptptp[0], array);
    // printf("ptptp[0][0][0] = %d\n", ptptp[0][1][0]);

    // printf("huh...");

}