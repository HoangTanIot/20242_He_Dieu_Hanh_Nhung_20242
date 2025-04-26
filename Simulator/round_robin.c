#include <stdio.h>
#include <string.h>

struct Process {
    char id[5];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

int main() {
    int n, time_quantum;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    struct Process p[n];

    // Input process details
    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i + 1);
        printf("  ID: "); scanf("%s", p[i].id);
        printf("  Arrival Time: "); scanf("%d", &p[i].arrival_time);
        printf("  Burst Time: "); scanf("%d", &p[i].burst_time);
        p[i].remaining_time = p[i].burst_time;
        p[i].completion_time = 0;
    }

    int time = 0, completed = 0;
    int queue[100], front = 0, rear = 0;
    int in_queue[n];
    memset(in_queue, 0, sizeof(in_queue));

    // Enqueue the first process
    for (int i = 0; i < n; i++) {
        if (p[i].arrival_time == time) {
            queue[rear++] = i;
            in_queue[i] = 1;
        }
    }

    printf("\n👉 Gantt Chart: ");
    while (completed < n) {
        if (front == rear) {
            time++;
            for (int i = 0; i < n; i++) {
                if (p[i].arrival_time == time && !in_queue[i]) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                }
            }
            continue;
        }

        int i = queue[front++];
        int exec_time = (p[i].remaining_time >= time_quantum) ? time_quantum : p[i].remaining_time;

        printf(" | %s ", p[i].id);
        time += exec_time;
        p[i].remaining_time -= exec_time;

        // Enqueue new processes that arrived during execution
        for (int j = 0; j < n; j++) {
            if (p[j].arrival_time > time - exec_time && p[j].arrival_time <= time && !in_queue[j]) {
                queue[rear++] = j;
                in_queue[j] = 1;
            }
        }

        // Requeue if not finished
        if (p[i].remaining_time > 0) {
            queue[rear++] = i;
        } else {
            p[i].completion_time = time;
            completed++;
        }
    }
    printf("|\n");

    // Calculate turnaround and waiting times
    float total_tt = 0, total_wt = 0;
    printf("\n%-5s %-12s %-10s %-15s %-15s %-15s\n", "ID", "Arrival", "Burst", "Completion", "Turnaround", "Waiting");
    for (int i = 0; i < n; i++) {
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

        total_tt += p[i].turnaround_time;
        total_wt += p[i].waiting_time;

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
