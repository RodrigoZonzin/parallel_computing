mpicc junto.c -o junto

time mpirun np -1 ./junto arquivo_teste.txt

time mpirun np -2 ./junto arquivo_teste.txt

time mpirun np -3 ./junto arquivo_teste.txt

time mpirun np -4 ./junto arquivo_teste.txt
