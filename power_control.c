#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nguyen Hoang Tan");
MODULE_DESCRIPTION("Kernel module to adjust CPU frequency based on battery level");
MODULE_VERSION("1.0");

#define BATTERY_PATH "/sys/class/power_supply/BAT0/capacity"
#define CPU_FREQ_PATH "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"

// Đọc mức pin từ file
static int read_battery_capacity(void)
{
    struct file *file;
    char buf[4] = {0};
    int percent = 100;
    loff_t pos = 0;

    file = filp_open(BATTERY_PATH, O_RDONLY, 0);
    if (IS_ERR(file)) {
        pr_err("Không thể mở %s\n", BATTERY_PATH);
        return percent;
    }

    kernel_read(file, buf, sizeof(buf) - 1, &pos);
    filp_close(file, NULL);

    if (kstrtoint(buf, 10, &percent) != 0) {
        pr_err("Lỗi chuyển đổi dung lượng pin sang số nguyên\n");
        percent = 100;
    }

    return percent;
}

// Ghi tần số CPU vào file
static void set_cpu_frequency(int freq)
{
    struct file *file;
    char freq_str[16];
    loff_t pos = 0;

    snprintf(freq_str, sizeof(freq_str), "%d\n", freq);

    file = filp_open(CPU_FREQ_PATH, O_WRONLY, 0);
    if (IS_ERR(file)) {
        pr_err("Không thể mở %s\n", CPU_FREQ_PATH);
        return;
    }

    kernel_write(file, freq_str, strlen(freq_str), &pos);
    filp_close(file, NULL);
}

// Luồng kiểm tra pin định kỳ
static int monitor_fn(void *data)
{
    while (!kthread_should_stop()) {
        int battery = read_battery_capacity();
        pr_info("🔋 Mức pin hiện tại: %d%%\n", battery);

        if (battery < 30) {
            set_cpu_frequency(600000);  // 600MHz
            pr_info("⚠️ Pin thấp - Hạ xung CPU\n");
        } else {
            set_cpu_frequency(1500000); // 1.5GHz
            pr_info("✅ Pin đủ - Tăng xung CPU\n");
        }

        ssleep(10);
    }
    return 0;
}

static struct task_struct *monitor_thread;

static int __init power_control_init(void)
{
    pr_info("🚀 Nạp module power_control\n");
    monitor_thread = kthread_run(monitor_fn, NULL, "battery_monitor_thread");
    if (IS_ERR(monitor_thread)) {
        pr_err("❌ Không thể tạo luồng kiểm tra\n");
        return PTR_ERR(monitor_thread);
    }
    return 0;
}

static void __exit power_control_exit(void)
{
    if (monitor_thread)
        kthread_stop(monitor_thread);
    pr_info("🧹 Gỡ module power_control\n");
}

module_init(power_control_init);
module_exit(power_control_exit);
