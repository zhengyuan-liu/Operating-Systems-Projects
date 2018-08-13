# NAME: Zhengyuan Liu
# EMAIL: zhengyuanliu@ucla.edu

# lab2b_1.png: throughput vs. number of threads for mutex and spin-lock synchronized list operations
# Mutex synchronized list operations, 1000 iterations, 1,2,4,8,12,16,24 threads
./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv

# Spin-lock synchronized list operations, 1000 iterations, 1,2,4,8,12,16,24 threads
./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2b_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2b_list.csv


# lab2b_2.png: the wait-for-lock time, and the average time per operation against the number of competing threads
# list mutex test again for 1,000 iterations and 1, 2, 4, 8, 16, 24 threads
# the target tests have done in the previous tests


# lab2b_3.png
# --yield=id, 4 lists, 1,4,8,12,16 threads, and 1, 2, 4, 8, 16 iterations (and no synchronization)
./lab2_list --threads=1 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=1 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=1 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=1 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=1 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv

./lab2_list --threads=4 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=4 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv

./lab2_list --threads=8 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=8 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv

./lab2_list --threads=12 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=12 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv

./lab2_list --threads=16 --iterations=1 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=2 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=4 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=8 --lists=4 --yield=id >> lab2b_list.csv
./lab2_list --threads=16 --iterations=16 --lists=4 --yield=id >> lab2b_list.csv

# --yield=id, 4 lists, 1,4,8,12,16 threads, and 10, 20, 40, 80 iterations, --sync=s and --sync=m
./lab2_list --threads=1 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=1 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=1 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=1 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=1 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=1 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=1 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=1 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv

./lab2_list --threads=4 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv

./lab2_list --threads=8 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv

./lab2_list --threads=12 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv

./lab2_list --threads=16 --iterations=10 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=20 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=40 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=80 --lists=4 --yield=id --sync=m >> lab2b_list.csv
./lab2_list --threads=16 --iterations=10 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=20 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=40 --lists=4 --yield=id --sync=s >> lab2b_list.csv
./lab2_list --threads=16 --iterations=80 --lists=4 --yield=id --sync=s >> lab2b_list.csv

# lab2b_4.png (symc=m) 
# all --lists=1 tests have done in the previous tests
./lab2_list --threads=1 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=4 --sync=m >> lab2b_list.csv

./lab2_list --threads=1 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=8 --sync=m >> lab2b_list.csv

./lab2_list --threads=1 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=16 --sync=m >> lab2b_list.csv

# lab2b_5.png (sync=s)
# all --lists=1 tests have done in the previous tests
./lab2_list --threads=1 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=4 --sync=s >> lab2b_list.csv

./lab2_list --threads=1 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=8 --sync=s >> lab2b_list.csv

./lab2_list --threads=1 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
./lab2_list --threads=2 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
./lab2_list --threads=4 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
./lab2_list --threads=8 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv
./lab2_list --threads=12 --iterations=1000 --lists=16 --sync=s >> lab2b_list.csv