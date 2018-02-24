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

i = 1
while i < len(times):
    if times[i] < times[i-1]:
        del times[i]
        del rssis[i]
    else:
        i += 1

#i = 0
#c = 0
#d = 1000
#while i < len(times):
#    if times[i] > c + d:
#        c = times[i]
#        print(c)
#    i += 1

#rssis = rssis[10:588]
#times = times[10:588]
#print(len(times), len(rssis))
#plt.plot(rssis)
#plt.plot(times)
plt.plot(times, rssis, 'ro-')
plt.title('RSSI')
plt.xlabel('time [ms]')
plt.ylabel('RSSI [dBm]')
plt.show()
