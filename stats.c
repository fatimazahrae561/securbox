#include "stats.h"

#include <stdio.h>
#include <sys/inotify.h>
#include <time.h>

// ================= STRUCT =================

typedef struct {

    int total;

    int create_count;

    int delete_count;

    int modify_count;

    int rename_count;

    int alert_count;

    time_t start_time;

} Stats;

// ================= GLOBAL =================

static Stats stats;

// ================= INIT =================

void init_stats() {

    stats.total = 0;

    stats.create_count = 0;

    stats.delete_count = 0;

    stats.modify_count = 0;

    stats.rename_count = 0;

    stats.alert_count = 0;

    stats.start_time = time(NULL);
}

// ================= UPDATE =================

void update_stats(int mask) {

    stats.total++;

    // CREATE
    if (mask & IN_CREATE)
        stats.create_count++;

    // DELETE
    if (mask & IN_DELETE)
        stats.delete_count++;

    // MODIFY
    if (mask & IN_MODIFY)
        stats.modify_count++;

    // RENAME / MOVE
    if ((mask & IN_MOVED_FROM) ||
        (mask & IN_MOVED_TO))
        stats.rename_count++;
}

// ================= ALERTS =================

void increment_alerts() {

    stats.alert_count++;
}

// ================= PRINT =================

void print_stats() {

    time_t now = time(NULL);

    int duration =
        (int)difftime(now,
                      stats.start_time);

    printf("\n===== STATISTIQUES =====\n");

    printf("Total événements : %d\n",
           stats.total);

    printf("Créations         : %d\n",
           stats.create_count);

    printf("Suppressions      : %d\n",
           stats.delete_count);

    printf("Modifications     : %d\n",
           stats.modify_count);

    printf("Renommages        : %d\n",
           stats.rename_count);

    printf("Alertes           : %d\n",
           stats.alert_count);

    printf("Durée exécution   : %d secondes\n",
           duration);

    printf("========================\n");
}
