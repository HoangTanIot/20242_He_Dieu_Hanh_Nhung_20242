#include <stdio.h>

struct Process {
    int arrival_time;
    int burst_time;
    int start_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
    int queue; // 1 or 2
};

void sort_by_arrival(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

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

        // Queue assignment
        if (p[i].arrival_time < 3)
            p[i].queue = 1;
        else
            p[i].queue = 2;
    }

    // Separate processes into queues
    struct Process q1[n], q2[n];
    int q1_count = 0, q2_count = 0;

    for (int i = 0; i < n; i++) {
        if (p[i].queue == 1)
            q1[q1_count++] = p[i];
        else
            q2[q2_count++] = p[i];
    }

    // Sort each queue by arrival time
    sort_by_arrival(q1, q1_count);
    sort_by_arrival(q2, q2_count);

    // Scheduling
    int time = 0;
    float total_waiting = 0, total_turnaround = 0;
    struct Process final[n];
    int idx = 0;

    // Execute Queue 1
    for (int i = 0; i < q1_count; i++) {
        if (time < q1[i].arrival_time)
            time = q1[i].arrival_time;

        q1[i].start_time = time;
        q1[i].finish_time = time + q1[i].burst_time;
        q1[i].waiting_time = q1[i].start_time - q1[i].arrival_time;
        q1[i].turnaround_time = q1[i].finish_time - q1[i].arrival_time;

        total_waiting += q1[i].waiting_time;
        total_turnaround += q1[i].turnaround_time;

        time = q1[i].finish_time;

        final[idx++] = q1[i];
    }

    // Execute Queue 2
    for (int i = 0; i < q2_count; i++) {
        if (time < q2[i].arrival_time)
            time = q2[i].arrival_time;

        q2[i].start_time = time;
        q2[i].finish_time = time + q2[i].burst_time;
        q2[i].waiting_time = q2[i].start_time - q2[i].arrival_time;
        q2[i].turnaround_time = q2[i].finish_time - q2[i].arrival_time;

        total_waiting += q2[i].waiting_time;
        total_turnaround += q2[i].turnaround_time;

        time = q2[i].finish_time;

        final[idx++] = q2[i];
    }

    // Output
    printf("\nProcess | Arrival | Burst | Start | Finish | Waiting | Turnaround | Queue\n");
    for (int i = 0; i < n; i++) {
        printf("  P%-5d|   %-6d|  %-5d|  %-5d|   %-6d|   %-6d|    %-6d|   Q%-1d\n",
            i + 1,
            final[i].arrival_time,
            final[i].burst_time,
            final[i].start_time,
            final[i].finish_time,
            final[i].waiting_time,
            final[i].turnaround_time,
            final[i].queue
        );
    }

    printf("\n⏳ Average Waiting Time: %.2f\n", total_waiting / n);
    printf("🔁 Average Turnaround Time: %.2f\n", total_turnaround / n);

    return 0;
}
