#include <stdio.h>
#include <stdbool.h>

struct Process {
    char id[5];
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    bool finished;
};

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    // Input processes
    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i + 1);
        printf("  ID: "); scanf("%s", p[i].id);
        printf("  Arrival Time: "); scanf("%d", &p[i].arrival_time);
        printf("  Burst Time: "); scanf("%d", &p[i].burst_time);
        p[i].finished = false;
    }

    int time = 0, completed = 0;
    float total_tt = 0, total_wt = 0;

    printf("\n👉 Gantt Chart: ");
    while (completed < n) {
        int idx = -1;
        int min_bt = 1e9;

        for (int i = 0; i < n; i++) {
            if (!p[i].finished && p[i].arrival_time <= time && p[i].burst_time < min_bt) {
                min_bt = p[i].burst_time;
                idx = i;
            }
        }

        if (idx != -1) {
            time += p[idx].burst_time;
            p[idx].completion_time = time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;

            total_tt += p[idx].turnaround_time;
            total_wt += p[idx].waiting_time;

            p[idx].finished = true;
            completed++;

            printf(" | %s ", p[idx].id);
        } else {
            time++; // Idle CPU
        }
    }
    printf("|\n");

    // Print table
    printf("\n%-5s %-12s %-10s %-15s %-15s %-15s\n", "ID", "Arrival", "Burst", "Completion", "Turnaround", "Waiting");
    for (int i = 0; i < n; i++) {
        printf("%-5s %-12d %-10d %-15d %-15d %-15d\n",
            p[i].id,
            p[i].arrival_time,
            p[i].burst_time,
            p[i].completion_time,
            p[i].turnaround_time,
            p[i].waiting_time
        );
    }

    printf("\n⏱️  Average Turnaround Time: %.2f\n", total_tt / n);
    printf("⏱️  Average Waiting Time: %.2f\n", total_wt / n);

    return 0;
}
