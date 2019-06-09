######################################
##          ISS 2018/2019           ##
##             Projekt              ##
##   Vytvoril: Ondrej Studnicka     ##
##         Login: xstudn00          ##
##            Ukol c. 6             ##
######################################

import wave
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import lfilter

sound_1= wave.open('xstudn00.wav','r')
signal = sound_1.readframes(-1)
signal = np.fromstring(signal, 'Int16')
fs = sound_1.getframerate()
signal = signal/24576

b = [0.0192, -0.0185, -0.0185, 0.0192]
a = [1, -2.8870, 2.7997, -0.9113]

time=np.linspace(0, 0.02, num=320)

plt.figure(1)
plt.xticks(np.arange(0, 0.02, step=0.002))
plt.ylim((-1, 1))
plt.yticks(np.arange(-1, 1.1, step=0.2))
plt.axhline(y=0, color='#44BD32')
plt.plot(time, signal[0:320], label='s[n]')


signalFiltered = lfilter(b, a, signal)
time=np.linspace(0, 0.02, num=320)
plt.figure(1)
plt.xticks(np.arange(0, 0.02, step=0.002))
plt.ylim((-1, 1))
plt.yticks(np.arange(-1, 1.1, step=0.2))
plt.axhline(y=0, color='#44BD32')
plt.plot(time, signalFiltered[0:320], label='ss[n]')

signalShifted = signalFiltered[15:32000] 

for i in range(7,320,16):
	if signalShifted[i] > 0:
		plt.vlines(x=time[i], ymin=0.0, ymax=1.0, color='#C23616')
		plt.scatter(time[i], 1.0, marker='o',facecolors='none', color='#C23616');
	else:
		plt.scatter(time[i], 0, marker='o',facecolors='none', color='#C23616');

plt.plot(time, signalShifted[0:320], label='ss-shifted[n]')

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

plt.legend(loc='upper right')
plt.ylabel('s[n], ss[n], ss-shifted[n], symbols')
plt.xlabel('t')

plt.show()

sound_1.close()