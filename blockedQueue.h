/**
 *  blockQueque.h
 *
 *  Full Name:
 *  Course section:
 *  Description of the program: queue that keeps track of the currently blocked processes
 * 
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"

//add a new node to the back of the queque (tail of the linked list)
void enqueue(Task **pHead, Task **pTail, Task *newNode) {
    //if the queue is empty
    if(*pHead == NULL){
        *pHead = newNode;
        *pTail = newNode;
    } else{
        //if queue has at least one node in it
        (*pTail) -> next = newNode;
        *pTail = newNode;
    }
    newNode -> next = NULL;
}

//remove a node from the front of the queue (head of the linked list)
// void dequeue(Task **pHead){
//     //only dequeue if the queue is not empty
//     if(*pHead != NULL){
//         Task *dequeued = *pHead;
//         *pHead = (*pHead) -> next;
//         dequeued->next = NULL;  // (*dequeued).next = NULL;
//         //printf("removed process with id = %d from the queue\n", dequeued->process->pid);
//     }
// }

//remove a node from the front of the queue (head of the linked list)
Task * dequeue(Task **pHead){
    //only dequeue if the queue is not empty, otherwise returns null
    Task *dequeued = NULL;
    if(*pHead != NULL){
        dequeued = *pHead;
        *pHead = (*pHead) -> next;
        dequeued->next = NULL;  // (*dequeued).next = NULL;
    }
    return dequeued;
}

//get the size of the queue
int getSize(Task **pHead){
    int count = 0;
    if(*pHead != NULL){
        Task *current = *pHead;
        while(current != NULL){
            current = current -> next;
            count++;
        }
    }
    return count;
}