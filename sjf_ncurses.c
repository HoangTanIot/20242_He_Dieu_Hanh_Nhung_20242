#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX 10

struct Process {
    char id[5];
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    bool finished;
};

void display_table(WINDOW *win, struct Process p[], int n, float avg_tt, float avg_wt) {
    mvwprintw(win, 1, 2, "%-5s %-10s %-10s %-15s %-15s %-15s", "ID", "Arrival", "Burst", "Completion", "Turnaround", "Waiting");
    for (int i = 0; i < n; i++) {
        mvwprintw(win, i + 2, 2, "%-5s %-10d %-10d %-15d %-15d %-15d",
                  p[i].id,
                  p[i].arrival_time,
                  p[i].burst_time,
                  p[i].completion_time,
                  p[i].turnaround_time,
                  p[i].waiting_time);
    }
    mvwprintw(win, n + 4, 2, "⏱️  Average Turnaround Time: %.2f", avg_tt);
    mvwprintw(win, n + 5, 2, "⏱️  Average Waiting Time: %.2f", avg_wt);
    wrefresh(win);
}

void animate_gantt(WINDOW *win, struct Process p[], int n) {
    int time = 0, completed = 0;
    float total_tt = 0, total_wt = 0;

    mvwprintw(win, 1, 2, "👉 Gantt Chart:");
    wrefresh(win);

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
            for (int i = 0; i < p[idx].burst_time; i++) {
                mvwprintw(win, 2, 2 + time * 5, "|%s", p[idx].id);
                wrefresh(win);
                usleep(300000); // Sleep 0.3s for animation
                time++;
            }

            p[idx].completion_time = time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;

            total_tt += p[idx].turnaround_time;
            total_wt += p[idx].waiting_time;

            p[idx].finished = true;
            completed++;
        } else {
            time++;
        }
    }
    mvwprintw(win, 3, 2 + time * 5, "|");
    wrefresh(win);

    display_table(win, p, n, total_tt / n, total_wt / n);
}

int main() {
    struct Process p[MAX];
    int n;

    printf("Enter number of processes (max %d): ", MAX);
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i + 1);
        printf("  ID: "); scanf("%s", p[i].id);
        printf("  Arrival Time: "); scanf("%d", &p[i].arrival_time);
        printf("  Burst Time: "); scanf("%d", &p[i].burst_time);
        p[i].finished = false;
    }

    // Start ncurses
    initscr();
    noecho();
    curs_set(FALSE);
    WINDOW *win = newwin(30, 100, 1, 1);
    box(win, 0, 0);
    animate_gantt(win, p, n);

    mvwprintw(win, 27, 2, "Press any key to exit...");
    wrefresh(win);
    getch();
    endwin();

    return 0;
}
