#include "analyzer.h"
#include "logger.h"
#include "stats.h"
#include "event.h"
#include "queue.h"
#include "log_queue.h"
#include "monitor.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/inotify.h>

#define DELETE_THRESHOLD 5
#define DELETE_TIME_WINDOW 10

#define MODIFY_THRESHOLD 3
#define MODIFY_TIME_WINDOW 5

// =====================================================
// TRACKING STRUCTURES
// =====================================================

typedef struct {

    int count;

    time_t first_time;

} counter_t;

// =====================================================
// GLOBAL COUNTERS
// =====================================================

static counter_t delete_counter = {0, 0};

static counter_t modify_counter = {0, 0};

// =====================================================
// CHECK FORBIDDEN EXTENSIONS
// =====================================================

int is_forbidden_extension(const char *filename) {

   const char *extensions[] = {
    ".sh", ".bash", ".zsh",
    ".bat", ".cmd", ".ps1",
    ".php", ".js", ".jsp", ".asp",
    ".py", ".pl", ".rb", ".lua",
    ".exe", ".com", ".msi", ".run", ".out",
    ".jar", ".apk"
};

    int n =
        sizeof(extensions) /
        sizeof(extensions[0]);

    for (int i = 0; i < n; i++) {

        int len_file = strlen(filename);

        int len_ext =
            strlen(extensions[i]);

        if (len_file >= len_ext &&
            strcmp(filename +
                   len_file -
                   len_ext,

                   extensions[i]) == 0) {

            return 1;
        }
    }

    return 0;
}

// =====================================================
// CHECK HIDDEN FILE
// =====================================================

int is_hidden_file(const char *filename) {

    return filename[0] == '.';
}

// =====================================================
// PUSH LOG
// =====================================================

void push_log(const char *level,
              const char *event,
              const char *filename) {

    log_message_t log;

    strcpy(log.level, level);

    strcpy(log.event, event);

    strcpy(log.resource, filename);

    snprintf(log.path,
             sizeof(log.path),
             "%.120s/%.120s",
             monitored_path,
             filename);

    log_queue_push(log);
}

// =====================================================
// ANALYZE EVENT
// =====================================================

void analyze_event(int mask,
                   const char *filename) {

    time_t now = time(NULL);

    update_stats(mask);

    // =================================================
    // CREATE
    // =================================================

    if (mask & IN_CREATE) {

        // CREATE DIRECTORY

        if (mask & IN_ISDIR) {

            push_log("INFO",
                     "CREATE DIRECTORY",
                     filename);
        }

        // CREATE FILE

        else {

            push_log("INFO",
                     "CREATE FILE",
                     filename);

            // HIDDEN FILE

            if (is_hidden_file(filename)) {

                push_log("ALERT",
                         "HIDDEN FILE",
                         filename);

                increment_alerts();

                printf("[ALERTE] "
                       "Fichier cache detecte : %s\n",
                       filename);
            }

            // FORBIDDEN EXTENSION

            if (is_forbidden_extension(filename)) {

                push_log("ALERT",
                         "FORBIDDEN EXTENSION",
                         filename);

                increment_alerts();

                printf("[ALERTE] "
                       "Extension interdite : %s\n",
                       filename);
            }
        }
    }

    // =================================================
    // DELETE
    // =================================================

    if (mask & IN_DELETE) {

        if (mask & IN_ISDIR) {

            push_log("INFO",
                     "DELETE DIRECTORY",
                     filename);
        }

        else {

            push_log("INFO",
                     "DELETE FILE",
                     filename);
        }

        // TRACK DELETE ACTIVITY

        if (delete_counter.count == 0) {

            delete_counter.first_time =
                now;
        }

        delete_counter.count++;

        if (difftime(now,
                     delete_counter.first_time)

            <= DELETE_TIME_WINDOW) {

            if (delete_counter.count
                >= DELETE_THRESHOLD) {

                push_log("ALERT",
                         "MULTIPLE DELETE DETECTED",
                         filename);

                increment_alerts();

                printf("[ALERTE] "
                       "%d suppressions detectees "
                       "en %d secondes\n",

                       delete_counter.count,
                       DELETE_TIME_WINDOW);

                delete_counter.count = 0;
            }

        } else {

            delete_counter.count = 1;

            delete_counter.first_time =
                now;
        }
    }

    // =================================================
    // MODIFY
    // =================================================

    if (mask & IN_MODIFY) {

        push_log("INFO",
                 "MODIFY FILE",
                 filename);

        if (modify_counter.count == 0) {

            modify_counter.first_time =
                now;
        }

        modify_counter.count++;

        if (difftime(now,
                     modify_counter.first_time)

            <= MODIFY_TIME_WINDOW) {

            if (modify_counter.count
                >= MODIFY_THRESHOLD) {

                push_log("ALERT",
                         "MULTIPLE MODIFY DETECTED",
                         filename);

                increment_alerts();

                printf("[ALERTE] "
                       "Modifications rapides detectees : %s\n",
                       filename);

                modify_counter.count = 0;
            }

        } else {

            modify_counter.count = 1;

            modify_counter.first_time =
                now;
        }
    }

    // =================================================
    // MOVE / RENAME
    // =================================================

    if (mask & IN_MOVED_FROM) {

        push_log("INFO",
                 "MOVED FROM",
                 filename);
    }

    if (mask & IN_MOVED_TO) {

        push_log("INFO",
                 "MOVED TO",
                 filename);
    }
}

// =====================================================
// ANALYZER THREAD
// =====================================================

void *analyzer_thread(void *arg) {

    while (running) {

        event_t event =
            queue_pop();

        analyze_event(event.mask,
                      event.filename);
    }

    return NULL;
}
