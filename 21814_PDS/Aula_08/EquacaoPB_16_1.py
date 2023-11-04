# -----------------------------------------------------------------------------
# 100 'LOW-PASS WINDOWED-SINC FILTER
# 110 'This program filters 5000 samples with a 101 point windowed-sinc filter,
# 120 'resulting in 4900 samples of filtered data.
# -----------------------------------------------------------------------------
# Autor: Marcos Paulo Soares [Eng. Computacao]
#------------------------------------------------------------------------------
import numpy
import matplotlib.pyplot as plt
import numpy as np
from scipy import signal as sf

dim_x = numpy.zeros(4999)           #X[ ] holds the input signal
dim_y = numpy.zeros(4999)           #Y[ ] holds the output signal

Fs = 8000
Fchz = 800
Fc = Fchz / Fs                      # = BW (normalizado)

# --- Faixa trans. BW_hz informada --------------------------------------------
Fthz = 0 # 200

#       Calcular M
#       ---------------------------------
#       M > 4 / BW = 4 / (Fchz / Fs)
#       M = 4 / 2000 / 8000 = M = 16 coef 
# -----------------------------------------------------------------------------
if Fthz > 0:
    M = int(4/(Fc))

# --- M e Faixa de corte informados -------------------------------------------
else:
    Fc = Fchz / Fs                  #Set the cutoff frequency (between 0 and 0.5)
    M = 100                         #Set filter length (101 points)

# --- declara o array do h em relação ao M ------------------------------------
dim_h = numpy.zeros(M)              #H[ ] holds the filter kernel
dim_h1 = numpy.zeros(M)             #H[ ] holds the filter kernel

# --- Inversa IFT - EquacaoSync - Implementado em FuncaoSync.py ---------------

for i in range(M):
    if (i - M / 2) == 0:        #If Erro div 0 
        dim_h[i] = 2 * np.pi * Fc
    else:
        dim_h[i] = np.sin((2 * np.pi * Fc) * (i - M / 2)) / ( i - M / 2) 

plt.stem(dim_h) #plotando filtro pobre
plt.title(f'Filtro pobre | Fc = {Fc} | Ft = Fchz = {Fchz} | M = {M}')
plt.show()

# --- DEBUG -------------------------------------------------------------------
exit()

[w, H] = sf.freqz(dim_h,fs=Fs) # freqz pobre
freq = w
plt.plot(freq, abs(H))
plt.title('Magnitude da resposta em frequencia, Filtro pobre')
plt.show()

data = numpy.memmap("Sweep40_3400Hz.pcm", dtype='h', mode='r') 

# convolui sweep com pobre
res_conv = np.convolve(dim_h,data)
plt.plot(res_conv)
plt.title('Conv. com Sweep, Filtro pobre')
plt.show()

# seleciona a janela
opc = input("Hamming ou Blackman? \nHamming = 1 | Blackman = 2\n")
opc = int(opc)

if(opc == 1):

    for i in range(M):
        dim_h[i] = dim_h[i] * (0.54 - 0.46 * np.cos(2 * np.pi * i / M))  # aplica o hamming para poder acertar o filtro

    plt.stem(dim_h)  # hamming no pobre
    plt.title(f'Hamming no Filtro pobre | Fc = {Fc}')
    plt.show()

    [w, H] = sf.freqz(dim_h, fs=Fs)  # polata freqz
    freq = w
    plt.plot(freq, abs(H))
    plt.title('Magnitude da resposta em frequencia, Hamming Filtro pobre')
    plt.show()

    res_conv = np.convolve(dim_h, data)  # convolui com sweep

    plt.plot(res_conv)
    plt.title('Conv. com Sweep, Hamming Filtro pobre')
    plt.show()

    soma = 0
    for i in range(M):
        soma = soma + dim_h[i]

    for i in range(M):
        dim_h[i] = dim_h[i] / soma

    plt.stem(dim_h)  # normaliza o filtro
    plt.title('Filtro pobre com Hamming Normalizado')
    plt.show()

    [w, H] = sf.freqz(dim_h, fs=Fs)  # freqz normalizado
    freq = w
    plt.plot(freq, abs(H))
    plt.title('Magnitude da resposta em frequencia, Filtro pobre Hamming normalizado')
    plt.show()

    res_conv = np.convolve(dim_h, data)  # conv com sweep normalizado

    plt.plot(res_conv)
    plt.title('Conv. com Sweep, Filtro pobre com Hamming Normalizado')
    plt.show()

elif (opc == 2):

    for i in range(M):
        dim_h[i] = dim_h[i] * (0.42 - 0.5 * np.cos(2 * np.pi * i / M) + 0.08 * np.cos(4 * np.pi * i / M))  # aplica o blackman

    plt.stem(dim_h)  # blackman no pobre
    plt.title(f'Blackman no Filtro pobre | Fc = {Fc}')
    plt.show()

    [w, H] = sf.freqz(dim_h, fs=Fs)  # polata freqz
    freq = w
    plt.plot(freq, abs(H))
    plt.title('Magnitude da resposta em frequencia, Blackman Filtro pobre')
    plt.show()

    res_conv = np.convolve(dim_h, data)  # convolui com sweep

    plt.plot(res_conv)
    plt.title('Conv. com Sweep, Blackman Filtro pobre')
    plt.show()

    soma = 0
    for i in range(M):
        soma = soma + dim_h[i]

    for i in range(M):
        dim_h[i] = dim_h[i] / soma

    plt.stem(dim_h)  # normaliza o filtro
    plt.title('Filtro pobre com Blackman Normalizado')
    plt.show()

    [w, H] = sf.freqz(dim_h, fs=Fs)  # freqz normalizado
    freq = w
    plt.plot(freq, abs(H))
    plt.title('Magnitude da resposta em frequencia, Filtro pobre Blackman normalizado')
    plt.show()

    res_conv = np.convolve(dim_h, data)  # conv com sweep normalizado

    plt.plot(res_conv)
    plt.title('Conv. com Sweep, Filtro pobre com Blackman Normalizado')
    plt.show()

with open("Coeficiente_PB.dat", "w") as f:
    for s in dim_h:
        f.write(str(s) +",\n")
