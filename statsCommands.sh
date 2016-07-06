#ref: http://www.dummies.com/how-to/content/how-to-calculate-percentiles-in-statistics.html

#Average:
cat latency_sort_1.txt | awk '{a+=$1} END{print "Avg: "a/NR}'

#StdDev
cat latency_sort_1.txt | awk '{sum+=$1; sumsq+=$1*$1;} END {print "stdev = " sqrt(sumsq/NR - (sum/NR)**2);}'

#Percentile
cat latency_sort_1.txt | awk 'BEGIN{n=0} {s[n]=$1; sum += $1; n++;} END {print sum/n; print s[int(NR*0.80-0.5)];}'

## cat OrderBookType2CB_Funct_Full.log | grep "Lat" | awk '{print $4}' | sort -n | awk 'BEGIN{n=0} {s[n]=$1; sum += $1; n++;} END {print sum/n; print s[int(NR*0.80-0.5)];}'
