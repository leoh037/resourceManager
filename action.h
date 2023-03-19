/**
 *  action.h
 *
 *  Full Name:
 *  Course section:
 *  Representation of an action a process can take (request or release) 
 *  
 */


#ifndef ACTION_H
#define ACTION_H

// representation of a a process
typedef struct action {
    int  actionType; // 0 = request, 1 = release
    int  delay;
    int  resourceType;  
    int  resourceAmmount;
    struct action *next;
} Action;

#endif