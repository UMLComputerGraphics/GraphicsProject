#!/bin/bash

grep -o -E '/([^/:]*\.[hc][^: ]*):' warnings.log | sort | uniq -c


