######################################
##          ISS 2018/2019           ##
##             Projekt              ##
##   Vytvoril: Ondrej Studnicka     ##
##         Login: xstudn00          ##
##            Ukol c. 1             ##
######################################



import wave

sound = wave.open('xstudn00.wav', 'rb')
vzorkovaci_frekvence = sound.getframerate()
delka_ve_vzorcich = sound.getnframes()
delka_v_sekundach = delka_ve_vzorcich/vzorkovaci_frekvence
pocet_binarnich_symbolu = int(delka_ve_vzorcich/16)

print("Vzorkovaci frekvence: ", vzorkovaci_frekvence)
print("Delka ve vzorcich: ", delka_ve_vzorcich)
print("Delka v sekundach: ", delka_v_sekundach)
print("Pocet binarnich symbolu: ", pocet_binarnich_symbolu)

sound.close()