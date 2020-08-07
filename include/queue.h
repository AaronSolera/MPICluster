#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct qNode
{
	void *data;
	struct qNode *next;
};

struct Queue
{
	int size;
	int element_size;
	struct qNode *front;
	struct qNode *rear;
};

/* 
	Function for initializing the queue 
*/
int initQueue(struct Queue *queue, int element_size);

/* 
	Function for creating a qNode 
*/
struct qNode * createqNode(void *element, int element_size);

/* 
	Function for checking whether the queue is empty
*/
int isEmpty(struct Queue *queue);

/* 
	Inserts an element at rear of queue
*/
int enqueue(struct Queue *queue, void *element);

/* 
	Removes an element from front of queue and gets its value
*/
int dequeue(struct Queue *queue, void *target);

/* 
	Deallocates the memory allocated to queue 
*/
int destroy(struct Queue *queue);

/*
	Function this function put front to queque
*/
void deenQueue(struct Queue *queue);

#endif
