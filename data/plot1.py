#!/usr/bin/env python3

import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('LOG_FORMATED')
args = parser.parse_args()

rssis = []
times = []
with open(args.LOG_FORMATED) as f:
    for line in f:
        measurement = line.rstrip().split(',')
        rssi = int(measurement[0])
        time = int(measurement[1])
        rssis.append(rssi)
        times.append(time)

# remove measurements with invalid times
i = 1
while i < len(times):
    if times[i] < times[i-1]:
        del times[i]
        del rssis[i]
    else:
        i += 1

# calc time since last message
diffs = []
mnt = min(times)
mxt = max(times)
ts = range(mnt, mxt+1)
last = times[0]
i = 0
j = 1
while i < len(ts):
    if ts[i] > times[j]:
        last = times[j]
        j += 1
    diffs.append(ts[i] - last)
    i += 1
plt.plot(ts, diffs, 'g-')
plt.title('Time since last message')
plt.xlabel('time [ms]')
plt.ylabel('time since last message [ms]')
plt.show()

plt.plot(times, rssis, 'ro-')
plt.title('RSSI')
plt.xlabel('time [ms]')
plt.ylabel('RSSI [dBm]')
plt.show()
