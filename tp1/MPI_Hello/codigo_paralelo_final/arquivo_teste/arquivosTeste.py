import numpy.random as npr
import sys

MAX = int(sys.argv[1])
MAX_AMOSTRA = int(sys.argv[2])
arq = open(f"arquivo_teste_s{MAX_AMOSTRA}.txt", 'w')


u = npr.random_integers(low = 0, high = MAX, size=MAX_AMOSTRA)

for v in u:
    arq.write(f"{str(v)} ")

arq.close()