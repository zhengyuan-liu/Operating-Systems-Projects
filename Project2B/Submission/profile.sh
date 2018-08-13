rm -f ./raw.tmp profile.out
LD_PRELOAD=$HOME/lib/libprofiler.so.0 CPUPROFILE=./raw.tmp ./lab2_list --threads=12 --iterations=1000 --sync=s
pprof --text ./lab2_list ./raw.tmp > profile.out
pprof --list=list_operation_spin_lock ./lab2_list ./raw.tmp >> profile.out
rm -f ./raw.tmp
