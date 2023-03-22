#!/bin/bash

sudo cpufreq-set --cpu 0 --governor performance
sudo cpufreq-set --cpu 1 --governor performance
sudo cpufreq-set --cpu 2 --governor performance
sudo cpufreq-set --cpu 3 --governor performance
sudo cpufreq-set --cpu 4 --governor performance
sudo cpufreq-set --cpu 5 --governor performance
sudo cpufreq-set --cpu 6 --governor performance
sudo cpufreq-set --cpu 7 --governor performance
sudo cpufreq-set --cpu 8 --governor performance
sudo cpufreq-set --cpu 9 --governor performance
sudo cpufreq-set --cpu 10 --governor performance
sudo cpufreq-set --cpu 11 --governor performance

cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor