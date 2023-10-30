import numpy
import matplotlib.pyplot as plt
from scipy import signal as sf

# data = numpy.memmap("degrauun.pcm", dtype='h', mode='r')

M = 500
K = 1
Fs = 8000
# Fchz = 2000
# Fc = Fchz / Fs
Fc = 0.015
x = M / 2
h = numpy.zeros(M)

for i in range(len(h)):
    if ((i-x)==0):
        h[i] = (2 * numpy.pi * Fc) * K
    else:
        h[i] = K * ((numpy.sin(2 * numpy.pi * Fc * (i - x))) / (i - x)) * (
                    (0.42 - 0.5 * numpy.cos((2 * numpy.pi * i) / M)) + 0.08 * numpy.cos((4 * numpy.pi * i) / M))

plt.plot(h, 'b')
plt.grid()

plt.tight_layout()
plt.show()

[w, h] = sf.freqz(h, fs = Fs)
freq = w
plt.plot(freq, 20*numpy.log10(abs(h)))
plt.title('Magnitude da resposta em frequencia')
plt.grid()
plt.tight_layout()
plt.show()

with open('16-4.pcm', 'wb') as fid:
    numpy.array(h, dtype=numpy.int16).tofile(fid)
fid.close()