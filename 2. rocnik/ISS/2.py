######################################
##          ISS 2018/2019           ##
##             Projekt              ##
##   Vytvoril: Ondrej Studnicka     ##
##         Login: xstudn00          ##
##            Ukol c. 2             ##
######################################

import wave
import struct
import numpy as np
import matplotlib.pyplot as plt

sound = wave.open('xstudn00.wav', 'rb')

ret = []
while sound.tell() < sound.getnframes():
    (decoded, ) = struct.unpack("<h", sound.readframes(1))
    ret.append(decoded)

binary_numbers = []
for i in range(7,32000,16):
    if ret[i] > 0:
        binary_numbers.append(1)
    else:
        binary_numbers.append(0)

sound_1= wave.open('xstudn00.wav','r')
signal = sound_1.readframes(-1)
signal = np.fromstring(signal, 'Int16')
fs = sound_1.getframerate()
max_value = (max(abs(signal)))
signal = signal/max_value

time=np.linspace(0, 0.02, num=320)

plt.xticks(np.arange(0, 0.02, step=0.002))
plt.ylim((-1, 1))
plt.yticks(np.arange(-1, 1.1, step=0.2))
plt.axhline(y=0, color='#44BD32')
plt.plot(time, signal[0:320], label='s[n]')

for i in range(7,320,16):
	if signal[i] > 0:
		plt.vlines(x=time[i], ymin=0.0, ymax=1.0, color='#F39C12')
		plt.scatter(time[i], 1.0, marker='o',facecolors='none', color='#F39C12');
	else:
		plt.scatter(time[i], 0, marker='o',facecolors='none', color='#F39C12');

plt.ylabel('s[n], symbols')
plt.xlabel('t')
plt.legend(loc='upper right')

plt.show()

sound.close()