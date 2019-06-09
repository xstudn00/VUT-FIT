######################################
##          ISS 2018/2019           ##
##             Projekt              ##
##   Vytvoril: Ondrej Studnicka     ##
##         Login: xstudn00          ##
##            Ukol c. 4             ##
######################################

import wave
from scipy.signal import freqz
import matplotlib.pyplot as plt

sound = wave.open('xstudn00.wav', 'rb')

b = [0.0192, -0.0185, -0.0185, 0.0192]
a = [1, -2.8870, 2.7997, -0.9113]

vzorkovaci_frekvence = sound.getframerate()
delka_ve_vzorcich = sound.getnframes()

ret = []
for i in range(256):
	i = i/256*vzorkovaci_frekvence/2
	ret.append(i)

H = list(freqz(b,a,256))
for i in range(len(H)):
	plt.figure()
	plt.plot(ret, abs(H[i]))
	if i == 0:
		plt.close()

plt.gca().set_ylabel('|H(f)|')
plt.gca().set_xlabel('f[Hz]')

plt.show()

print("Dolni propust")
print("Mezni frekvence: 500Hz")


sound.close()