#!/bin/bash

# Khởi tạo dung lượng pin
battery_capacity=20000  # mAh
voltage=5.0             # V
interval=5              # giây
remaining_capacity=$battery_capacity

while true; do
    # Đọc mức tiêu thụ từ file
    if [[ -f /tmp/power_usage.txt ]]; then
        power_mw=$(cat /tmp/power_usage.txt) # Doc so cong suat tu file
    else
        power_mw=0
    fi

    # Tính năng lượng tiêu thụ trong khoảng thời gian $interval
    energy_mWh=$(echo "$power_mw * $interval / 3600" | bc -l)
    energy_mAh=$(echo "$energy_mWh / $voltage" | bc -l)

    # Trừ lượng tiêu thụ khỏi dung lượng pin còn lại
    remaining_capacity=$(echo "$remaining_capacity - $energy_mAh" | bc -l)
    
    # Không để âm
    if (( $(echo "$remaining_capacity < 0" | bc -l) )); then
        remaining_capacity=0
    fi

    # Tính phần trăm còn lại
    percent=$(echo "$remaining_capacity * 100 / $battery_capacity" | bc -l)

    # Làm tròn số
    percent_int=$(printf "%.0f" $percent)
    remaining_capacity_int=$(printf "%.0f" $remaining_capacity)

    echo "$percent_int" > /tmp/fake_batt_level # Ket qua % pin se duoc luu vao trong file nay

    echo "$(date): Còn ${percent_int}% pin (~${remaining_capacity_int} mAh)"
    sleep $interval
done