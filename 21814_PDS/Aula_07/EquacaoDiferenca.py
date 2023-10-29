import numpy
import matplotlib.pyplot as plt

#Projeto do filtro PB RC
# y[n] = a x[n] + a x[n-1] - b y[n-1]
data = numpy.memmap('21814_PDS/Aula_07/sweep.pcm', dtype='h', mode='r')

ynm1 = 0
xnm1 = 0

#calculando os parametros Wc = 2*pi*Fc
Fc = 1000
Fs = 8000
# Fchz = 2000
# Fc = Fchz / Fs

Wc = 2 * numpy.pi * Fc

Fli = 2 * Fs

tamaloop = len(data)
vet_saida =[]

opc = input("PA ou PB? \nPA = 1 | PB = 2\n")
# print(opc)
opc = int(opc)

a = 0
b = 0

if(opc == 2): # PB
    a = Wc / (Fli + Wc)
    b = (Wc - Fli) / (Fli + Wc)
    j=1
    for j in range(tamaloop):
        input = data[j]
        y = a * input + a * xnm1 - b * ynm1
        ynm1 = y
        xnm1 = input
        vet_saida.append(y)

elif(opc == 1): # PA
    a = Fli/(Fli + Wc)
    b = (-Fli + Wc) / (Fli + Wc)
    j = 1
    for j in range(tamaloop):
        input = data[j]
        y = a * input - a * xnm1 - b * ynm1
        ynm1 = y
        xnm1 = input
        vet_saida.append(y)
else:
    print("Escolha um tipo de filtro")

print(f'a: {a}\nb: {b}')


with open('21814_PDS/Aula_07/Transferencia.pcm', 'wb') as fid:
    numpy.array(vet_saida, dtype=numpy.int16).tofile(fid)
fid.close()

plt.plot(data,'b')
plt.plot(vet_saida,'r')
plt.show()
