#!/bin/bash

echo "[*] Chạy power_monitor.py..."
./monitor/power_monitor.py &

sleep 1

echo "[*] Chạy battery_monitor.sh..."
./monitor/battery_monitor.sh &

sleep 1

echo "[*] Nạp kernel module power_control.ko..."
sudo insmod power_control.ko
