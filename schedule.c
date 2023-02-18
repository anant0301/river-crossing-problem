#include "headers.h"

int nboats_rowed = 0;

Boat *boat;
Rats *rats;
Lions *lions;

// char rat_buf[BUFSIZE];
// char lion_buf[BUFSIZE];

pthread_t *kill_threads;
// thread primitives required
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t rat_board_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t lion_board_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t rat_on_boarding = PTHREAD_COND_INITIALIZER;
pthread_cond_t lion_on_boarding = PTHREAD_COND_INITIALIZER;
pthread_cond_t row_boat_cond = PTHREAD_COND_INITIALIZER;

void initialize(char *instring, int nlion, int nrat)
{
    // initialize global variables
    boat = init_boat();
    rats = init_rats(nrat);
    lions = init_lions(nlion);
    // memset(rat_buf, 0, BUFSIZE);
    // memset(lion_buf, 0, BUFSIZE);

    // make threads for each individual
    pthread_t threads[nrat + nlion];
    pthread_t row_boat_thread;
    // load the input string and create respective threads
    Rat irat = 0;
    Lion ilion = 0;
    for (int i = 0; i < nrat + nlion; ++i)
    {
        switch (instring[i])
        {
        case 'l':
            pthread_create(&threads[i], NULL, lion_routine, (void *)i);
            ++ilion;
            break;
        case 'r':
            pthread_create(&threads[i], NULL, rat_routine, (void *)i);
            ++irat;
            break;
        default:
            printf("Input contains characters other than l/r\n");
            exit(ERR);
        }
    }
    if (ilion != nlion)
    {
        printf("No of lions passed not consistent with input string\n");
        for (int i = 0; i < strlen(instring); i++)
        {
            pthread_cancel(threads[i]);
        }
        exit(ERR);
    }
    else if (irat != nrat)
    {
        printf("No of rats passed not consistent with input string\n");
        exit(ERR);
    }
    pthread_create(&row_boat_thread, NULL, row_boat, instring);
    pthread_cond_signal(&row_boat_cond);
    kill_threads = threads;

    if (nlion)
    {
        pthread_cond_signal(&lion_board_cond);
    }
    if (nrat)
    {
        pthread_cond_signal(&rat_board_cond);
    }
    pthread_join(row_boat_thread, NULL);
}

Rats *init_rats(int nrat)
{
    Rats *rats = calloc(1, sizeof(Rats));
    rats->rat_q = init_queue(nrat);
    rats->nboarded = 0;
    return rats;
}

Lions *init_lions(int nlion)
{
    Lions *lions = calloc(1, sizeof(Rats));
    lions->lion_q = init_queue(nlion);
    lions->nboarded = 0;
    return lions;
}

Boat *init_boat()
{
    Boat *boat = calloc(1, sizeof(Boat));
    boat->nmem = 0;
    return boat;
}

void *rat_routine(void *p)
{
    Rat irat = (Rat)p;
    pthread_mutex_lock(&lock);
    rat_arrive(irat);
    board_boat(irat, rats->rat_q, &rat_board_cond, &rat_on_boarding);
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

void rat_arrive(Rat irat)
{
    enqueue(rats->rat_q, irat);
}

void *lion_routine(void *p)
{
    Lion ilion = (Lion)p;
    pthread_mutex_lock(&lock);
    lion_arrive(ilion);
    board_boat(ilion, lions->lion_q, &lion_board_cond, &lion_on_boarding);
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

void lion_arrive(Lion ilion)
{
    enqueue(lions->lion_q, ilion);
}

void board_boat(int ianimal, Queue *q, pthread_cond_t *waitcond, pthread_cond_t *boardingcond)
{
    do {
        pthread_cond_wait(waitcond, &lock);
    } while(boat->nmem == 4 || ianimal != peek(q));
    dequeue(q);
    // printf("Before: %d %d\n", q->front, ianimal);
    // not the last of the species on boa/t
    if (q->capacity != q->front)
    {
        if (q->front % 2 == 0)
        {
            // odd no of animal of same species on boat
            boat->mem[boat->nmem++] = ianimal;
            pthread_cond_signal(boardingcond);
        }
        else
        {
            // even no of animal of same species on boat
            pthread_cond_signal(waitcond);
            pthread_cond_wait(boardingcond, &lock);
            boat->mem[boat->nmem++] = ianimal;
        }
    }
    else
    {
        // last species on the boat
        // printf("continue %d\n", q->capacity);
        boat->mem[boat->nmem++] = ianimal;
        pthread_cond_signal(boardingcond);

    }
    pthread_cond_signal(&row_boat_cond);
    // printf("After: %d %d\n", ianimal, boat->nmem);
}

void *row_boat(void *p)
{
    char *instring = (char *)p;
    int len = strlen(instring);
    int left = len;
    pthread_mutex_lock(&lock);
    while (left > 4)
    {
        while (
            (boat->nmem != 4)
        )
        {
            pthread_cond_wait(&row_boat_cond, &lock);
        }
        printf("Boat %d Rowed:\n", nboats_rowed++);
        for (int i = 0; i < boat->nmem; ++i)
        {
            if (instring[boat->mem[i]] == 'l')
            {
                // lion on boat
                printf("Lion: %d\n", boat->mem[i]);
            }
            else
            {
                // rat on boat
                printf("Rat: %d\n", boat->mem[i]);
            }
        }
        // printf("------------------------------------\n");
        left -= 4;
        boat->nmem = 0;
        pthread_cond_signal(&rat_board_cond);
        pthread_cond_signal(&lion_board_cond);
    }
    if (left) {
        int x;
        printf("Boat %d Rowed:\n", nboats_rowed++);
        for (int i = 0; i < left; ++i)
        {
            if (lions->lion_q->front != lions->lion_q->capacity)
            {
                // lion on boat
                x = dequeue(lions->lion_q);
                printf("Lion: %d\n", x);
            }
            else if (rats->rat_q->front != rats->rat_q->capacity)
            {
                // rat on boat
                x = dequeue(rats->rat_q);
                printf("Rat: %d\n", x);
            }
        }
        // printf("------------------------------------\n");
    }
    for (int i = 0; i < len; i++) {
        pthread_kill(kill_threads[i], KILLSIGN);
    }
    pthread_mutex_unlock(&lock);
}
