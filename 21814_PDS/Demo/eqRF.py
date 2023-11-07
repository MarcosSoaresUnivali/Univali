import numpy
import matplotlib.pyplot as plt
import numpy as np
from scipy import signal as sf

# 'LOW-PASS WINDOWED-SINC FILTER
# 'This program filters 5000 samples with a 101 point windowed-sinc filter,
# 'resulting in 4900 samples of filtered data.
# '

dim_x = numpy.zeros(4999)
dim_y = numpy.zeros(4999)
dim_h = numpy.zeros(100)
dim_h1 = numpy.zeros(100)

Fs = 8000
Fchz = 1000
Fc = Fchz / Fs

Fchz1 = 3000
Fc1 = Fchz1 / Fs
M = 100

for i in range(M):
    if (i - M / 2) == 0:
        dim_h[i] = 2*np.pi*Fc
    else:
        dim_h[i] = np.sin((2*np.pi*Fc)*(i-M/2))/(i-M/2)#sobre 0 ia dar erro

for i in range(M):
    if (i - M / 2) == 0:
        dim_h1[i] = 2*np.pi*Fc1
    else:
        dim_h1[i] = np.sin((2*np.pi*Fc1)*(i-M/2))/(i-M/2)#sobre 0 ia dar erro

data = numpy.memmap("Sweep.pcm", dtype='h', mode='r') # convolui sweep com pobre

opc = input("Hamming ou Blackman? \nHamming = 1 | Blackman = 2\n")
# print(opc)
opc = int(opc)

if(opc == 1):

    # h

    for i in range(M):
        dim_h[i] = dim_h[i] * (0.54 - 0.46 * np.cos(2 * np.pi * i / M))  # aplica o hamming para poder acertar o filtro

    soma = 0
    for i in range(M):
        soma = soma + dim_h[i]

    for i in range(M):
        dim_h[i] = dim_h[i] / soma

    # h1

    for i in range(M):
        dim_h1[i] = dim_h1[i] * (0.54 - 0.46 * np.cos(2 * np.pi * i / M))  # aplica o hamming para poder acertar o filtro

    soma1 = 0
    for i in range(M):
        soma1 = soma + dim_h1[i]

    for i in range(M):
        dim_h1[i] = dim_h1[i] / soma1

    for i in range(M):
        dim_h1[i] = dim_h1[i] * -1
    dim_h1[int(M / 2)] = dim_h1[int(M / 2)] + 1

elif (opc == 2):

    for i in range(M):
        dim_h[i] = dim_h[i] * (0.42 - 0.5 * np.cos(2 * np.pi * i / M) + 0.08 * np.cos(4 * np.pi * i / M))  # aplica o blackman

    soma = 0
    for i in range(M):
        soma = soma + dim_h[i]

    for i in range(M):
        dim_h[i] = dim_h[i] / soma

    for i in range(M):
        dim_h1[i] = dim_h[i] * -1
    dim_h1[int(M / 2)] = dim_h1[int(M / 2)] + 1


#--------------------------------------------------------------------------------------------------------
# PASSA FAIXA

dim_h2 = numpy.zeros(100)

[w, H] = sf.freqz(dim_h, fs=Fs)
freq = w
plt.plot(freq, abs(H))
plt.title('Magnitude da resposta em frequencia, Passa Baixa')
plt.show()

[w, H] = sf.freqz(dim_h1, fs=Fs)
freq = w
plt.plot(freq, abs(H))
plt.title('Magnitude da resposta em frequencia, Passa Alta')
plt.show()

for i in range(M):
    dim_h2[i] = dim_h[i] + dim_h1[i]

with open("Coeficiente_RF.dat", "w") as f:
    for s in dim_h2:
        f.write(str(s) +",\n")

plt.stem(dim_h2)  # sem riple e acerta mag
plt.title('Rejeita Faixa | (PB + PA)')
plt.show()

[w, H] = sf.freqz(dim_h2, fs=Fs)
freq = w
plt.plot(freq, abs(H))
plt.title('Magnitude da resposta em frequencia, Passa Faixa')
plt.show()

res_conv = np.convolve(dim_h2, data)

plt.plot(res_conv)
plt.title('Conv. com Sweep, Passa Faixa')
plt.show()

