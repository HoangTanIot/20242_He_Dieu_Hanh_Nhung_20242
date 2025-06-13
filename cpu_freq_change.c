#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/file.h>
#include <linux/string.h>


#define CPU_FREQ_PATH "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"
#define LOW_FREQ "1500000\n" // Thay doi theo xung nhip can thay doi

static char original_freq[16] = {0};
static int original_freq_set = 0;

// Hàm ghi tần số CPU
static void write_cpu_freq(const char *freq) {
    struct file *file;
    loff_t pos = 0;

    file = filp_open(CPU_FREQ_PATH, O_WRONLY, 0);
    if (!IS_ERR(file)) {
        kernel_write(file, freq, strlen(freq), &pos);
        filp_close(file, NULL);
    } else {
        pr_err("Không ghi được tần số CPU\n");
    }
}

// Hàm đọc tần số CPU hiện tại
static void read_original_freq(void) {
    struct file *file;
    loff_t pos = 0;

    file = filp_open(CPU_FREQ_PATH, O_RDONLY, 0);
    if (!IS_ERR(file)) {
        kernel_read(file, original_freq, sizeof(original_freq) - 1, &pos);
        original_freq_set = 1;
        filp_close(file, NULL);
    } else {
        pr_err("Không đọc được tần số CPU hiện tại\n");
    }
}

static int __init cpu_module_init(void) {
    pr_info("Tải module giảm xung CPU\n");

    read_original_freq();
    write_cpu_freq(LOW_FREQ);
    pr_info("Tần số CPU đã được giảm xuống %s", LOW_FREQ);

    return 0;
}

static void __exit cpu_module_exit(void) {
    if (original_freq_set) {
        write_cpu_freq(original_freq);
        pr_info("Tần số CPU đã khôi phục về %s", original_freq);
    }

    pr_info("Gỡ module giảm xung CPU\n");
}

module_init(cpu_module_init);
module_exit(cpu_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bạn");
MODULE_DESCRIPTION("Module kernel đơn giản để giảm xung nhịp CPU và khôi phục lại khi gỡ module");
