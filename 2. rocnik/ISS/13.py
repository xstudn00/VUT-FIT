######################################
##          ISS 2018/2019           ##
##             Projekt              ##
##   Vytvoril: Ondrej Studnicka     ##
##         Login: xstudn00          ##
##            Ukol c. 13            ##
######################################

import wave
import numpy as np

sound_1= wave.open('xstudn00.wav','r')
signal = sound_1.readframes(-1)
signal = np.fromstring(signal, 'Int16')
fs = sound_1.getframerate()
signal = signal/24576

delka_ve_vzorcich = sound_1.getnframes()

px1x2_all = [] # pole funkci hustoty rozdeleni pravdepodobnosti
x1e_all = []   # pole hranic jednotlivych useku, na kterych aproximujeme hodnotu x_1
x2e_all = []   # pole hranic jednotlivych useku, na kterych aproximujeme hodnotu x_2
R_all = []     # pole vypocitanych autokorelacnich koeficientu
int_all = []   # pole integralu p(x1,x2,n1,n2) - melo by byt vzdy rovno 1

x = np.linspace(min(signal), max(signal), 100)

px1x2, x1_edges, x2_edges = np.histogram2d(signal[0:delka_ve_vzorcich-1],signal[1:delka_ve_vzorcich], x, normed=True)
px1x2_all.append(px1x2)
x1e_all.append(x1_edges)
x2e_all.append(x2_edges)

# integral p(x1, x2, n1, n2), mel by vyjit 1
binsize = np.abs(x1_edges[0] - x1_edges[1]) * np.abs(x2_edges[0] - x2_edges[1])
integral = np.sum(px1x2 * binsize)
int_all.append(integral)

print(int_all)


sound_1.close()