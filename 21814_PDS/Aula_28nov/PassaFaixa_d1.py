import numpy
import matplotlib.pyplot as plt
import numpy as np
from scipy import signal as sf

#-----------------------------------
# Input data filter requirements
#-----------------------------------
fs              = 8000
fc              = 1209 # 697 770 852 941 - 1209 1336 1477
fb              = 40 
#
cnum = (np.tan(np.pi * (fb/fs)))-1
cdem = (np.tan(2*np.pi * (fb/fs)))+1
c = cnum / cdem
#
d = -np.cos(2*np.pi*(fc/fs))
xnm1 = 0
xnm2 = 0
ynm1 = 0
ynm2 = 0

#Sweep40_3400Hz
entrada = numpy.memmap("21814_PDS/Aula_28nov/Sweep40_3400Hz.pcm", dtype='h', mode='r')
saida = np.memmap("21814_PDS/Aula_28nov/saida_1209hz.pcm", dtype='h', mode='w+', shape=entrada.shape)

for i in range(entrada.size):
    yf = -c * entrada[i] + d*(1-c) * xnm1 + xnm2 - d*(1-c)*ynm1 + c*ynm2
    ynm2=ynm1
    ynm1=yf
    xnm2=xnm1
    xnm1 = entrada[i]
    saida[i]=0.5*(entrada[i] - yf)

#plt.plot(saida, 'b')
#plt.grid()