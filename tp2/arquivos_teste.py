import random 
import sys

n = int(sys.argv[1])
m = int(sys.argv[2])

fout = open(f'arquivo_teste_n{n}.txt', 'w')

for i in range(0, n):
    fout.write(f'{str(random.randint(0, m))} ')

fout.close()