######################################
##          ISS 2018/2019           ##
##             Projekt              ##
##   Vytvoril: Ondrej Studnicka     ##
##         Login: xstudn00          ##
##            Ukol c. 3             ##
######################################

from scipy.signal import tf2zpk
import numpy as np
import matplotlib.pyplot as plt

b = [0.0192, -0.0185, -0.0185, 0.0192]
a = [1, -2.8870, 2.7997, -0.9113]

z, p, k = tf2zpk(b, a)
is_stable = (p.size == 0) or np.all(np.abs(p) < 1) 

plt.figure(figsize=(5,4.5))

# jednotkova kruznice
ang = np.linspace(0, 2*np.pi,100)
plt.plot(np.cos(ang), np.sin(ang))

# nuly, poly
plt.scatter(np.real(z), np.imag(z), marker='o', facecolors='none', edgecolors='#F39C12', label='nuly')
plt.scatter(np.real(p), np.imag(p), marker='x', color='g', label='póly')

plt.gca().set_xlabel('Realná složka')
plt.gca().set_ylabel('Imaginarní složka')

plt.legend(loc='upper right')


plt.show()

if is_stable == 1:
	print("Filtr je stabilní")
else:
	print("Filtr není stabilní")
