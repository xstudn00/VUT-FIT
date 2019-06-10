#!/usr/bin/env python3
# -*- coding: utf-8 -*-

####################################################
##              Ondrej Studnicka                  ##
##                 xstudn00                       ##
##               varianta c.2                     ##
##   API KEY: debf856e3b7d009b711eff2993cecce6    ##
####################################################

#nacteni knihoven
import socket
import json
import sys

#kontrola poctu argumentu
#pokud je pocet zadanch argumentu mensi nez 3, tak nejaky argument chybi
#ukonci skript, vypis hlasku
if(len(sys.argv) < 3):
	sys.exit("Error, too few arguments")


#kontrola poctu argumentu
#pokud je pocet zadanch argumentu vetsi nez 3, tak nejaky argument prebyva
#ukonci skript, vypis hlasku
if(len(sys.argv) > 3):
	sys.exit("Error, too much arguments")

#do promennych prirad zadane argumenty
api_key = sys.argv[1]
city = sys.argv[2]

#do url vloz api_key a nastav metricke jednotky
url = '/data/2.5/weather?appid=%s&q=%s&units=metric' % (api_key, city)
#server na ktery se chci pripojit
server = 'api.openweathermap.org'

#vytvoreni a encoding stringu (skladam dohromady jednotlive casti requestu)
request = str("GET " + url + " HTTP/1.1\nHost:" + server + "\r\n\r\n").encode('utf-8')


#vytvoreni socketu pomoci knihovny; vytvori INET, streaming socket
#AF_INET -> Internet address family pro IPv4
#SOCK_STREAM -> typ socketu pro TCP
create_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#vytvori pripojeni TCP
#kam se pripojuji ; nastaveni portu 80 (HTTP – přenos WWW stránek i jiných dat)
#pokud se nepodarilo navazat spojeni, tak ukonci skript a vypis hlasku
try: create_socket.connect(('api.openweathermap.org', 80))
except:
	sys.exit("Could not connect to server")

#posilam soketu data
create_socket.sendall(request)

#ziskavani dat ze serveru; cislo v zavorce udava maxilmalni velikost prijatych dat; dekoduj pomoci UTF-8 
data_from_open_weather = (create_socket.recv(2048)).decode('utf-8')

#promenna data_from_weather bude rozdelena do dvou poli -> header a data_slice
#do novych promennych se ulozi informace rozdelene pomoci bilych znaku \r\n\r\n (v php lze za stejnym ucelem pouzit explode)
header, data_slice = data_from_open_weather.split('\r\n\r\n')

#z headeru je vyparsovana ciselna hodnota, pomoci ktere nasledne zjistujeme jestli se nam podarilo obdrzet data
get_header_status = header.split('\n')
get_header_status = get_header_status[0]
get_header_status = get_header_status.split(' ')
get_header_status = get_header_status[1]


#metoda loads nacte data z retezce
try: usefull_data = json.loads(data_slice)
except:
	sys.exit("Unexpected error")

#pokud se vyparsovana hodnota nerovna cislu 200 (znaci uspech), tak ukoncim skript a podam patricne hlaseni
#v opacnem pripade je vse v poradku a zacnu vypisovat informace na vystup
if (get_header_status != str(200)):
	#pokud jsem obdrzel z open weather zpravu o tom co se nepovedlo, tak ji vytisknu
	#v opacnem pripade vypisu vlastni chybovou hlasku
	if (usefull_data.get('message')):
		sys.exit(usefull_data['message'])
	else:
		sys.exit("Unexpected error")
else: 
	#kontrola jestli jsem obrzel zpet jmeno mesta
	#pokud ano pokracuji k dalsi kontrole
	#pokud ne, tak tisknu: City name:  n/a
	if (usefull_data.get("name")): 
		#kontrola jestli jsme spolecne s mestem obdrzel i informaci, ve kterem state se nachazi
		#pokud ano, tak vytisknu nazev mesta i stat
		#pokud ne, tak vytiskni pouze nazev mesta
		if (usefull_data['sys'].get("country")):
			print(usefull_data['name'] + ', ' + usefull_data['sys']['country'])
		else:
			print(usefull_data['name'])
	else:
		print("City name:  n/a")

	#kontrola jestli jsem prijal popis pocasi
	#pokud ano, tak ho vytisknu
	#pokud ne, tak vytiskni hlasku: Current weather is temporarily not enable.
	if (usefull_data['weather'][0].get("description")):
		print(usefull_data['weather'][0]['description'].title())
	else:
		print("Current weather is temporarily not enable.")

	#kontrola jestli jsem prijal teplotu
	#pokud ano, tak ji vytisknu
	#pokud ne, tak vytiskni hlasku: Temp: n/a
	if (usefull_data['main'].get("temp")):
		print('Temp:\t\t' + str(usefull_data['main']['temp']) + '°C')
	else:
		print("Temp:\t\tn/a")

	#kontrola jestli jsem prijal vlhkost
	#pokud ano, tak ji vytisknu
	#pokud ne, tak vytiskni hlasku: Humidity: n/a
	if (usefull_data['main'].get("humidity")):
		print('Humidity:\t' + str(usefull_data['main']['humidity']) + '%')
	else:
		print("Humidity:\t\tn/a")

	#kontrola jestli jsem prijal tlak
	#pokud ano, tak ho vytisknu
	#pokud ne, tak vytiskni hlasku: Pressure: n/a
	if (usefull_data['main'].get("pressure")):
		print('Pressure:\t' + str(usefull_data['main']['pressure']) + 'hPa')
	else:
		print("Pressure:\t\tn/a")

	#kontrola jestli jsem prijal rychlost vetru
	#pokud ano, tak ji vytisknu (je potreba vynasobit 3,6 pro prevod z m/s na km/h)
	#pokud ne, tak vytiskni hlasku: Wind speed: n/a
	if (usefull_data['wind'].get("speed")):
		print('Wind-speed:\t' + str(round(usefull_data['wind']['speed']) * 3.6) + 'km/h')
	else:
		print("Wind-speed:\t\tn/a")

	#kontrola jestli jsem prijal smer vertru
	#pokud ne, tak vytiskni hlasku: Wind-deg: n/a
	if usefull_data['wind'].get("deg"):
		
		#pokud ano, tak zkontroluji jestli fouka vitr
		#pokud fouka tak vytisknu smer vetru
		#pokud nefouka tak smer vetru je 0
		if str(usefull_data['wind'].get("speed")) != str(0):
			print('Wind-deg:\t' + str(usefull_data['wind']['deg']) + '°' )
		else:
			print('Wind-deg:\t-' )
	else:
		print("Wind-deg:\tn/a")