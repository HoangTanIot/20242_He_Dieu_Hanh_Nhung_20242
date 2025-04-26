#include <stdio.h>

struct Process {
    int arrival_time;
    int burst_time;
    int start_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
};

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    // Input
    for (int i = 0; i < n; i++) {
        printf("Arrival time of process %d: ", i + 1);
        scanf("%d", &p[i].arrival_time);
        printf("Burst time of process %d: ", i + 1);
        scanf("%d", &p[i].burst_time);
    }

    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    // FCFS Scheduling
    int time = 0;
    float total_waiting = 0, total_turnaround = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival_time)
            time = p[i].arrival_time;

        p[i].start_time = time;
        p[i].finish_time = time + p[i].burst_time;
        p[i].waiting_time = p[i].start_time - p[i].arrival_time;
        p[i].turnaround_time = p[i].finish_time - p[i].arrival_time;

        total_waiting += p[i].waiting_time;
        total_turnaround += p[i].turnaround_time;

        time = p[i].finish_time;
    }

    // Output
    printf("\nProcess | Arrival | Burst | Start | Finish | Waiting | Turnaround\n");
    for (int i = 0; i < n; i++) {
        printf("  P%-5d|   %-6d|  %-5d|  %-5d|   %-6d|   %-6d|    %-6d\n",
            i + 1,
            p[i].arrival_time,
            p[i].burst_time,
            p[i].start_time,
            p[i].finish_time,
            p[i].waiting_time,
            p[i].turnaround_time
        );
    }

    printf("\n⏳ Average Waiting Time: %.2f\n", total_waiting / n);
    printf("🔁 Average Turnaround Time: %.2f\n", total_turnaround / n);

    return 0;
}
