#!bin/bash

# Example: Dua cpu ve che do tiet kien dien
echo "powersave" | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

# Example tat Wifi
nmcli radio wifi off

# Example ngat bluetooth
rfkill block bluetooth 

#log 
echo "Low power mode enabled at $(date)" >> /var/log/power_schedule.log
