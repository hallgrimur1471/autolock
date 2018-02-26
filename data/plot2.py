#!/usr/bin/env python3

import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('LOG_FORMATED')
args = parser.parse_args()

rssis = []
times = []
locks = []
unlocks = []
with open(args.LOG_FORMATED) as f:
    data = f.readlines()
for i, line in enumerate(data):
    line = line.rstrip()
    if line == "lock":
        if i > 0 and i < len(data)-1 and ',' in data[i-1] and ',' in data[i+1]:
            locks.append(("lock",
                         int(data[i-1].split(',')[0]),
                         int(data[i+1].split(',')[0])))
    elif line == "unlock":
        if i > 0 and i < len(data)-1:
            unlocks.append(("unlock",
                           int(data[i-1].split(',')[0]),
                           int(data[i+1].split(',')[0])))
    else:
        measurement = line.split(',')
        time = int(measurement[0])
        rssi = int(measurement[1])
        times.append(time)
        rssis.append(rssi)

print(len(times), len(rssis), len(locks), len(unlocks))

#plt.plot(ts, diffs, 'g-')
#plt.title('Time since last message')
#plt.xlabel('time [ms]')
#plt.ylabel('time since last message [ms]')
#plt.show()
#
plt.plot(times, rssis, 'bo-', label="RSSI")
plt.title('RSSI')
plt.xlabel('time [ms]')
plt.ylabel('RSSI [dBm]')

# add the locks/unlocks
for lock in locks:
    plt.plot([lock[1], lock[2]], [-45, -45], 'r+-', label="lock")
for unlock in unlocks:
    plt.plot([unlock[1], unlock[2]], [-42, -42], 'g+-', label="unlock")

plt.legend()
plt.show()
