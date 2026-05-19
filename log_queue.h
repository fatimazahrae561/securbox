#ifndef LOG_QUEUE_H
#define LOG_QUEUE_H

#define MAX_LOG_QUEUE 100

typedef struct {

    char level[32];

    char message[512];

} log_message_t;

void log_queue_push(log_message_t log);

log_message_t log_queue_pop();

#endif
