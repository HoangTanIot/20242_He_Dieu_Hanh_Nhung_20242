#!/usr/bin/env python3
import time
import random

LOG_FILE = "/tmp/power_usage.txt"

# Giả lập giá trị công suất tiêu thụ (bạn có thể thay bằng đo thực nếu có)
def get_power_usage():
    return random.randint(3000, 5000)  # đơn vị: mW

while True:
    power_mw = get_power_usage()
    with open(LOG_FILE, "w") as f:
        f.write(str(power_mw))
    print(f"[PowerMonitor] Tiêu thụ: {power_mw / 1000:.1f}W")
    time.sleep(5)
