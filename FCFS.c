#include <stdio.h>

struct Process {
    char id[5];
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
};

int main() {
    int n;
    printf("Nhap so tien trinh: ");
    scanf("%d", &n);

    struct Process p[n];

    // Nhập thông tin tiến trình
    for (int i = 0; i < n; i++) {
        printf("Tien trinh P%d:\n", i + 1);
        sprintf(p[i].id, "P%d", i + 1);
        printf("  Thoi diem den: ");
        scanf("%d", &p[i].arrival_time);
        printf("  Thoi gian chay: ");
        scanf("%d", &p[i].burst_time);
    }

    // Sắp xếp theo thời điểm đến
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].arrival_time > p[j].arrival_time) {
                struct Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    int current_time = 0;
    float total_waiting = 0, total_turnaround = 0;

    printf("\n%-8s %-10s %-10s %-10s %-10s %-12s %-15s\n",
           "Tien trinh", "Den", "Chay", "Bat dau", "Ket thuc", "Waiting", "Turnaround");

    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time) {
            current_time = p[i].arrival_time;
        }
        p[i].start_time = current_time;
        p[i].completion_time = current_time + p[i].burst_time;
        p[i].waiting_time = p[i].start_time - p[i].arrival_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;

        current_time = p[i].completion_time;

        total_waiting += p[i].waiting_time;
        total_turnaround += p[i].turnaround_time;

        printf("%-8s %-10d %-10d %-10d %-10d %-12d %-15d\n",
               p[i].id, p[i].arrival_time, p[i].burst_time, p[i].start_time,
               p[i].completion_time, p[i].waiting_time, p[i].turnaround_time);
    }

    printf("\n⏳ Thoi gian cho trung binh: %.2f\n", total_waiting / n);
    printf("🔁 Thoi gian hoan thanh trung binh: %.2f\n", total_turnaround / n);

    return 0;
}
