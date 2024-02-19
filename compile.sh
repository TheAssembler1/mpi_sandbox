mpicc -o example_1.out hello_world/example_1.c
mpicc -o example_2.out point_to_point/example_2.c
mpicc -o example_3.out point_to_point/example_3.c
mpicc -o example_4.out point_to_point/example_4.c
mpicc -o example_5.out dynamic_point_to_point/example_5.c
mpicc -o example_6.out dynamic_point_to_point/example_6.c
mpic++ -o example_7.out application/example_7.cpp
mpic++ -o example_8.out collective_broadcasting/example_8.c

echo "%% ========================================================== %%"
echo "%% [example_1] ============================================== %%"
echo "%% ========================================================== %%"
mpirun -np 12 --use-hwthread-cpus example_1.out
echo "%% ========================================================== %%"
echo "%% [example_2] ============================================== %%"
echo "%% ========================================================== %%"
mpirun -np 12 --use-hwthread-cpus example_2.out
echo "%% ========================================================== %%"
echo "%% [example_3] ============================================== %%"
echo "%% ========================================================== %%"
mpirun -np 12 --use-hwthread-cpus example_3.out
echo "%% ========================================================== %%"
echo "%% [example_4] ============================================== %%"
echo "%% ========================================================== %%"
mpirun -np 12 --use-hwthread-cpus example_4.out
echo "%% ========================================================== %%"
echo "%% [example_5] ============================================== %%"
echo "%% ========================================================== %%"
mpirun -np 12 --use-hwthread-cpus example_5.out
echo "%% ========================================================== %%"
echo "%% [example_6] ============================================== %%"
echo "%% ========================================================== %%"
mpirun -np 12 --use-hwthread-cpus example_6.out
echo "%% ========================================================== %%"
echo "%% [example_7] ============================================== %%"
echo "%% ========================================================== %%"
mpirun -np 12 --use-hwthread-cpus example_7.out
echo "%% ========================================================== %%"
echo "%% [example_8] ============================================== %%"
echo "%% ========================================================== %%"
mpirun -np 12 --use-hwthread-cpus example_8.out

rm -rf *.out