mpicc -o example_1.out example_1.c
mpicc -o example_2.out example_2.c
mpicc -o example_3.out example_3.c
mpicc -o example_4.out example_4.c

echo "example_1=============================================="
mpirun -np 12 --use-hwthread-cpus example_1.out
echo "example_2=============================================="
mpirun -np 12 --use-hwthread-cpus example_2.out
echo "example_3=============================================="
mpirun -np 12 --use-hwthread-cpus example_3.out
echo "example_4=============================================="
mpirun -np 12 --use-hwthread-cpus example_4.out

rm -rf *.out