#include "log_queue.h"

#include <pthread.h>

static log_message_t queue[MAX_LOG_QUEUE];

static int front = 0;
static int rear = 0;
static int count = 0;

static int stop=0;

static pthread_mutex_t mutex =
    PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t not_empty =
    PTHREAD_COND_INITIALIZER;

static pthread_cond_t not_full =
    PTHREAD_COND_INITIALIZER;

// =====================================================
// PUSH
// =====================================================

void log_queue_push(log_message_t log)
{
    pthread_mutex_lock(&mutex);

    while(count == MAX_LOG_QUEUE && !stop)
    {
        pthread_cond_wait(&not_full,
                          &mutex);
    }

    if(stop)
    {
        pthread_mutex_unlock(&mutex);
        return;
    }

    queue[rear] = log;

    rear = (rear + 1) % MAX_LOG_QUEUE;

    count++;

    pthread_cond_signal(&not_empty);

    pthread_mutex_unlock(&mutex);
}

// =====================================================
// POP
// =====================================================

log_message_t log_queue_pop()
{
    pthread_mutex_lock(&mutex);

    while(count == 0 && !stop)
    {
        pthread_cond_wait(&not_empty,
                          &mutex);
    }

    if(stop)
    {
        log_message_t empty = {0};

        pthread_mutex_unlock(&mutex);

        return empty;
    }

    log_message_t log = queue[front];

    front = (front + 1) % MAX_LOG_QUEUE;

    count--;

    pthread_cond_signal(&not_full);

    pthread_mutex_unlock(&mutex);

    return log;
}

// =====================================================
// STOP
// =====================================================

void log_queue_stop()
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

void log_queue_destroy()
{
    pthread_mutex_destroy(&mutex);

    pthread_cond_destroy(&not_empty);

    pthread_cond_destroy(&not_full);
}

