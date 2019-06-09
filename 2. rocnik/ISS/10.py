######################################
##          ISS 2018/2019           ##
##             Projekt              ##
##   Vytvoril: Ondrej Studnicka     ##
##         Login: xstudn00          ##
##            Ukol c. 10            ##
######################################

import wave
import numpy as np
import matplotlib.pyplot as plt

sound_1= wave.open('xstudn00.wav','r')
signal = sound_1.readframes(-1)
signal = np.fromstring(signal, 'Int16')
fs = sound_1.getframerate()
signal = signal/24576

delka_ve_vzorcich = sound_1.getnframes()

k = np.arange(-delka_ve_vzorcich+1, delka_ve_vzorcich)
Rv = np.correlate(signal[:delka_ve_vzorcich:], signal[:delka_ve_vzorcich:], 'full') / delka_ve_vzorcich

pocet_indexu = 50
pocatek = k.size // 2 - pocet_indexu
konec = k.size // 2 + pocet_indexu

x = k[pocatek:konec:]
y = Rv[pocatek:konec:]

plt.plot(x, y)
plt.gca().set_xlabel('$k$')

plt.show()

sound_1.close()