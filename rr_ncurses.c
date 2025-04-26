#include <ncurses.h>
#include <unistd.h>
#include <string.h>

#define TIME_QUANTUM 2
#define MAX 100

struct Process {
    char id[5];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int is_completed;
};

int main() {
    initscr();
    start_color();
    curs_set(0);
    noecho();

    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);

    struct Process p[3] = {
        {"P1", 0, 5},
        {"P2", 1, 3},
        {"P3", 2, 4}
    };
    int n = 3;

    for (int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        p[i].is_completed = 0;
        p[i].completion_time = 0;
    }

    int time = 0, completed = 0;
    int x = 2, y = 2;
    int queue[MAX], front = 0, rear = 0, in_queue[10] = {0};

    queue[rear++] = 0;
    in_queue[0] = 1;

    while (completed < n) {
        int i = queue[front++];
        in_queue[i] = 0;

        if (p[i].remaining_time > 0) {
            beep();
            int exec_time = (p[i].remaining_time >= TIME_QUANTUM) ? TIME_QUANTUM : p[i].remaining_time;

            attron(COLOR_PAIR(i + 1));
            for (int j = 0; j < exec_time; j++) {
                for (int k = 0; k < 5; k++) {
                    mvprintw(y, x + j * 5 + k, " ");
                }
                mvprintw(y, x + j * 5 + 1, "%s", p[i].id);
                refresh();
                usleep(500000);
            }
            attroff(COLOR_PAIR(i + 1));

            x += exec_time * 5;
            time += exec_time;
            p[i].remaining_time -= exec_time;

            if (p[i].remaining_time == 0 && !p[i].is_completed) {
                p[i].completion_time = time;
                p[i].is_completed = 1;
                completed++;
            }

            // Thêm các tiến trình mới đến hàng đợi
            for (int j = 0; j < n; j++) {
                if (p[j].arrival_time <= time && p[j].remaining_time > 0 && !in_queue[j]) {
                    queue[rear++] = j;
                    in_queue[j] = 1;
                }
            }

            // Nếu tiến trình vẫn còn, cho quay lại cuối hàng đợi
            if (p[i].remaining_time > 0) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        // Nếu hàng đợi rỗng nhưng vẫn còn tiến trình => bỏ qua thời gian
        if (front == rear && completed < n) {
            for (int j = 0; j < n; j++) {
                if (p[j].arrival_time > time) {
                    time = p[j].arrival_time;
                    queue[rear++] = j;
                    in_queue[j] = 1;
                    break;
                }
            }
        }
    }

    // In thời gian
    x = 2;
    y += 1;
    mvprintw(y, x - 1, "0");
    time = 0;
    for (int i = 0; i < rear; i++) {
        time += (p[queue[i]].remaining_time == 0 ? 0 : (p[queue[i]].burst_time >= TIME_QUANTUM ? TIME_QUANTUM : p[queue[i]].burst_time));
        x += TIME_QUANTUM * 5;
        mvprintw(y, x - 2, "%d", time);
    }

    y += 2;
    attron(A_BOLD);
    mvprintw(y, 2, "✅ Round Robin hoan thanh! Nhan phim bat ky de thoat...");
    attroff(A_BOLD);
    refresh();
    getch();
    endwin();

    return 0;
}
