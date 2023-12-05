import numpy as np
import matplotlib.pyplot as plt

# Carregar Coeficientes
with open('Coeficiente_MM_8.dat', 'r') as f:
    coefs= [line.strip().replace(',', '') for line in f]
# with open('Coeficiente_PB.dat', 'r') as f:
#     coefs= [line.strip().replace(',', '') for line in f]

coef = np.zeros(len(coefs))
print(coefs)
for i in range(len(coefs)):
    coef[i] = np.float64(coefs[i].split(','))
print(coef)
# Carregar Entrada
with open('ruido_branco.pcm', 'rb') as fid:
    x = np.fromfile(fid, np.int16)
fid.close()

print(len(x))

u = 0.00000000005  # passso, quanto menor melhor
K = 100              # COEF DESEJADOS
N = len(x)          # qntd de amostras

d = np.zeros(len(x))
e = np.zeros(len(x))
y = np.zeros(len(x), dtype=np.int16)
w = np.zeros(K)

amostrasD = np.zeros(len(coef))
amostrasY = np.zeros(len(w))

########################################################
# Sinal Esperado
# Ruido branco * coeficiente da estranho (MM)
########################################################
for i in range(len(x)):
    for j in range(len(coef)):
        if (i - j) >= 0:
            amostrasD[j] = x[i - j] * float(coef[j])
    d[i] = amostrasD.sum()


########################################################
# Sinal Atual
# Ruido branco * coeficiente que estamos descobrindo (vai iniciar com 0)
########################################################
for i in range(len(x)):
    for j in range(len(w)):
        if (i - j) >= 0:
            amostrasY[j] = x[i-j] * float(w[j])
    y[i] = amostrasY.sum()
########################################################
# Calcula o erro entre o esperado e o que temos
########################################################
    e[i] = d[i] - y[i]

########################################################
# Calcula os novos coef
########################################################
    for k in range(len(w)):
        if (i - k) >= 0:
            w[k] = w[k] + (u * e[i] * x[i-k])

plt.subplot(3,1,1)
plt.plot(d, 'b')
plt.title("Entrada")
plt.grid()
plt.subplot(3,1,2)
plt.plot(y, 'r')
plt.title("Saída")
plt.grid()
plt.tight_layout()
plt.subplot(3,1,3)
plt.plot(e, 'r')
plt.title("Erro")
plt.grid()
plt.show()

print(w)

with open("saida_lms.pcm", 'w') as fid:
    np.array(y, dtype=np.int16).tofile(fid)
fid.close()

with open("saida_lms_d.pcm", 'w') as fid:
    np.array(d, dtype=np.int16).tofile(fid)
fid.close()