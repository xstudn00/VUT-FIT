######################################
##          ISS 2018/2019           ##
##             Projekt              ##
##   Vytvoril: Ondrej Studnicka     ##
##         Login: xstudn00          ##
##            Ukol c. 7             ##
######################################

import wave
import numpy as np
from scipy.signal import lfilter

sound_1= wave.open('xstudn00.wav','r')
signal = sound_1.readframes(-1)
signal = np.fromstring(signal, 'Int16')
fs = sound_1.getframerate()
signal = signal/24576

b = [0.0192, -0.0185, -0.0185, 0.0192]
a = [1, -2.8870, 2.7997, -0.9113]

time=np.linspace(0, 0.02, num=320)

signalFiltered = lfilter(b, a, signal)

signalShifted = signalFiltered[15:32000] 

binary_numbers_shifted = []
for i in range(7,31985,16):
    if signalShifted[i] > 0:
       binary_numbers_shifted.append(1)
    else:
       binary_numbers_shifted.append(0)

binary_numbers = []
for i in range(7,32000,16):
    if signal[i] > 0:
       binary_numbers.append(1)
    else:
      binary_numbers.append(0)

counter = 0
for i in range(1999):
 if binary_numbers[i] != binary_numbers_shifted[i]:
   counter = counter + 1

print("Počet chyb: ", counter)
print("Chybovost: ", counter/1999*100, "%")

sound_1.close()