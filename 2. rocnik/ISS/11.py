######################################
##          ISS 2018/2019           ##
##             Projekt              ##
##   Vytvoril: Ondrej Studnicka     ##
##         Login: xstudn00          ##
##            Ukol c. 11            ##
######################################

import wave
import numpy as np

sound_1= wave.open('xstudn00.wav','r')
signal = sound_1.readframes(-1)
signal = np.fromstring(signal, 'Int16')
fs = sound_1.getframerate()
signal = signal/24576

delka_ve_vzorcich = sound_1.getnframes()

k = np.arange(-50, 50)
Rv = np.correlate(signal, signal, 'full') / delka_ve_vzorcich

print(Rv[-1+delka_ve_vzorcich])
print(Rv[0+delka_ve_vzorcich])
print(Rv[15+delka_ve_vzorcich])



sound_1.close()