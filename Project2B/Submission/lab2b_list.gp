# NAME: Zhengyuan Liu
# EMAIL: zhengyuanliu@ucla.edu

# 
#! /usr/bin/gnuplot
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2b_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#   8. average wait-for-lock time (ns) 
#
# output:
#	lab2b_1.png ... throughput vs. number of threads for mutex and spin-lock
#	lab2b_2.png ... average wait-for-lock time and time per operation vs number of competing threads
#	lab2b_3.png ... threads and iterations that run (protected) w/o failure
#	lab2b_4.png ... aggregated throughput vs. the number of threads for mutex
#	lab2b_5.png ... aggregated throughput vs. the number of threads for spin-lock
#

# general plot parameters
set terminal png
set datafile separator ","

# Plot lab2b_1.png
set title "Lab2b-1: Number of operations per second"
set xlabel "Number of threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Number of operations per second"
set logscale y
set output 'lab2b_1.png'
set key left top
plot \
     "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list w/mutex' with linespoints lc rgb 'blue', \
     "< grep 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list w/spin-lock' with linespoints lc rgb 'green'

# Plot lab2b_2.png
set title "Lab2b-2: Average wait-for-lock time and time per operation for list w/mutex"
set xlabel "Number of threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Average wait-for-lock time and average time per operation"
set logscale y 10
set output 'lab2b_2.png'
set key left top
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
	title 'wait-for-lock time' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
	title 'time per operation' with linespoints lc rgb 'green'

# Plot lab2b_3.png     
set title "Lab2b-3: Successful Iterations that run without failure"
unset logscale x
set xrange [0.75:]
set xlabel "Number of threads"
set logscale x 2
set ylabel "Successful iterations"
set logscale y 10
set output 'lab2b_3.png'
plot \
    "< grep 'list-id-none,' lab2b_list.csv" using ($2):($3) \
	with points lc rgb "green" title "Unprotected", \
    "< grep 'list-id-m,' lab2b_list.csv" using ($2):($3) \
	with points lc rgb "red" title "Mutex", \
    "< grep 'list-id-s,' lab2b_list.csv" using ($2):($3) \
	with points lc rgb "blue" title "Spin-Lock"
#
# "no valid points" is possible if even a single iteration can't run
#

# Plot lab2b_4.png
set title "Lab2b-4: Aggregated throughput vs number of threads for mutex"
set xlabel "Number of threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Number of operations per second"
set logscale y
set output 'lab2b_4.png'
set key left top
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list = 1' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list = 4' with linespoints lc rgb 'green', \
	 "< grep -e 'list-none-m,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list = 8' with linespoints lc rgb 'red', \
	 "< grep -e 'list-none-m,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list = 16' with linespoints lc rgb 'yellow'

# Plot lab2b_5.png
set title "Lab2b-5: Aggregated throughput vs number of threads for spin-lock"
set xlabel "Number of threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Number of operations per second"
set logscale y
set output 'lab2b_5.png'
set key left top
plot \
     "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list = 1' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-s,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list = 4' with linespoints lc rgb 'green', \
	 "< grep -e 'list-none-s,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list = 8' with linespoints lc rgb 'red', \
	 "< grep -e 'list-none-s,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
	title 'list = 16' with linespoints lc rgb 'yellow'