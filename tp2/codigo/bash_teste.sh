#!/bin/bash

# Primeiro parâmetro fixo
arquivo_entrada="arquivo_teste_n10000.txt"

# Loop para variar o segundo parâmetro de 100 até 200
for ((i=100; i<=2000; i+=200)); do
    time ./main "$arquivo_entrada" "$i"
done
