#include <ncurses.h>
#include <unistd.h>  // usleep
#include <string.h>

struct Process {
    char id[5];
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
};

int main() {
    initscr();
    start_color();
    curs_set(0);
    noecho();

    // Tạo các cặp màu động
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);

    struct Process p[3] = {
        {"P1", 0, 3},
        {"P2", 1, 2},
        {"P3", 2, 4}
    };

    // Sắp xếp FCFS
    for (int i = 0; i < 2; i++)
        for (int j = i + 1; j < 3; j++)
            if (p[i].arrival_time > p[j].arrival_time) {
                struct Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }

    int time = 0;
    int x = 2, y = 2;

    for (int i = 0; i < 3; i++) {
        if (time < p[i].arrival_time) time = p[i].arrival_time;
        p[i].start_time = time;
        p[i].completion_time = time + p[i].burst_time;

        beep(); // 🔔 âm thanh khi bắt đầu tiến trình
        attron(COLOR_PAIR(i + 1));
        for (int j = 0; j < p[i].burst_time; j++) {
            for (int k = 0; k < 5; k++) {
                mvprintw(y, x + j * 5 + k, " ");
            }
            mvprintw(y, x + j * 5 + 1, "%s", p[i].id);  // In tiến trình
            refresh();
            usleep(500000);  // 0.5s mỗi đơn vị thời gian
        }
        attroff(COLOR_PAIR(i + 1));

        time = p[i].completion_time;
        x += p[i].burst_time * 5;
    }

    // In mốc thời gian
    x = 2;
    y += 1;
    mvprintw(y, x - 1, "%d", p[0].start_time);
    for (int i = 0; i < 3; i++) {
        x += p[i].burst_time * 5;
        mvprintw(y, x - 2, "%d", p[i].completion_time);
    }

    // Kết thúc
    y += 2;
    attron(A_BOLD);
    mvprintw(y, 2, "✅ Nhan phim bat ky de thoat...");
    attroff(A_BOLD);
    refresh();
    getch();
    endwin();
    return 0;
}
