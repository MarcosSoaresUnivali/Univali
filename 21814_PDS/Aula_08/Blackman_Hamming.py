import numpy 
import matplotlib.pyplot as plt
from scipy import signal as sf

#data = numpy.memmap("degrauun.pcm", dtype='h', mode='r')

M = 100
passo = (numpy.pi/1000)
p = numpy.arange(0, numpy.pi, passo)
k = numpy.arange(0, numpy.pi, passo)
w = numpy.zeros(M)

w_hamming = numpy.zeros(M)

for pointer in range(len(w)):
     w[pointer] = 0.42 - (0.5 * (numpy.cos((2*numpy.pi*pointer)/M))) + (0.08 * (numpy.cos((4*numpy.pi*pointer)/M)))  


for i in range(len(w_hamming)):
     w_hamming[i] = 0.54 - (0.46 * (numpy.cos((2*numpy.pi*i)/M))) 

plt.subplot(2, 2, 1)
plt.title("Hamming")
plt.plot(w_hamming, 'b')
plt.grid()

plt.subplot(2, 2, 2)
plt.title("Blackman")
plt.plot(w, 'r')
plt.grid()

plt.subplot(2, 2, 3)
plt.title("Hamming vs Blackman")
plt.plot(w_hamming, 'b',label='Hamming')
plt.plot(w, 'r',label='Blackman')
plt.legend(loc='upper center', bbox_to_anchor=(0.5, 0.05), shadow=True, ncol=2)
plt.grid()


plt.tight_layout()
plt.show()

[p, w] = sf.freqz(w, 1,8000)
[k, w_hamming] = sf.freqz(w_hamming, 1,8000)

plt.subplot(2, 2, 1)
plt.title("Magnitude da resposta em frequencia Blackman")
plt.plot(p, abs(w), 'r', label='Blackman')
plt.grid()

plt.subplot(2, 2, 2)
plt.title('Magnitude da resposta em frequencia Hamming')
plt.plot(k, abs(w_hamming), 'b', label ='Hamming')
plt.grid()

plt.subplot(2, 2, 3)
plt.title('Magnitude da resposta em frequencia Hamming vs Blackman')
plt.plot(k, abs(w_hamming), 'b', label ='Hamming')
plt.plot(p, abs(w), 'r', label='Blackman')
plt.legend(loc='upper center', bbox_to_anchor=(0.5, 0.05), shadow=True, ncol=2)
plt.grid()

plt.tight_layout()
plt.show()