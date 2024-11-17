f = open('arquivo_teste.txt', 'r')

E = f.read()
 
vet = [E[0]]
for e in E: 
    for v in vet: 
        print(e, v)
        if e != v:
            vet.append(e)

print(vet)