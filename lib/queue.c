#include "../include/queue.h"

int initQueue(struct Queue *queue, int element_size)
{
	queue->size = 0;
	queue->element_size = element_size;
	queue->front = NULL;
	queue->rear = NULL;

	return 0;
}

struct qNode * createqNode(void *element, int element_size)
{
	struct qNode *new_qNode = (struct qNode *)calloc(1, sizeof(struct qNode));

	if (new_qNode != NULL)
	{
		new_qNode->data = calloc(1, element_size);
		new_qNode->next = NULL;
		memcpy(new_qNode->data, element, element_size);
	}

	return new_qNode;
}


int isEmpty(struct Queue *queue)
{
	return queue->front == NULL;
}


int enqueue(struct Queue *queue, void *element)
{
	struct qNode *new_qNode = createqNode(element, queue->element_size);

	if (new_qNode == NULL)
		return -1;

	if (!isEmpty(queue))
	{
		queue->rear->next = new_qNode;
		queue->rear = new_qNode;
	}
	else
		queue->front = queue->rear = new_qNode;

	queue->size++;

	return 0;
}


int dequeue(struct Queue *queue, void *target)
{

	if (isEmpty(queue))
		return -1;

	memcpy(target, queue->front->data, queue->element_size);

	struct qNode *tmp = queue->front;
	queue->front = queue->front->next;
	queue->size--;

	free(tmp->data);
	free(tmp);

	return 0;
}

int destroy(struct Queue *queue)
{
	struct qNode *current_qNode;

	while (queue->front != NULL)
	{
		current_qNode = queue->front;

		queue->front = current_qNode->next;

		free(current_qNode->data);
		free(current_qNode);
	}

	return 0;
}

void deenQueue(struct Queue *queue){
	struct qNode * temp = queue->front->next;
	queue->rear->next = queue->front;
	queue->front->next = NULL;
	queue->rear = queue->front;
	queue->front = temp;
}
