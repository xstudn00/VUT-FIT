######################################
##          ISS 2018/2019           ##
##             Projekt              ##
##   Vytvoril: Ondrej Studnicka     ##
##         Login: xstudn00          ##
##            Ukol c. 9             ##
######################################

import wave
import numpy as np
import matplotlib.pyplot as plt

sound_1= wave.open('xstudn00.wav','r')
signal = sound_1.readframes(-1)
signal = np.fromstring(signal, 'Int16')
fs = sound_1.getframerate()
signal = signal/24576

xmin = np.min(signal)
xmax = np.max(signal)
n_aprx = 50
x = np.linspace(xmin, xmax, n_aprx)

binsize = np.abs(x[1] - x[0])
hist, _ = np.histogram(signal, n_aprx)
px = hist / 32000

plt.figure(figsize=(8,3))
plt.plot(x, px)
plt.gca().set_xlabel('$x$')
plt.gca().set_title(f'Odhad funkce hustoty rozdělení pravděpodobnosti')

plt.show()

#integral
integral= np.sum(px)
print(integral)

sound_1.close()