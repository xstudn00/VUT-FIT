######################################
##          ISS 2018/2019           ##
##             Projekt              ##
##   Vytvoril: Ondrej Studnicka     ##
##         Login: xstudn00          ##
##            Ukol c. 8             ##
######################################

import wave
import numpy as np
import numpy.fft as npf
import matplotlib.pyplot as plt
from scipy.signal import lfilter

sound_1= wave.open('xstudn00.wav','r')
signal = sound_1.readframes(-1)
signal = np.fromstring(signal, 'Int16')
fs = sound_1.getframerate()
signal = signal/24576


signalFFT = npf.fft(signal, 16000)
moduleSignalFFT = abs(signalFFT)
plt.plot(range(8000), moduleSignalFFT[0:8000], label='s[n]')

b = [0.0192, -0.0185, -0.0185, 0.0192]
a = [1, -2.8870, 2.7997, -0.9113]

signalFiltered = lfilter(b, a, signal)


signalFilteredFFT = npf.fft(signalFiltered, 16000)
modulesignalFilteredFFT = abs(signalFilteredFFT)
plt.plot(range(8000), modulesignalFilteredFFT[0:8000], label='ss[n]')

plt.legend(loc='upper right')
plt.ylabel('H[z]')
plt.xlabel('f[Hz]')

plt.show()

sound_1.close()