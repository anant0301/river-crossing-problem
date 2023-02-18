#ifndef HEADERS
#define HEADERS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#define ERR -1
#define LION 1
#define RAT 0
#define BUFSIZE 32
#define BOATSIZE 4
#define KILLSIGN 9

typedef int Rat;
typedef int Lion;
typedef struct
{
    int *q;
    int rear;
    int front;
    int capacity;
} Queue;
typedef struct
{
    int nboarded;
    Queue *rat_q;
} Rats;
typedef struct
{
    int nboarded;
    Queue *lion_q;
} Lions;
typedef struct
{
    int nmem;
    int mem[BOATSIZE];
} Boat;

void initialize(char *, int, int);
Boat *init_boat();
Rats *init_rats(int);
Lions *init_lions(int);

// animal thread opertaions
void *rat_routine(void *);
void *lion_routine(void *);

// given function definitions
void rat_arrive(Rat);
void lion_arrive(Lion);
void board_boat(Queue *, pthread_cond_t *, pthread_cond_t *);
void *row_boat(void *);

// queue operations
int is_empty(Queue *);
int is_full(Queue *);
int dequeue(Queue *);
int enqueue(Queue *, int);
int get_count(Queue *);
int peek(Queue *queue);
Queue *init_queue(int capacity);

#endif