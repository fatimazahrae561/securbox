#include "queue.h"
#include <pthread.h>

static event_t queue[MAX_QUEUE];

static int front = 0;
static int rear = 0;
static int count = 0;

static int stop=0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;

// =====================================================
// PUSH
// =====================================================

void queue_push(event_t event)
{
    pthread_mutex_lock(&mutex);

    while(count == MAX_QUEUE && !stop)
    {
        pthread_cond_wait(&not_full,
                          &mutex);
    }

    if(stop)
    {
        pthread_mutex_unlock(&mutex);
        return;
    }

    queue[rear] = event;

    rear = (rear + 1) % MAX_QUEUE;

    count++;

    pthread_cond_signal(&not_empty);

    pthread_mutex_unlock(&mutex);
}

// =====================================================
// POP
// =====================================================

event_t queue_pop()
{
    pthread_mutex_lock(&mutex);

    while(count == 0 && !stop)
    {
        pthread_cond_wait(&not_empty,
                          &mutex);
    }

    if(stop)
    {
        event_t empty = {0};

        pthread_mutex_unlock(&mutex);

        return empty;
    }

    event_t event = queue[front];

    front = (front + 1) % MAX_QUEUE;

    count--;

    pthread_cond_signal(&not_full);

    pthread_mutex_unlock(&mutex);

    return event;
}

// =====================================================
// STOP
// =====================================================

void queue_stop()
{
    pthread_mutex_lock(&mutex);

    stop = 1;

    pthread_cond_broadcast(&not_empty);

    pthread_cond_broadcast(&not_full);

    pthread_mutex_unlock(&mutex);
}

// =====================================================
// DESTROY
// =====================================================

void queue_destroy()
{
    pthread_mutex_destroy(&mutex);

    pthread_cond_destroy(&not_empty);

    pthread_cond_destroy(&not_full);
}

