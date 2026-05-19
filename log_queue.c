#include "log_queue.h"

#include <pthread.h>

static log_message_t queue[MAX_LOG_QUEUE];

static int front = 0;
static int rear = 0;
static int count = 0;

static pthread_mutex_t mutex =
    PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t not_empty =
    PTHREAD_COND_INITIALIZER;

static pthread_cond_t not_full =
    PTHREAD_COND_INITIALIZER;

void log_queue_push(log_message_t log) {

    pthread_mutex_lock(&mutex);

    while (count == MAX_LOG_QUEUE)
        pthread_cond_wait(&not_full, &mutex);

    queue[rear] = log;

    rear = (rear + 1) % MAX_LOG_QUEUE;

    count++;

    pthread_cond_signal(&not_empty);

    pthread_mutex_unlock(&mutex);
}

log_message_t log_queue_pop() {

    pthread_mutex_lock(&mutex);

    while (count == 0)
        pthread_cond_wait(&not_empty, &mutex);

    log_message_t log = queue[front];

    front = (front + 1) % MAX_LOG_QUEUE;
    count--;

    pthread_cond_signal(&not_full);

    pthread_mutex_unlock(&mutex);

    return log;
}
