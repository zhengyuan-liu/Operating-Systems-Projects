# lab2_list-1.png: cost per operation vs the number of iterations
# a single thread, and increasing numbers of iterations (10, 100, 1000, 10000, 20000)
./lab2_list --threads=1 --iterations=10 >> lab2_list.csv
./lab2_list --threads=1 --iterations=100 >> lab2_list.csv
./lab2_list --threads=1 --iterations=1000 >> lab2_list.csv
./lab2_list --threads=1 --iterations=10000 >> lab2_list.csv
./lab2_list --threads=1 --iterations=20000 >> lab2_list.csv


# lab2_list-2.png: successful runs vs the number of threads and iterations for non-yield and yield
# threads=(2,4,8,12), iterations=(1, 10, 100, 1000), non-yield
./lab2_list --threads=2 --iterations=1 >> lab2_list.csv
./lab2_list --threads=2 --iterations=10 >> lab2_list.csv
./lab2_list --threads=2 --iterations=100 >> lab2_list.csv
./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv

./lab2_list --threads=4 --iterations=1 >> lab2_list.csv
./lab2_list --threads=4 --iterations=10 >> lab2_list.csv
./lab2_list --threads=4 --iterations=100 >> lab2_list.csv
./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv

./lab2_list --threads=8 --iterations=1 >> lab2_list.csv
./lab2_list --threads=8 --iterations=10 >> lab2_list.csv
./lab2_list --threads=8 --iterations=100 >> lab2_list.csv
./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv

./lab2_list --threads=12 --iterations=1 >> lab2_list.csv
./lab2_list --threads=12 --iterations=10 >> lab2_list.csv
./lab2_list --threads=12 --iterations=100 >> lab2_list.csv
./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv

# threads=(2,4,8,12), iterations=(1,2,4,8,16,32), yield=(i, d, il, dl)
./lab2_list --threads=2 --iterations=1 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=i >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=i >> lab2_list.csv

./lab2_list --threads=2 --iterations=1 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=d >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=d >> lab2_list.csv

./lab2_list --threads=2 --iterations=1 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=il >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=il >> lab2_list.csv

./lab2_list --threads=2 --iterations=1 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=2 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=4 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=8 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=16 --yield=dl >> lab2_list.csv
./lab2_list --threads=2 --iterations=32 --yield=dl >> lab2_list.csv

./lab2_list --threads=4 --iterations=1 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=i >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=i >> lab2_list.csv

./lab2_list --threads=4 --iterations=1 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=d >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=d >> lab2_list.csv

./lab2_list --threads=4 --iterations=1 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=il >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=il >> lab2_list.csv

./lab2_list --threads=4 --iterations=1 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=2 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=4 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=8 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=16 --yield=dl >> lab2_list.csv
./lab2_list --threads=4 --iterations=32 --yield=dl >> lab2_list.csv

./lab2_list --threads=8 --iterations=1 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=i >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=i >> lab2_list.csv

./lab2_list --threads=8 --iterations=1 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=d >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=d >> lab2_list.csv

./lab2_list --threads=8 --iterations=1 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=il >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=il >> lab2_list.csv

./lab2_list --threads=8 --iterations=1 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=2 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=4 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=8 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=16 --yield=dl >> lab2_list.csv
./lab2_list --threads=8 --iterations=32 --yield=dl >> lab2_list.csv

./lab2_list --threads=12 --iterations=1 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=i >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=i >> lab2_list.csv

./lab2_list --threads=12 --iterations=1 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=d >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=d >> lab2_list.csv

./lab2_list --threads=12 --iterations=1 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=il >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=il >> lab2_list.csv

./lab2_list --threads=12 --iterations=1 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=2 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=4 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=8 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=16 --yield=dl >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=dl >> lab2_list.csv


# lab2_list-3.png: successful runs vs the number of iterations for mutex and spin-lock protection with four yield combinations
./lab2_list --threads=12 --iterations=32 --yield=i --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=d --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=i --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=d --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab2_list.csv


# lab2_list-4.png: per operation times vs the number of threads
# threads (1, 2, 4, 8, 12, 16, 24), iterations (1000), without yield, sync options: mutex, spin
./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=4 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=8 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv

./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=4 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=8 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2_list.csv
./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv
