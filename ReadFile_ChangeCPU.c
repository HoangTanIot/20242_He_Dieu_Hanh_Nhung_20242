#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/file.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define PIN_SCRIPT_DESC "Pin_20000mAh.sh" // File setup pin 20000mAh
#define BATT_LEVEL_FILE "/tmp/fake_batt_level" //Gia tri pin sau khi doc tu 'Pin_20000mAh' se luu o day
#define CPU_FREQ_PATH "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq" //Thay doi CPU vao file nay
#define MAX_CPU_FREQ   "2500000\n" //25MHz
#define MIN_CPU_FREQ   "1500000\n" //5MHz

static struct task_struct *monitor_thread;
static char original_freq[16] = {0};
static int has_original = 0;

// Tính toán xung nhịp theo % pin
static const char *calculate_freq(int batt_level) {
    if (batt_level > 30) return MAX_CPU_FREQ;  // 1.5GHz
    // if (batt_level >= 60) return "2000000\n";  // 1.2GHz
    // if (batt_level >= 40) return "1800000\n";  // 1.0GHz
    if (batt_level <= 30) return MIN_CPU_FREQ;   // 800MHz
    // return "600000\n";                         // Pin thấp, giảm sâu
}

static int read_battery_level(void) {
    struct file *file;
    char buf[16] = {0};
    int level = 100;
    loff_t pos = 0;

    file = filp_open(BATT_LEVEL_FILE, O_RDONLY, 0);
    if (!IS_ERR(file)) {
        kernel_read(file, buf, sizeof(buf) - 1, &pos);
        kstrtoint(buf, 10, &level);
        filp_close(file, NULL);
    } else {
        pr_err("Khong doc duoc file mo phong: %s\n", BATT_LEVEL_FILE);
    }

    return level;
}

static void write_cpu_freq(const char *freq) {
    struct file *file;
    loff_t pos = 0;

    file = filp_open(CPU_FREQ_PATH, O_WRONLY, 0);
    if (!IS_ERR(file)) {
        kernel_write(file, freq, strlen(freq), &pos);
        filp_close(file, NULL);
    } else {
        pr_err("Khong ghi duoc tan so vao CPU: %s\n", CPU_FREQ_PATH);
    }
}

static int monitor_fn(void *data) {
    while (!kthread_should_stop()) {
        int level = read_battery_level();
        const char *freq = calculate_freq(level);

        pr_info("Muc pin%d%% -> Cai xung CPU: %s", level, freq);
        write_cpu_freq(freq);

        ssleep(60);  // mỗi phút kiểm tra
    }
    return 0;
}

static int __init battery_control_init(void) {
    struct file *file;
    loff_t pos = 0;

    file = filp_open(CPU_FREQ_PATH, O_RDONLY, 0);
    if (!IS_ERR(file)) {
        kernel_read(file, original_freq, sizeof(original_freq) - 1, &pos);
        has_original = 1;
        filp_close(file, NULL);
    }

    monitor_thread = kthread_run(monitor_fn, NULL, "battery_cpu_monitor");
    if (IS_ERR(monitor_thread)) {
        pr_err("Khong the tao luong giam sat pin\n");
        return PTR_ERR(monitor_thread);
    }

    pr_info("Kernel module theo doi pin tu %s da khoi dong\n", PIN_SCRIPT_DESC);
    return 0;
}

static void __exit battery_control_exit(void) {
    if (monitor_thread)
        kthread_stop(monitor_thread);

    if (has_original)
        write_cpu_freq(original_freq);

    pr_info("Kernel module da dung, khoi phuc tan so ban dau\n");
}

module_init(battery_control_init);
module_exit(battery_control_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LuongHuuPhuc");
MODULE_DESCRIPTION("Kernel module dieu chinh CPU theo muc pin mo phong tu Pin_20000mAh.sh");
