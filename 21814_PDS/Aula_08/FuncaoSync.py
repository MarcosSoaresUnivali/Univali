# -----------------------------------------------------------------------------
#  Função Sync
# -----------------------------------------------------------------------------
# Autor: Marcos Paulo Soares [Eng. Computacao]
#------------------------------------------------------------------------------
import numpy 
import matplotlib.pyplot as plt
from scipy import signal as sf

M = 10
Fs = 8000

passo = (numpy.pi/1000)
Fc = 0.1
w = numpy.arange(0, numpy.pi, passo)
saida = numpy.zeros(M)
x = numpy.arange(-M/2, M/2, 1)


for i in range(M):
    if(i == M/2):
        saida[i] = 1
    if(i < M/2):
        saida[i] = (numpy.sin(2 * numpy.pi * Fc * (-i+M/2))) / ((-i+M/2) * numpy.pi)
    elif(i > M/2):
        saida[i] = (numpy.sin(2 * numpy.pi * Fc * (i-M/2))) / ((i-M/2) * numpy.pi)

plt.plot(x,saida,'r')
plt.grid()
plt.show()


[w, saida] = sf.freqz(saida, 1,Fs)
plt.plot(w, abs(saida))
plt.title('Magnitude da resposta em frequencia')
plt.grid()
plt.tight_layout()
plt.show()
