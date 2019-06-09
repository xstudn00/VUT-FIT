######################################
##          ISS 2018/2019           ##
##             Projekt              ##
##   Vytvoril: Ondrej Studnicka     ##
##         Login: xstudn00          ##
##            Ukol c. 5             ##
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

plt.legend(loc='upper right')

plt.show()

x = list(np.correlate(signal, signalFiltered, "full"))
y = (max(x))

z = x.index(y)
shift = len(signal) - z
print("Posun: ",shift)

sound_1.close()