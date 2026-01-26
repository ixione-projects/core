#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	void *(*front)(const void *queue);
	void *(*back)(const void *queue);
	void (*push_front)(void *queue, void *value);
	void *(*pop_back)(void *queue, void *rvalue);
	size_t (*size)(const void *queue);
	bool (*is_empty)(const void *queue);
} QueueVTable;

typedef struct Queue Queue;

Queue *NewQueue(void *backend, QueueVTable *vtable);
void DeleteQueue(Queue *stack);

void *QueueFront(const Queue *stack);
void *QueueBack(const Queue *stack);
void QueuePushFront(Queue *stack, void *value);
void *QueuePopBack(Queue *stack, void *rvalue);

size_t QueueSize(const Queue *stack);
bool QueueIsEmpty(const Queue *stack);

#ifdef __cplusplus
}
#endif

#endif // QUEUE_H
