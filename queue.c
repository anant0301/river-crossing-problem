#include "headers.h"

Queue *init_queue(int capacity)
{
    Queue *queue = calloc(1, sizeof(Queue));
    queue->capacity = capacity;
    queue->rear = queue->front = 0;
    queue->q = calloc(capacity, sizeof(int));
    return queue;
}

int is_empty(Queue *queue)
{
    if (queue->rear == queue->front)
    {
        return 1;
    }
    return 0;
}

int is_full(Queue *queue)
{
    if (queue->front == queue->capacity)
    {
        return 1;
    }
    return 0;
}

int dequeue(Queue *queue)
{
    if (is_empty(queue))
    {
        return ERR;
    }
    int x = queue->q[queue->front++];
    return x;
}

int enqueue(Queue *queue, int x)
{
    if (is_full(queue))
    {
        return ERR;
    }
    queue->q[queue->rear++] = x;
    return x;
}

int get_count(Queue *queue)
{
    return queue->rear - queue->front;
}

int peek(Queue *queue) {
    if (is_empty(queue)) {
        return ERR;
    }
    return queue->q[queue->front];
}