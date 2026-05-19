#include "queue.h"
#include <pthread.h>

static event_t queue[MAX_QUEUE];

static int front = 0;
static int rear = 0;
static int count = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;

void queue_push(event_t event) {

    pthread_mutex_lock(&mutex);

    while (count == MAX_QUEUE)
        pthread_cond_wait(&not_full, &mutex);

    queue[rear] = event;

    rear = (rear + 1) % MAX_QUEUE;

    count++;

    pthread_cond_signal(&not_empty);

    pthread_mutex_unlock(&mutex);
}

event_t queue_pop() {

    pthread_mutex_lock(&mutex);

    while (count == 0)
        pthread_cond_wait(&not_empty, &mutex);

    event_t event = queue[front];

    front = (front + 1) % MAX_QUEUE;

    count--;

    pthread_cond_signal(&not_full);

    pthread_mutex_unlock(&mutex);

    return event;
}
