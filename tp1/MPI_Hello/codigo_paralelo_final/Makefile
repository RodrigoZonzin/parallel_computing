all: 
	mpicc main.c Grafo.c Grafo.h -o main


run: 
	mpirun -np 2 ./main arquivo_teste.txt

clean: 
	rm *.cng main
