#!/usr/bin/env bash
log_file="$1"
cat ${log_file} \
    | egrep '^\[0\]' \
    | awk '{print $3,$4}' \
    | sed 's/ (/,/g' > `echo ${log_file} | sed 's/\.log//g'`_formated.log
