#include "stats.h"
#include <stdio.h>
#include <sys/inotify.h>

typedef struct {
    int total;
    int create_count;
    int delete_count;
    int modify_count;
    int alert_count;
} Stats;

static Stats stats;

void init_stats() {
    stats.total = 0;
    stats.create_count = 0;
    stats.delete_count = 0;
    stats.modify_count = 0;
    stats.alert_count = 0;
}

void update_stats(int mask) {
    stats.total++;

    if (mask & IN_CREATE)
        stats.create_count++;

    if (mask & IN_DELETE)
        stats.delete_count++;

    if (mask & IN_MODIFY)
        stats.modify_count++;
}

void increment_alerts() {
    stats.alert_count++;
}

void print_stats() {
    printf("\n===== STATISTIQUES =====\n");
    printf("Total événements : %d\n", stats.total);
    printf("Créations         : %d\n", stats.create_count);
    printf("Suppressions      : %d\n", stats.delete_count);
    printf("Modifications     : %d\n", stats.modify_count);
    printf("Alertes           : %d\n", stats.alert_count);
    printf("========================\n");
}
