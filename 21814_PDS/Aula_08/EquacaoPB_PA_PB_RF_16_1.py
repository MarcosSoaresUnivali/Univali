# -----------------------------------------------------------------------------
# 100 'HIGH-PASS > 'LOW-PASS WINDOWED-SINC FILTER
#       - Inverte o Sinal das Amostras no dominio do tempo
#       - Adiciona 1 no Centro de Simetria no dominio do tempo
# -------------------------------------------------------------
# 110 'This program filters 5000 samples with a 101 point windowed-sinc filter,
# 120 'resulting in 4900 samples of filtered data.
# -----------------------------------------------------------------------------
# Autor: Marcos Paulo Soares [Eng. Computacao]
#------------------------------------------------------------------------------
import numpy
import matplotlib.pyplot as plt
import numpy as np
from scipy import signal as sf
#
#------------------------------------------------------------------------------
# Functions
#------------------------------------------------------------------------------
def showrespTime(p_title, p_dim_h, p_Fs, p_Fc, p_Fchz, p_Fthz, p_M):
    plt.stem(p_dim_h) #plotando filtro pobre
    plt.title(f'{p_title} | Fs(hz) = {p_Fs} | Fc(norm) = {p_Fc} | Fchz = {p_Fchz} | Fthz = {p_Fthz} | M(unit) = {p_M}')
    plt.show()
    return 0


def showrespFreq(p_title, p_dim_h, p_Fs, p_Fc, p_Fchz, p_Fthz):
    [w, H] = sf.freqz(p_dim_h,fs=p_Fs) # freqz p_title
    freq = w
    plt.plot(freq, abs(H))
    plt.title(f'Amp.(hz) Frequence response, {p_title}  | Fs(hz) = {p_Fs} | Fc(norm) = {p_Fc} | Fchz = {p_Fchz} | Fthz = {p_Fthz}')
    plt.show()
    return 0


def showconvFile(p_file, p_title, p_dim_h, p_Fs, p_Fc, p_Fchz, p_Fthz, p_M):
    # Input signal Test file... 
    data = numpy.memmap(p_file, dtype='h', mode='r') 
    #
    # convolui sweep com filtro pobre
    res_conv = np.convolve(p_dim_h,data)
    plt.plot(res_conv)
    plt.title(f'Conv. - {p_file}, {p_title} | Fs(hz) = {p_Fs} | Fc(norm) = {p_Fc} | Fchz = {p_Fchz} | Fthz = {p_Fthz} | M(unit) = {p_M}')
    plt.show()
    return 0


def plotResults(p_opc_type, p_file, p_title, p_dim_h, p_Fs, p_Fc, p_Fchz, p_Fthz, p_M):
    #
    showrespTime(f"{p_title} ({p_opc_type}) ", p_dim_h, p_Fs, p_Fc, p_Fchz, p_Fthz, p_M)
    showrespFreq(f"{p_title} ({p_opc_type}) ", p_dim_h, p_Fs, p_Fc, p_Fchz, p_Fthz)
    showconvFile(p_file, f"{p_title} ({p_opc_type}) ", p_dim_h, p_Fs, p_Fc, p_Fchz, p_Fthz, p_M)
    return 0


#------------------------------------------------------------------------------
# Main program
#------------------------------------------------------------------------------
dim_x = numpy.zeros(4999)           #X[ ] holds the input signal
dim_y = numpy.zeros(4999)           #Y[ ] holds the output signal

#-----------------------------------
# Input data filter requirements
#-----------------------------------
pcm_inp_file    = "Sweep40_3400Hz.pcm"      #input file Convolution...
#
Fs              = 8000
#-------------------------------------------------------
Fchz            = 1000
Fc              = Fchz / Fs         # = BW (normalized)
Fthz            = 0                 # 200 - Transition band BW_hz informed 
#-------------------------------------------------------
Fchz1           = 3000
Fc1             = Fchz1 / Fs
Fthz1           = 0                 # 200 - Transition band BW_hz informed 
#------------------------------------------------------------------------------
opf             = input("""
                            +---------------------------------------------------+    
                            |   Which filter should be generated?               |
                            |   (1)LOW-PASS     = 1 | (2)HIGH-PASS      = 2     |
                            |   (3)BAND-PASS    = 3 | (4)REJECT-PASS    = 4     |
                            +---------------------------------------------------+\n    
                        """)
opc             = input("""
                            +---------------------------------------------------+    
                            |   Which window should be ajusted?                 |
                            |   (H)amming       = H | (B)lackman        = B     |
                            +---------------------------------------------------+\n    
                        """)
opf             = int(opf)
#----------------------------
if (opc.upper() == "H"):
    opc_type    = "Hamming"
else:
    opc_type    = "Blackman"
#----------------------------
#
# -----------------------------------------------------------------------------
#               M Calculation
#               ---------------------------------
#               M > 4 / BW = 4 / (Fchz / Fs)
#               M = 4 / 2000 / 8000 = M = 16 coef 
# -----------------------------------------------------------------------------
if Fthz > 0:
    M           = int(4/(Fc))
#
# --- M and cutoff informed ---------------------------------------------------
else:
    Fc          = Fchz / Fs     #Set the cutoff frequency (between 0 and 0.5)
    M           = 100           #Set filter length (101 points)
# -----------------------------------------------------------------------------
if Fthz1 > 0:
    M           = int(4/(Fc1))
#
# --- M and cutoff informed ---------------------------------------------------
else:
    Fc1         = Fchz1 / Fs    #Set the cutoff frequency (between 0 and 0.5)
    M           = 100           #Set filter length (101 points)
#
# --- declare the array from h to M -------------------------------------------
dim_h           = numpy.zeros(M)              #H[ ] holds the filter kernel
dim_h1          = numpy.zeros(M)
dim_h2          = numpy.zeros(M)
# -----------------------------------------------------------------------------
# --- Inverse IFT - EquationSync - Implement in FuncaoSync.py -----------------
# -----------------------------------------------------------------------------
for i in range(M):
    if (i - M / 2) == 0:            #If Erro div 0 
        dim_h[i] = 2 * np.pi * Fc
    else:
        dim_h[i] = np.sin((2 * np.pi * Fc) * (i - M / 2)) / ( i - M / 2) 
#
for i in range(M):
    if (i - M / 2) == 0:            #If Erro div 0 
        dim_h1[i] = 2 * np.pi * Fc1
    else:
        dim_h1[i] = np.sin((2 * np.pi * Fc1) * (i - M / 2)) / ( i - M / 2) 
#
#
# -----------------------------------------------------------------------------
# Plot set of Results no Window...
# -----------------------------------------------------------------------------
plotResults("", pcm_inp_file, "Poor filter", dim_h, Fs, Fc, Fchz, Fthz, M)

# -----------------------------------------------------------------------------
if (opc.upper() == "H"):        # --- Hamming window apllied
# -----------------------------------------------------------------------------
    opc_type = "Hamming"
    #
    for i in range(M):
        dim_h[i] = dim_h[i] * (0.54 - 0.46 * np.cos(2 * np.pi * i / M))  
    #
    for i in range(M):
        dim_h1[i] = dim_h1[i] * (0.54 - 0.46 * np.cos(2 * np.pi * i / M))  
# -----------------------------------------------------------------------------
elif (opc.upper() == "B"):      # --- Blackman window apllied
# -----------------------------------------------------------------------------
    opc_type = "Blackman"
    #
    for i in range(M):
        dim_h[i] = dim_h[i] * (0.42 - 0.5 * np.cos(2 * np.pi * i / M) + 0.08 * np.cos(4 * np.pi * i / M))
    #
    for i in range(M):
        dim_h1[i] = dim_h1[i] * (0.42 - 0.5 * np.cos(2 * np.pi * i / M) + 0.08 * np.cos(4 * np.pi * i / M))
#
# -----------------------------------------------------------------------------
# Plot set of Results with Window...
# -----------------------------------------------------------------------------
plotResults(opc_type, pcm_inp_file, "Windowed", dim_h, Fs, Fc, Fchz, Fthz, M)

# -----------------------------------------------------------------------------
# --- Normalize filter DIV by Sum ---------------------------------------------
# -----------------------------------------------------------------------------
soma = 0
for i in range(M):
    soma = soma + dim_h[i]
#
for i in range(M):
    dim_h[i] = dim_h[i] / soma
#
soma1 = 0
for i in range(M):
    soma1 = soma1 + dim_h1[i]
#
for i in range(M):
    dim_h1[i] = dim_h1[i] / soma1
#

# -----------------------------------------------------------------------------
# Plot set of Results with Window Normalized...
# -----------------------------------------------------------------------------
plotResults(opc_type, pcm_inp_file, "Windowed Normalized", dim_h, Fs, Fc, Fchz, Fthz, M)

# -----------------------------------------------------------------------------
# Plot set of Results with Window Normalized1...
# -----------------------------------------------------------------------------
plotResults(opc_type, pcm_inp_file, "Windowed Normalized", dim_h1, Fs, Fc1, Fchz1, Fthz, M)

# -----------------------------------------------------------------------------
if (opf == 2):              #'HIGH-PASS
# -----------------------------------------------------------------------------
    #- Inverte o Sinal das Amostras no dominio do tempo
    for i in range(M):
        dim_h[i] = -dim_h[i]
    #        
    #- Adiciona 1 no Centro de Simetria no dominio do tempo
    dim_h[int(M / 2)] = dim_h[int(M / 2)] + 1
    #
    # -----------------------------------------------------------------------------
    # Plot set of Results with Window Normalized...
    # -----------------------------------------------------------------------------
    plotResults(opc_type, pcm_inp_file, "HIGH-PASS Windowed Normalized", dim_h, Fs, Fc, Fchz, Fthz, M)

# -----------------------------------------------------------------------------
if (opf == 3):              #'BAND-PASS
# -----------------------------------------------------------------------------
    #-Faz a inversão da frequencia - *[n]& h[n]
    for i in range(M):
        dim_h1[i] = dim_h[i] * -1
    
    dim_h1[int(M / 2)] = dim_h1[int(M / 2)] + 1

    # Plot LOW-PASS in Frequence
    showrespFreq(f"LOW-PASS Windowed Normalized ({opc_type}) ", dim_h, Fs, Fc, Fchz, Fthz)

    # Plot HIGH-PASS in Frequence
    showrespFreq(f"HIGH-PASS Windowed Normalized ({opc_type}) ", dim_h1, Fs, Fc, Fchz, Fthz)

    #-Soma LOW-PASS + HIGH-PASS
    for i in range(M):
        dim_h2[i] = dim_h[i] + dim_h1[i]

    #-----------------------------------------
    #-Faz a inversão da Soma acima -(PB + PA) 
    #-----------------------------------------
    for i in range(M):
        dim_h2[i] = dim_h2[i] * -1

    #-Adiciona 1 no Centro de Simetria 
    dim_h2[int(M/2)] = dim_h2[int(M/2)] + 1

    # -----------------------------------------------------------------------------
    # Plot set of Results with Window Normalized...
    # -----------------------------------------------------------------------------
    plotResults(opc_type, pcm_inp_file, "BAND-PASS Windowed Normalized", dim_h2, Fs, Fc, Fchz, Fthz, M)
    
    #exit()



# --- DEBUG -------------------------------------------------------------------
#exit()

with open(f"Coeficiente_{opf}_{opc_type}.dat", "w") as f:
    for s in dim_h:
        f.write(str(s) +",\n")
