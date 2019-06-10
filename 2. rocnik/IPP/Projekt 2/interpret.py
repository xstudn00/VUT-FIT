#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#####################################
##          IPP 2. projekt         ##
##            interpret            ##
##            2018/2019            ##
##   Ondrej Studnicka (xstudn00)   ##
#####################################

import sys
import re
import os
import xml.etree.ElementTree as et

## kontrola poctu argumentu
number_of_arguments=len(sys.argv)
if number_of_arguments !=2 and number_of_arguments != 3:
	sys.exit(10)

## --help
if number_of_arguments == 2 and re.match(r'^(--help)$', sys.argv[1]) is not None:
	print("Program nacte XML reprezentaci programu ze zadaneho souboru a tento program s vyuzitim standardniho vstupu a vystupu interpretuje. Vstupni XML reprezentace je napr. generovana skriptem parse.php (ale ne nutne) ze zdrojoveho kodu v IPPcode19.")
	sys.exit(0)

## --source
## pokud je zadan source, ktery neexistuje -> konec s chybou 11
## jinak do source_list nacti obsah souboru (po radku)
elif number_of_arguments == 2 and re.match(r'^--source=.+$', sys.argv[1]) is not None:
	source = sys.argv[1][9:]
	if(not os.path.isfile(source)):
		sys.exit(11)

	source_list = []

	source=open(source, 'r')
	for line in source:
		source_list.append(line)

## -- input
## pokud je zadan input, ktery neexistuju -> konec s chybou 11
## jinak do input_list nacti radky ze souboru
## a ze stdin cti po radku vstup, ten pak uloz do source _list 
elif number_of_arguments == 2 and re.match(r'^--input=.+$', sys.argv[1]) is not None:
	input_file = sys.argv[1][8:]
	if(not os.path.isfile(input_file)):
		sys.exit(11)

	input_list = []

	input_file=open(input_file, 'r')
	for line in input_file:
		line = line.splitlines()
		input_list.append(line)

	source = sys.stdin
	source = source.readlines()
	i = 0
	size = len(source)
	source_list = []
	while i < size:
		source[i] = source[i].strip()
		if source[i]:
			source_list.append(source[i])
		i = i+1

## --input --source 
## nacti do input_list soubor (pokud existuje), nacti do source_list soubor (pokud existuje)
elif number_of_arguments == 3 and re.match(r'^--input=.+$', sys.argv[1]) is not None and re.match(r'^--source=.+$', sys.argv[2]) is not None:
	source = sys.argv[2][9:]
	input_file = sys.argv[1][8:]
	if(not os.path.isfile(source)):
		sys.exit(11)

	source_list = []

	source=open(source, 'r')
	for line in source:
		source_list.append(line)


	if(not os.path.isfile(input_file)):
		sys.exit(11)

	input_list = []

	input_file=open(input_file, 'r')
	for line in input_file:
		line = line.splitlines()
		input_list.append(line)

## --source --input 
## nacti do input_list soubor (pokud existuje), nacti do source_list soubor (pokud existuje)
elif number_of_arguments == 3 and re.match(r'^--input=.+$', sys.argv[2]) is not None and re.match(r'^--source=.+$', sys.argv[1]) is not None:
	source = sys.argv[1][9:]
	input_file = sys.argv[2][8:]
	if(not os.path.isfile(source)):
		sys.exit(11)

	source_list = []

	source=open(source, 'r')
	for line in source:
		source_list.append(line)


	if(not os.path.isfile(input_file)):
		sys.exit(11)
	
	input_list = []

	input_file=open(input_file, 'r')
	for line in input_file:
		line = line.splitlines()
		input_list.append(line)

else:
	sys.exit(10)


## zkus nacist XML reprezentaci
## pokud except -> chyba 31
try: 
	program = et.fromstringlist(source_list)
except: 
	sys.exit(31)

## osetreni tagu program
if program.tag != "program":
	sys.exit(32)

if len(program.attrib) < 1 and len(program.attrib) > 3:
	sys.exit(32)

if len(program.attrib) == 2:
	if ('name' not in program.attrib) and ('description' not in program.attrib):
		sys.exit(32)

if len(program.attrib) == 3:
	if ('name' not in program.attrib) or ('description' not in program.attrib):
		sys.exit(32)


if ('language' not in program.attrib):
        sys.exit(32)
else:
	if program.get("language") != "IPPcode19":
		sys.exit(32)

## osetreni instrukce
for instrukce in program:
	if len(instrukce.attrib) !=2:
		sys.exit(32)
	if 'opcode' not in instrukce.attrib or 'order' not in instrukce.attrib:
		sys.exit(32)
	if instrukce.tag != 'instruction':
		sys.exit(32)

## osetreni instrukce
for instrukce in program:
	for atribut in instrukce:
		if len(atribut.attrib) != 1:
			sys.exit(32)
		if 'type' not in atribut.attrib:
			sys.exit(32)
		if atribut.tag != 'arg1' and atribut.tag != 'arg2' and atribut.tag != 'arg3':
			 sys.exit(32)

		if len(atribut) != 0:
			sys.exit(32)

## sortovani instrukci podle order
data = []
for instrukce in program:
    key = instrukce.get("order")
    data.append((int(key), instrukce))

## except nastane pokud jsou dve stejne cisla
try:
	data.sort()
except:
	sys.exit(32)

program[:] = [item[-1] for item in data]

## sortovani argumentu instrukce podle cisla u argumentu
for instrukce in program:
	data_2 = []
	for atribut in instrukce:
		key = int(atribut.tag[3:])
		data_2.append((key, atribut))

	try:
		data_2.sort()
	except:
		sys.exit(32)
	instrukce[:] = [item[-1] for item in data_2]

## kontrola jestli jde order po porade (1, 2, 3, ..., n)
i = 1
for instrukce in program:
	if int(instrukce.get("order")) != i:
		sys.exit(32)
	i = i+1

## pripustne instrukce rozdelene podle poctu argumentu
inst_no_opr = ["CREATEFRAME", "PUSHFRAME", "POPFRAME", "RETURN", "BREAK"]
instr_1_var = ["DEFVAR", "POPS"]
instr_1_label = ["CALL", "LABEL", "JUMP"]
instr_1_symbol = ["PUSHS", "WRITE", "EXIT", "DPRINT"]
instr_2_var_symb = ["MOVE", "INT2CHAR", "STRLEN", "TYPE", "NOT"]
instr_2_var_type = ["READ"]
instr_3_label_symb_symb = ["JUMPIFEQ", "JUMPIFNEQ"]
instr_3_var_symb_symb = ["ADD", "SUB", "MUL", "IDIV", "LT", "GT", "EQ", "AND", "OR", "STRI2INT", "CONCAT", "GETCHAR", "SETCHAR"]

## pro kazdou instrukci v programu
## vem opcode
## preved opcode na velke pismena
## zkontroluj jestli se instrukce nachazi v jednom z poli (pokud ne tak chyba 32)
## podle toho v jakem poli se instrunkce nachazi zkontroluj jestli jestli sedi pocet jejich argumentu (pokud nesedi tak chyba 32)
for instrukce in program:
	tested_instruction = instrukce.attrib.get("opcode")
	tested_instruction = tested_instruction.upper()

	if tested_instruction not in inst_no_opr and tested_instruction not in instr_1_var and tested_instruction not in instr_1_label and tested_instruction not in instr_1_symbol and tested_instruction not in instr_2_var_symb and tested_instruction not in instr_2_var_type and tested_instruction not in instr_3_label_symb_symb and tested_instruction not in instr_3_var_symb_symb:
		sys.exit(32)

	if tested_instruction in inst_no_opr:
		size = len(instrukce)
		if size != 0:
			sys.exit(32)

	if tested_instruction in instr_1_var:
		size = len(instrukce)
		if size != 1:
			sys.exit(32)

	if tested_instruction in instr_1_label:
		size = len(instrukce)
		if size != 1:
			sys.exit(32)

	if tested_instruction in instr_1_symbol:
		size = len(instrukce)
		if size != 1:
			sys.exit(32)

	if tested_instruction in instr_2_var_symb:
		size = len(instrukce)
		if size != 2:
			sys.exit(32)

	if tested_instruction in instr_2_var_type:
		size = len(instrukce)
		if size != 2:
			sys.exit(32)

	if tested_instruction in instr_3_label_symb_symb:
		size = len(instrukce)
		if size != 3:
			sys.exit(32)

	if tested_instruction in instr_3_var_symb_symb:
		size = len(instrukce)
		if size != 3:
			sys.exit(32)


## regexy pro symbol, label a type
var_regex = '^(GF|LF|TF)(@)([a-zA-Z]|[_\-$&%*!?])([a-zA-Z]|[0-9]|[_\-$&%*!?])*$'
label_regex = '^([a-zA-Z]|[_\-$&%*!?])([a-zA-Z]|[0-9]|[_\-$&%*!?])*$'
type_regex = '^(int|bool|string)$'

## pro kazdou instrukci v programu zkontroluj
## jestli sedi ocekavany typ neterminalu se svym regexem (jinak chyba 32)
for instrukce in program:
	tested_instruction = instrukce.attrib.get("opcode")
	tested_instruction = tested_instruction.upper()
	#print(tested_instruction)

	if tested_instruction in instr_1_var:
		point = 1
	if tested_instruction in instr_1_label:
		point = 2
	if tested_instruction in instr_1_symbol:
		point = 3
	if tested_instruction in instr_2_var_symb:
		point = 4
	if tested_instruction in instr_2_var_type:
		point = 5
	if tested_instruction in instr_3_label_symb_symb:
		point = 6
	if tested_instruction in instr_3_var_symb_symb:
		point = 7

	pocitadlo = 1

	## pro kazdy argument v instrukci kontroluj jestli:
	## se nachazi \ v textu 
	##     pokud ano tak zkus vzit nasledujici tri znaky a zkontroluj jestli to jsou cisla -> pokud ne chyba 32 -> pokud ano tak preved ty tri cisla na znak
	## se nachayi # v textu
	##     pokud ano tak chyba 32
	## sedi opcode pro instrukci
	##     pokud nesedi tak chyba 32
	for atribut in instrukce:
		porovnavac = atribut.tag[3:]
		if atribut.text:
			if "\\" in r"%r" % atribut.text:
				esc_index = []
				for i in range(len(atribut.text)):
					if atribut.text[i] == "\\":
						esc_index.append(i)

				for i in range(len(esc_index)):
					try:
						prvni = atribut.text[esc_index[i]+1]
					except:
						sys.exit(32)
					try:
						druhy = atribut.text[esc_index[i]+2]
					except:
						sys.exit(32)
					try:
						treti = atribut.text[esc_index[i]+3]
					except:
						sys.exit(32)

					if prvni.isdigit() and druhy.isdigit() and treti.isdigit():
						continue
					else:
						sys.exit(32)

				esc = re.findall("\\\\[0-9][0-9][0-9]", atribut.text)
				for escaped in esc:
					atribut.text = atribut.text.replace(escaped, chr(int(escaped.lstrip('\\'))))

			if "#" in r"%r" % atribut.text:
				sys.exit(32)

		
		if point == 1:
			if str(pocitadlo) != porovnavac:
				sys.exit(32)
			
			if atribut.get('type') != "var":
				sys.exit(32)
			else:
				if re.match(var_regex, atribut.text) is None:
					sys.exit(32)

		if point == 2:
			if str(pocitadlo) != porovnavac:
				sys.exit(32)

			if atribut.get('type') != "label":
				sys.exit(32)
			else:
				if re.match(label_regex, atribut.text) is None:
					sys.exit(32)

		if point == 3:
			if str(pocitadlo) != porovnavac:
				sys.exit(32)

			if atribut.get('type') != "int" and atribut.get('type') != "string" and atribut.get('type') != "bool" and atribut.get('type') != "nil" and atribut.get('type') != "var":
				sys.exit(32)

			if atribut.get("type") == "var":
				if re.match(var_regex, atribut.text) is None:
						sys.exit(32)

			if atribut.get("type") == "int":
				if re.match(r'^(.+)$', atribut.text) is None:
						sys.exit(32)

			if atribut.get("type") == "string":
				if atribut.text:
					if re.match(r'^(.*)$', atribut.text) is None:
							sys.exit(32)
						
			if atribut.get("type") == "bool":
				if re.match(r'^(true|false)$', atribut.text) is None:
						sys.exit(32)

			if atribut.get("type") == "nil":
				if re.match(r'^(nil)$', atribut.text) is None:
						sys.exit(32)			

		if point == 4:
			if str(pocitadlo) != porovnavac:
				sys.exit(32)

			if atribut.tag == "arg1":
				if atribut.get('type') != "var":
					sys.exit(32)
				else:
					if re.match(var_regex, atribut.text) is None:
						sys.exit(32)

			if atribut.tag == "arg2":
				if atribut.get('type') != "int" and atribut.get('type') != "string" and atribut.get('type') != "bool" and atribut.get('type') != "nil" and atribut.get('type') != "var":
					sys.exit(32)

				if atribut.get("type") == "var":
					if re.match(var_regex, atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "int":
					if re.match(r'^(.+)$', atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "string":
					if atribut.text:
						if re.match(r'^(.*)$', atribut.text) is None:
							sys.exit(32)
						
				if atribut.get("type") == "bool":
					if re.match(r'^(true|false)$', atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "nil":
					if re.match(r'^(nil)$', atribut.text) is None:
						sys.exit(32)

		if point == 5:
			if str(pocitadlo) != porovnavac:
				sys.exit(32)

			if atribut.tag == "arg1":
				if atribut.get('type') != "var":
					sys.exit(32)
				else:
					if re.match(var_regex, atribut.text) is None:
						sys.exit(32)

			if atribut.tag == "arg2":
				if atribut.get('type') != "type":
					sys.exit(32)
				else:
					if re.match(type_regex, atribut.text) is None:
						sys.exit(32)


		if point == 6:
			if str(pocitadlo) != porovnavac:
				sys.exit(32)

			if atribut.tag == "arg1":
				if atribut.get('type') != "label":
					sys.exit(32)
				else:
					if re.match(label_regex, atribut.text) is None:
						sys.exit(32)

			if atribut.tag == "arg2":
				if atribut.get('type') != "int" and atribut.get('type') != "string" and atribut.get('type') != "bool" and atribut.get('type') != "nil" and atribut.get('type') != "var":
					sys.exit(32)

				if atribut.get("type") == "var":
					if re.match(var_regex, atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "int":
					if re.match(r'^(.+)$', atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "string":
					if atribut.text:
						if re.match(r'^(.*)$', atribut.text) is None:
							sys.exit(32)
						
				if atribut.get("type") == "bool":
					if re.match(r'^(true|false)$', atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "nil":
					if re.match(r'^(nil)$', atribut.text) is None:
						sys.exit(32)

			if atribut.tag == "arg3":
				if atribut.get('type') != "int" and atribut.get('type') != "string" and atribut.get('type') != "bool" and atribut.get('type') != "nil" and atribut.get('type') != "var":
					sys.exit(32)

				if atribut.get("type") == "var":
					if re.match(var_regex, atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "int":
					if re.match(r'^(.+)$', atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "string":
					if atribut.text:
						if re.match(r'^(.*)$', atribut.text) is None:
							sys.exit(32)
						
				if atribut.get("type") == "bool":
					if re.match(r'^(true|false)$', atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "nil":
					if re.match(r'^(nil)$', atribut.text) is None:
						sys.exit(32)

		if point == 7:
			if str(pocitadlo) != porovnavac:
				sys.exit(32)

			if atribut.tag == "arg1":
				if atribut.get('type') != "var":
					sys.exit(32)
				else:
					if re.match(var_regex, atribut.text) is None:
						sys.exit(32)

			if atribut.tag == "arg2":
				if atribut.get('type') != "int" and atribut.get('type') != "string" and atribut.get('type') != "bool" and atribut.get('type') != "nil" and atribut.get('type') != "var":
					sys.exit(32)

				if atribut.get("type") == "var":
					if re.match(var_regex, atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "int":
					if re.match(r'^(.+)$', atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "string":
					if atribut.text:
						if re.match(r'^(.*)$', atribut.text) is None:
							sys.exit(32)
						
				if atribut.get("type") == "bool":
					if re.match(r'^(true|false)$', atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "nil":
					if re.match(r'^(nil)$', atribut.text) is None:
						sys.exit(32)

			if atribut.tag == "arg3":
				if atribut.get('type') != "int" and atribut.get('type') != "string" and atribut.get('type') != "bool" and atribut.get('type') != "nil" and atribut.get('type') != "var":
					sys.exit(32)

				if atribut.get("type") == "var":
					if re.match(var_regex, atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "int":
					if re.match(r'^(.+)$', atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "string":
					if atribut.text:
						if re.match(r'^(.*)$', atribut.text) is None:
							sys.exit(32)
						
				if atribut.get("type") == "bool":
					if re.match(r'^(true|false)$', atribut.text) is None:
						sys.exit(32)

				if atribut.get("type") == "nil":
					if re.match(r'^(nil)$', atribut.text) is None:
						sys.exit(32)
		pocitadlo = pocitadlo + 1

## do all_data uloz instrukci, type, opcode a order
all_data = []
for instrukce in program:
	nonterminal_array = []
	if len(instrukce) > 0:
		for atribut in instrukce:
			atribut_data = []
			atribut_data.append(atribut.get("type"))
			if atribut.text:
				atribut_data.append(atribut.text)
			nonterminal_array.append(atribut_data)
	upper_opcode = instrukce.get("opcode")
	upper_opcode = upper_opcode.upper()
	nonterminal_array.insert(0, upper_opcode)
	nonterminal_array.append(instrukce.get("order"))
	all_data.append(nonterminal_array)


globalni_ramec_promenne = []
lokalni_ramec_promenne = []
docasny_ramec_promenne = []

## funkce na definice promenne
## rozdel argument podle @
## vyber ramec do ktereho patri (pokud je ramec spatny tak chyba 55)
## zkontroluj jestli uz se stejna promenna v ramci nenachazi
## pokud je promenna v globalnim ramci tak ji tam uloz a nastav jeji hodnoty na None -> promenna po praci bude vypadat nasledovne : ('5', 'int')
## pokud je promenna v lokalnim ramci tak se podivej jestli ramec existuje (jinak chyba 55), a zkontroluj jestli se v ramci jiz nenachazi -> pokud ne tak uloz (jinak chyba 52)
## pokud je promenna v docasnem ramci tak se podivej jestli ramec existuje (jinak chyba 55), a zkontroluj jestli se v ramci jiz nenachazi -> pokud ne tak uloz (jinak chyba 52)
def defvar_function(promenna):
		ramec, promenna = promenna.split('@', 1)
		
		if ramec == "GF":
			if promenna not in globalni_ramec_promenne:
				globalni_ramec_promenne.append(promenna)
				globalni_ramec[promenna] = (None, None)
			else:
				sys.exit(52)

		elif ramec == "LF":
			if lokalni_ramec is None:
				sys.exit(55)
			else:
				if promenna not in lokalni_ramec_promenne:
					lokalni_ramec_promenne.append(promenna)
					lokalni_ramec[promenna] = (None, None)
				else:
					sys.exit(52)

		elif ramec == "TF":
			if docasny_ramec is None:
				sys.exit(55)
			else:
				if promenna not in docasny_ramec_promenne:
					docasny_ramec_promenne.append(promenna)
					docasny_ramec[promenna] = (None, None)
				else:
					sys.exit(52)

		else:
			sys.exit(55)


## funkce pro kontrolu jestli je promenna definovana
## rozdel argument podle @
## hledej v kterem ramci je
## pokud neni v zadnem ramci tak chyba 54
## pokud hledany ramec neexistuje tak chyba 55
def check_frame_function(promenna):
	
	ramec, promenna = promenna.split("@", 1)

	if ramec == "GF":
		if promenna not in globalni_ramec:
			sys.exit(54)

	if ramec == "LF":
		if lokalni_ramec is None:
			sys.exit(55)
		else:
			if promenna not in lokalni_ramec:
				sys.exit(54)

	if ramec == "TF":
		if docasny_ramec is None:
			sys.exit(55)
		else:
			if promenna not in docasny_ramec:
				sys.exit(54)


## funkce pro nastaveni hodnoty promenne
## rozdel argument podle @
## najdi ramec
## prirad k hledane promenne hodnotu a typ
def set_var_value(promenna, hodnota, typ):
	ramec, promenna = promenna.split("@", 1)
	if ramec == "GF":
		globalni_ramec[promenna] = (hodnota, typ)

	if ramec == "LF":
		lokalni_ramec[promenna] = (hodnota, typ)

	if ramec == "TF":
		docasny_ramec[promenna] = (hodnota, typ)

## funkce pro ziskani hodnoty promenne
def get_value(promenna, ramec):
	if ramec == "GF":
		return globalni_ramec[promenna][0]
	elif ramec == "LF":
		return lokalni_ramec[promenna][0]
	elif ramec == "TF":
		return docasny_ramec[promenna][0]
	else:
		sys.exit(54)

## funkce pro ziskani typu promenne
def get_type(promenna, ramec):
	if ramec == "GF":
		return globalni_ramec[promenna][1]
	elif ramec == "LF":
		return lokalni_ramec[promenna][1]
	elif ramec == "TF":
		return docasny_ramec[promenna][1]
	else:
		sys.exit(54)


## definice pocatecniho stavu intrpretu
globalni_ramec = {}
docasny_ramec = None
lokalni_ramec = None
zasobnik = []
zasobnik_volani = []
zasobnik_ramcu = []
navesti = []
pocet_instrukci_read = 0

## kontrola jestli v programu neni redefinice navesti
for i in range(len(all_data)):
	current_instruction = all_data[i][0]
	if current_instruction == "LABEL" :
		if all_data[i][1][1] not in navesti: 
				navesti.append(all_data[i][1][1])
		else:
			sys.exit(52)


#print(all_data)
## zaloha dat kvuli instrukci CALL a RETURN
zaloha = all_data
k = 1

## funkce pro vycisteni pole
def pop_all(l):
    r, l[:] = l[:], []
    return r

## funkce pro interpretaci
## pro kazdou instrukci v programu najdi jak se jmenuje a chovej se podle urcenych kroku
def interpretace(all_data):

	for i in range(len(all_data)):
		global lokalni_ramec, docasny_ramec, pocet_instrukci_read, k
		current_instruction = all_data[i][0]
		
		## CREATEFRAME - > vytvor novy docasny ramec a odstran z docasny_ramec_promenne vsechny promenne
		if current_instruction == "CREATEFRAME" :
			docasny_ramec = {}
			pop_all(docasny_ramec_promenne)
		
		## PUSHFRAME -> pokud neni co PUSHovat tak chyba 55
		## jinak do zasobniku ramcu uloz docasny ramec, na lokalnim ramci nastav novy prvni prvek, docasny_ramec nastav na None
		elif current_instruction == "PUSHFRAME" :
			if docasny_ramec is None:
				sys.exit(55)
			else:
				zasobnik_ramcu.append(docasny_ramec)
				lokalni_ramec = zasobnik_ramcu[len(zasobnik_ramcu)-1]
				docasny_ramec = None
		
		## POPFRAME -> pokud neni co POPovat, tak chyba 55
		## jinak docasny_ramec nastav na nulty prvek co se nachazi v zasobniku ramcu (ten pak ze zasobniku ramcu smaz)
		## zjisit jaka je aktualni velikost zasobniku ramcu 
		## pokud je nula tak lokalni ramec je None
		## jinak je lokalniho ramce nastav novy prvni prvek
		elif current_instruction == "POPFRAME" :
			if lokalni_ramec is None:
				sys.exit(55)
			else:
				docasny_ramec = zasobnik_ramcu[0]
				zasobnik_ramcu.pop(0)
	
				velikost = len(zasobnik_ramcu)
	
				if velikost  == 0:
					lokalni_ramec = None
				else:
					lokalni_ramec = zasobnik_ramcu[velikost-1]
	
		## RETURN -> obnov all_data ze zalohy
		## pokud je zasobnik volani 0 -> chyba 56
		## jinak vrat interpretaci na prvni prvek za labelem ktery je ulozen na ulozen na konci pole zasobnik volani
		elif current_instruction == "RETURN" :
			all_data = zaloha
			if not zasobnik_volani:
				sys.exit(56)
			else:
				#print(zasobnik_volani)
				skok = zasobnik_volani.pop()
				#print(zasobnik_volani)
				interpretace(all_data[skok-1:])
				break

		## BREAK -> vypise aktualni stav insterpretu na stderr
		elif current_instruction == "BREAK" :
			pozice = str("Pozice v kodu: " + str(k) + "\n")
			vykonano = str("Pocet vykonanych instrukci: " + str(k-1) + "\n")
			globalni_ramec_stav = str("Globalni ramec: " +  str(globalni_ramec) + "\n")
			lokalni_ramec_stav = str("Lokalni ramec: " +  str(lokalni_ramec) + "\n")
			docasny_ramec_stav = str("Docasny ramec: " + str(docasny_ramec) + "\n")

			sys.stderr.write("---- Vypis stavu interpreteru na stderr ----\n")
			sys.stderr.write(pozice)
			sys.stderr.write(vykonano)
			sys.stderr.write(globalni_ramec_stav)
			sys.stderr.write(lokalni_ramec_stav)
			sys.stderr.write(docasny_ramec_stav)
		
		## DEFVAR -> definice promenne
		elif current_instruction == "DEFVAR" :
			defvar_function(all_data[i][1][1])

		## POPS 
		## zjisit jestli je zadana promenna definovana
		## pokud v zasobniku nic neni tak chyba 56
		## jinak zjisti hodnotu a typ
		## informace pak nastav do zadane promenne
		elif current_instruction == "POPS" :
			check_frame_function(all_data[i][1][1])
			if zasobnik == []:
				sys.exit(56)
			else:
				hodnota = zasobnik.pop(0)
				try:
					typ = hodnota[1]
				except:
					typ = None
				
				try:
					hodnota = hodnota[0]
				except:
					hodnota = None

				set_var_value(all_data[i][1][1], hodnota, typ)

		## CALL -> zkontroluj jestli je label definovan (jinak chyba 52)
		## do zasobniku volani uloz dalsi pozici za aktualni instrukci
		## najdi kam skocit
		## na nalezene instrukci zahaj interpretovani
		elif current_instruction == "CALL" :

			if all_data[i][1][1] in navesti:
				zasobnik_volani.append(int(all_data[i][2])+1)
				for z in range(len(all_data)):
					if len(all_data[z]) > 2:
						if all_data[z][1][1] == all_data[i][1][1] and all_data[z][1][1] in navesti:
							skok = z+1
							

				interpretace(all_data[skok:])
				break
			else:
				sys.exit(52)
		
		## LABEL -> pokracuj v interpretaci
		elif current_instruction == "LABEL" :
			continue

		## JUMP zjisti jestli je zadany label v poli navesti (pokud ne tak chyba 52)
		## najdi kam skocit
		## na zadanem indexu zahaj novou interpretaci
		elif current_instruction == "JUMP" :
			#print(all_data[i][0])
			if all_data[i][1][1] in navesti:
				for z in range(len(all_data)):
					if len(all_data[z]) > 2:
						if all_data[z][1][1] == all_data[i][1][1] and all_data[z][1][1] in navesti:
							skok = z+1
				interpretace(all_data[skok:])
				break

			else:
				sys.exit(52)

		## PUSHS -> zkontroluj jestli je promenna definovana
		## najdi hodnotu a typ argumentu (pokud jeto promenna tak jeste zjisti jestli sedi typ a hodnota do instrukce)
		## vloz na zasobnik hodnotu a typ
		elif current_instruction == "PUSHS" :
			promenna = all_data[i][1][0]
			#print(zasobnik)
			if promenna == "var":
				check_frame_function(all_data[i][1][1])

				hledane = all_data[i][1][1]
				ramec, hledane = hledane.split("@", 1)
		
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][1][1]
				ramec, typ = typ.split("@", 1)
		
				typ = get_type(typ, ramec)

				if typ == "int":
					try:
						hodnota_1 = int(hodnota_1)
					except:
						sys.exit(57)

				elif typ == "string":
					if not isinstance(hodnota_1, str):
						sys.exit(57)

				elif typ == "bool":
					if hodnota_1 != "true" and hodnota_1 != "false":
						sys.exit(53)

				elif typ == "nil":
					if hodnota_1 != "nil":
						sys.exit(57)

				else:
					sys.exit(53)


			elif promenna == "int":
				try:
					hodnota_1 = int(all_data[i][1][1])
				except:
					sys.exit(57)

				typ = "int"

			elif promenna == "string":
				if not isinstance(all_data[i][1][1], str):
					sys.exit(57)

				hodnota_1 = all_data[i][1][1]
				typ = "string"

			elif promenna == "bool":
				if all_data[i][1][1] != "true" and all_data[i][1][1] != "false":
					sys.exit(57)

				hodnota_1 = all_data[i][1][1]
				typ = "bool"

			elif promenna == "nil":
				if all_data[i][1][1] != "nil":
					sys.exit(57)
				hodnota_1 = all_data[i][1][1]
				typ = "nil"

			else:
				sys.exit(53)

			vkladane = [hodnota_1, typ]

			zasobnik.append(vkladane)

		## WRITE -> pokud je promenna tak zjisti jestli je definovana a jestli sedi typ
		## jinak kontroluj jestli sedi typ a hodnota
		## nalezenou hodnotu vytiskni na stdout
		elif current_instruction == "WRITE" :

			promenna = all_data[i][1][0]
			if promenna == "var":
				check_frame_function(all_data[i][1][1])

				hledane = all_data[i][1][1]
				ramec, hledane = hledane.split("@", 1)
				#print(ramec, hledane)
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][1][1]
				ramec, typ = typ.split("@", 1)
				#print(ramec, typ)
				typ = get_type(typ, ramec)

				if typ == "int":
					try:
						hodnota_1 = int(hodnota_1)
					except:
						sys.exit(57)

				elif typ == "string":
					if not isinstance(hodnota_1, str):
						sys.exit(57)

				elif typ == "bool":
					if hodnota_1 != "true" and hodnota_1 != "false":
						sys.exit(57)
				elif typ == "nil":
					if hodnota_1 != "nil":
						sys.exit(57)
					else:
						continue
				else:
					sys.exit(53)

				print(hodnota_1, end='')


			elif promenna == "int":
				try:
					hodnota_1 = int(all_data[i][1][1])
				except:
					sys.exit(57)

				print(hodnota_1, end='')

			elif promenna == "string":
				hodnota_1 = all_data[i][1][1]
				if not isinstance(hodnota_1, str):
					sys.exit(57)
				else:
					print(hodnota_1, end='')

			elif promenna == "bool":
				hodnota_1 = all_data[i][1][1]
				if hodnota_1 != "true" and hodnota_1 != "false":
					sys.exit
				else:
					print(hodnota_1, end='')

			elif promenna == "nil":
				hodnota_1 = all_data[i][1][1]
				
				if hodnota_1 != "nil":
					sys.exit(57)
				else:
					continue

			else:
				sys.exit(53)

		## EXIT -> pokud je v exitu promenna tak zjisti jestli je definovana a zjisti jestli sedi typ a hodnota
		## pokud sedi a hodnota je v rozmezi 0 az 49 (vcetne) tak ukonci interpretaci se zadanou chybou
		elif current_instruction == "EXIT" :
			if all_data[i][1][0] == "var":
				#print(all_data[i][1][1])
				check_frame_function(all_data[i][1][1])

				hledane = all_data[i][1][1]
				ramec, hledane = hledane.split("@", 1)

				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][1][1]
				ramec, typ = typ.split("@", 1)
		
				typ = get_type(typ, ramec)

				if typ != "int":
					sys.exit(53)

				else:
					try:
						int(hodnota_1)
					except:
						sys.exit(57)
	
					if int(hodnota_1) >=0 and int(hodnota_1) <=49:
						sys.exit(int(hodnota_1))
					else:
						sys.exit(57)


			elif all_data[i][1][0] == "int":
				try:
					int(all_data[i][1][1])
				except:
					sys.exit(57)
	
				if int(all_data[i][1][1]) >=0 and int(all_data[i][1][1]) <=49:
					sys.exit(int(all_data[i][1][1]))
				else:
					sys.exit(57)

			else:
				sys.exit(53)
		
		## DPRINT 
		## pokud var tak zjisti jestli je promenna definovana a vytiskni ji na stderr
		## jinak rovnou tiskni hodnotu na stderr
		elif current_instruction == "DPRINT" :

			if all_data[i][1][0] == "var":
	
				promenna = all_data[i][1][1]
				check_frame_function(promenna)
				ramec, promenna = promenna.split("@", 1)
	
				if ramec == "GF":
					try:
						sys.stderr.write(globalni_ramec[promenna][0])
					except:
						sys.exit(56)
	
				if ramec == "LF":
					try:
						sys.stderr.write(lokalni_ramec[promenna][0])
					except:
						sys.exit(56)
	
				if ramec == "TF" :
					try:
						sys.stderr.write(docasny_ramec[promenna][0])
					except:
						sys.exit(56)
	
			else:
				
				sys.stderr.write(all_data[i][1][1])
	
		

		## MOVE -> pokud var tak zjisti jestli je definovana, zjisti jestli je typ a hodnota pripustna 
		## jinak zjisti jen jestli je hodnota pripustna
		## nastav hodnotu a typ do promenne
		elif current_instruction == "MOVE" :
			check_frame_function(all_data[i][1][1])


			if all_data[i][2][0] == "var":

				check_frame_function(all_data[i][2][1])
				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ == "int":
					try:
						hodnota_1 = int(hodnota_1)
					except:
						sys.exit(57)

				elif typ == "string":
					if not isinstance(hodnota_1, str):
						sys.exit(57)

				elif typ == "bool":
					if hodnota_1 != "true" and hodnota_1 != "false":
						sys.exit(57)

				elif typ == "nil":
					if hodnota_1 != "nil":
						sys.exit(57)

				else:
					sys.exit(53)



			elif all_data[i][2][0] == "int":
				try:
					hodnota_1 = int(all_data[i][2][1])
				except:
					sys.exit(57)

				typ = "int"

			elif all_data[i][2][0] == "string":
				try: 
					hodnota_1 = all_data[i][2][1]
				except:
					hodnota_1 = ""

				if not isinstance(hodnota_1, str):
					sys.exit(57)
				typ = "string"

			elif all_data[i][2][0] == "bool":
				if all_data[i][2][1] != "true" and all_data[i][2][1] != "false":
					sys.exit(57)
				hodnota_1 = all_data[i][2][1]
				typ = "bool"

			elif all_data[i][2][0] == "nil":
				if all_data[i][2][1] != "nil":
					sys.exit(57)
				hodnota_1 = all_data[i][2][1]
				typ = "nil"

			else:
				sys.exit(53)

			if hodnota_1 is None:
				sys.exit(56)

			set_var_value(all_data[i][1][1], hodnota_1, typ)


		## INT2CHAR - > pokud promenna tak zjisti jestli je definovana
		## pokud je symb typu var tak zjisti jestli je definovana a vem jeji hodnotu a typ -> zkontroluj jestli sedi
		## jinak zkontroluj jestli hodnota sedi k zadanemu typu
		## pokud je vse v poradku tak preved cislo na znak a uloz jej do zadane promenne
		elif current_instruction == "INT2CHAR" :

			check_frame_function(all_data[i][1][1])
			if all_data[i][2][0] == "int":
				hodnota_1 = all_data[i][2][1]

			elif all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])
				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "int":
					sys.exit(53)

			else:
				sys.exit(53)

			try:
				hodnota_1 = int(hodnota_1)
			except:
				sys.exit(53)

			try:
				hodnota_1 = chr(hodnota_1)
			except:
				sys.exit(58)

			set_var_value(all_data[i][1][1], hodnota_1, "string")
				

		## STRLEN -> zkontroluj jestli je prvni argument definovana promenna
		## pokud je druhy argument promenna tak zkontroluj jestli je definovana, dal zkontroluj jestli sedi hodnota a typ (pouze string)
		## pokud je druhy argument string tak ok
		## spocitej pocet znaku
		## uloz pocet do zadane promenne (prvni argument)
		elif current_instruction == "STRLEN" :
			check_frame_function(all_data[i][1][1])
			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])

				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)

				typ = get_type(typ, ramec)

				if typ != "string":
					sys.exit(53)


			elif (all_data[i][2][0]) == "string":
				try:
					hodnota_1 = all_data[i][2][1]
				except:
					hodnota_1 = ""

			else:
				sys.exit(53)

			hodnota_1 = int(len(hodnota_1))
			set_var_value(all_data[i][1][1], hodnota_1, "int")


		## TYPE -> zkontroluj jestli promenna v prvnim argumentu je definovana
		## zkontroluj jestli sedi typ
		## pokud je promenna tak tak vytahni hodnotu a typ
		## ziskane informace uloz do zadane promenne
	
		elif current_instruction == "TYPE" :
			check_frame_function(all_data[i][1][1])

			if all_data[i][2][0] == "string":
				typ = all_data[i][2][0]

			if all_data[i][2][0] == "int":
				typ = all_data[i][2][0]

			if all_data[i][2][0] == "bool":
				typ = all_data[i][2][0]

			if all_data[i][2][0] == "nil":
				typ = all_data[i][2][0]

			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])

				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)

				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					typ=""

				if hodnota_1 == "string":
					typ = "string"

				elif hodnota_1 == "int":
					typ = "int"

				elif hodnota_1 == "bool":
					typ = "bool"

				elif hodnota_1 == "nil":
					typ = "nil"

				else:
					typ = ''

			set_var_value(all_data[i][1][1], typ, "string")


		## NOT -> zjisti jestli je promenna v prvnim argumentu definovana
		## pokud je druhy argument promenna tak zjisti hodnotu a typ (a zkontroluj jestli jsou tyto informace pripustne)
		## zneguj hodnotu a uloz ji do promenne
		elif current_instruction == "NOT" :
			check_frame_function(all_data[i][1][1])

			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])

				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "bool":
					sys.exit(53)

				if hodnota_1 != "true" and hodnota_1 != "false":
					sys.exit(57)

			elif all_data[i][2][0] == "bool" :
				hodnota_1 = all_data[i][2][1]
				if hodnota_1 != "true" and hodnota_1 != "false":
					sys.exit(57)
			else:
				sys.exit(53)
			
			if hodnota_1 == "true":
				hodnota_1 = "false"
			else:
				hodnota_1 = "true"
			set_var_value(all_data[i][1][1], hodnota_1, "bool")


		## READ -> zjisti z kama brat informace (bud stdin nebo soubor)
		## pokud ze souboru tak : zjisti jestli je promenna do ktere se bude prirazovat definovana
		## zjisti jakeho je typu je zadana hodnota 
		## zkus jestli k typu sedi hodnota (pokud ne tak implicitne nastavuj podle zadani)
		## nastav hodnotu do promenne
		elif current_instruction == "READ" :
			try:
				input_list
				nastaven = 0
			except:
				nastaven = 1

			if nastaven == 0:
				check_frame_function(all_data[i][1][1])
				if all_data[i][2][1] == "string":
					typ = "string"

				elif all_data[i][2][1] == "int":
					typ = "int"

				elif all_data[i][2][1] == "bool":
					typ = "bool"

				else:
					sys.exit(53)


				if typ == "int":
						try:
							hodnota_1 = int(input_list[pocet_instrukci_read][0])
						except:
							hodnota_1 = 0

				elif typ == "string":
					if not isinstance(input_list[pocet_instrukci_read][0], str):
						hodnota_1 = ""						
					else:
						hodnota_1 = input_list[pocet_instrukci_read][0]

				elif typ == "bool":
					input_list[pocet_instrukci_read][0] = input_list[pocet_instrukci_read][0].lower()
					if input_list[pocet_instrukci_read][0] == "true":
						hodnota_1 = input_list[pocet_instrukci_read][0]
					else:
						hodnota_1 = "false"
				else:
					sys.exit(53)

				pocet_instrukci_read = pocet_instrukci_read + 1

				set_var_value(all_data[i][1][1], hodnota_1, typ)


			else:
				check_frame_function(all_data[i][1][1])
				if all_data[i][2][1] == "string":
					typ = "string"

				elif all_data[i][2][1] == "int":
					typ = "int"

				elif all_data[i][2][1] == "bool":
					typ = "bool"

				else:
					sys.exit(53)

				try:
					vstup = input()

					if typ == "int":
						try:
							hodnota_1 = int(vstup)
						except:
							hodnota_1 = 0

					elif typ == "string":
						if not isinstance(vstup, str):
							hodnota_1 = ""
						else:
							hodnota_1 = vstup

					elif typ == "bool":
						vstup = vstup.lower()
						if vstup == "true":
							hodnota_1 = vstup
						else:
							hodnota_1 = "false"
					else:
						sys.exit(53)

				except:
					if typ == "int":
						hodnota_1 = 0

					elif typ == "string":
						hodnota_1 = ""

					elif typ == "bool":
						hodnota_1 = "false"

					else:
						sys.exit(52)

				set_var_value(all_data[i][1][1], hodnota_1, typ)

		## JUMPIFEQ -> zkontruluj jestli je prvni argument definovane navesti
		## najdi typ druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## porovnej hodnoty a typy prvniho a druheho argumentu -> pokud je vse ok(stejne typy a stejne hodnoty) tak skoc na zadane navesti
		elif current_instruction == "JUMPIFEQ" :
			if all_data[i][1][1] in navesti:

				if all_data[i][2][0] == "var":
					check_frame_function(all_data[i][2][1])

					hledane = all_data[i][2][1]
					ramec, hledane = hledane.split("@", 1)
		
					hodnota_1 = get_value(hledane, ramec)

					if hodnota_1 is None:
						sys.exit(56)

					typ_1 = all_data[i][2][1]
					ramec, typ_1 = typ_1.split("@", 1)
		
					typ_1 = get_type(typ_1, ramec)
					
					
					if typ_1 != "int" and typ_1 != "string" and typ_1 != "bool" and typ_1 != "nil":
						sys.exit(53)


					if typ_1 == "int":
						try:
							int(hodnota_1)
						except:
							sys.exit(57)

					elif typ_1 == "string":
						if not isinstance(hodnota_1, str):
							sys.exit(57)

					elif typ_1 == "bool":
						if hodnota_1 != "true" and hodnota_1 != "false":
							sys.exit(57)

					else:
						if hodnota_1 != "nil":
							sys.exit(57)

				elif all_data[i][2][0] == "int":
					try:
						int(all_data[i][2][1])
					except:
						sys.exit(57)

					hodnota_1 = all_data[i][2][1]
					typ_1 = "int"

				elif all_data[i][2][0] == "string":
					if not isinstance(all_data[i][2][1], str):
						sys.exit(57)
					hodnota_1 = all_data[i][2][1]
					typ_1 = "string"


				elif all_data[i][2][0] == "bool":
					if all_data[i][2][1] != "true" and all_data[i][2][1] != "false":
						sys.exit(57)

					hodnota_1 = all_data[i][2][1]
					typ_1 = "bool"


				elif all_data[i][2][0] == "nil":
					if all_data[i][2][1] != "nil":
						sys.exit(57)
					hodnota_1 = all_data[i][2][1]
					typ_1 = "nil"

				else:
					sys.exit(53)

				if all_data[i][3][0] == "var":
					check_frame_function(all_data[i][3][1])

					hledane = all_data[i][3][1]
					ramec, hledane = hledane.split("@", 1)
		
					hodnota_2 = get_value(hledane, ramec)

					if hodnota_2 is None:
						sys.exit(56)

					typ_2 = all_data[i][3][1]
					ramec, typ_2 = typ_2.split("@", 1)
		
					typ_2 = get_type(typ_2, ramec)
					

					if typ_2 != "int" and typ_2 != "string" and typ_2 != "bool" and typ_2 != "nil":
						sys.exit(53)

					if typ_2 == "int":
						try:
							int(hodnota_2)
						except:
							sys.exit(57)

					elif typ_2 == "string":
						if not isinstance(hodnota_2, str):
							sys.exit(57)

					elif typ_2 == "bool":
						if hodnota_2 != "true" and hodnota_2 != "false":
							sys.exit(57)

					else:
						if hodnota_2 != "nil":
							sys.exit(57)

				elif all_data[i][3][0] == "int":
					try:
						int(all_data[i][3][1])
					except:
						sys.exit(57)

					hodnota_2 = all_data[i][3][1]
					typ_2 = "int"

				elif all_data[i][3][0] == "string":
					if not isinstance(all_data[i][3][1], str):
						sys.exit(57)

					hodnota_2 = all_data[i][3][1]
					typ_2 = "string"


				elif all_data[i][3][0] == "bool":
					if all_data[i][3][1] != "true" and all_data[i][3][1] != "false":
						sys.exit(57)
					hodnota_2 = all_data[i][3][1]
					typ_2 = "bool"


				elif all_data[i][3][0] == "nil":
					if all_data[i][3][1] != "nil":
						sys.exit(57)
					hodnota_2 = all_data[i][3][1]
					typ_2 = "nil"

				if typ_1 == typ_2:
					#print(all_data[i][2][1], all_data[i][3][1])
					if hodnota_1 == hodnota_2:
						#print(len(all_data))
						for z in range(len(all_data)):
							if len(all_data[z]) > 2:
								if all_data[z][1][1] == all_data[i][1][1] and all_data[z][1][1] in navesti:
									skok = z+1
						#print(skok)
						#print(all_data[skok:])
						interpretace(all_data[skok:])
						break
					else:
						continue

				else:

					sys.exit(53)


				
			else:
				sys.exit(52)
		
		## JUMPIFEQ -> zkontruluj jestli je prvni argument definovane navesti
		## najdi typ druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## porovnej hodnoty a typy prvniho a druheho argumentu -> pokud je vse ok(stejne typy a rozdilne hodnoty) tak skoc na zadane navesti
		elif current_instruction == "JUMPIFNEQ" :
			if all_data[i][1][1] in navesti:

				if all_data[i][2][0] == "var":
					check_frame_function(all_data[i][2][1])

					hledane = all_data[i][2][1]
					ramec, hledane = hledane.split("@", 1)
		
					hodnota_1 = get_value(hledane, ramec)

					if hodnota_1 is None:
						sys.exit(56)

					typ_1 = all_data[i][2][1]
					ramec, typ_1 = typ_1.split("@", 1)
		
					typ_1 = get_type(typ_1, ramec)
					

					if typ_1 != "int" and typ_1 != "string" and typ_1 != "bool" and typ_1 != "nil":
						sys.exit(53)

					if typ_1 == "int":
						try:
							int(hodnota_1)
						except:
							sys.exit(57)

					elif typ_1 == "string":
						if not isinstance(hodnota_1, str):
							sys.exit(57)

					elif typ_1 == "bool":
						if hodnota_1 != "true" and hodnota_1 != "false":
							sys.exit(57)

					else:
						if hodnota_1 != "nil":
							sys.exit(57)

				elif all_data[i][2][0] == "int":
					try:
						int(all_data[i][2][1])
					except:
						sys.exit(57)

					hodnota_1 = all_data[i][2][1]
					typ_1 = "int"

				elif all_data[i][2][0] == "string":
					if not isinstance(all_data[i][2][1], str):
						sys.exit(57)
					hodnota_1 = all_data[i][2][1]
					typ_1 = "string"


				elif all_data[i][2][0] == "bool":
					if all_data[i][2][1] != "true" and all_data[i][2][1] != "false":
						sys.exit(57)

					hodnota_1 = all_data[i][2][1]
					typ_1 = "bool"


				elif all_data[i][2][0] == "nil":
					if all_data[i][2][1] != "nil":
						sys.exit(57)
					hodnota_1 = all_data[i][2][1]
					typ_1 = "nil"

				else:
					sys.exit(53)

				if all_data[i][3][0] == "var":
					check_frame_function(all_data[i][3][1])

					hledane = all_data[i][3][1]
					ramec, hledane = hledane.split("@", 1)
		
					hodnota_2 = get_value(hledane, ramec)

					if hodnota_2 is None:
						sys.exit(56)

					typ_2 = all_data[i][3][1]
					ramec, typ_2 = typ_2.split("@", 1)
		
					typ_2 = get_type(typ_2, ramec)
					

					if typ_2 != "int" and typ_2 != "string" and typ_2 != "bool" and typ_2 != "nil":
						sys.exit(53)

					if typ_2 == "int":
						try:
							int(hodnota_2)
						except:
							sys.exit(57)

					elif typ_2 == "string":
						if not isinstance(hodnota_2, str):
							sys.exit(57)

					elif typ_2 == "bool":
						if hodnota_2 != "true" and hodnota_2 != "false":
							sys.exit(57)

					else:
						if hodnota_2 != "nil":
							sys.exit(57)

				elif all_data[i][3][0] == "int":
					try:
						int(all_data[i][3][1])
					except:
						sys.exit(57)

					hodnota_2 = all_data[i][3][1]
					typ_2 = "int"

				elif all_data[i][3][0] == "string":
					if not isinstance(all_data[i][3][1], str):
						sys.exit(57)

					hodnota_2 = all_data[i][3][1]
					typ_2 = "string"


				elif all_data[i][3][0] == "bool":
					if all_data[i][3][1] != "true" and all_data[i][3][1] != "false":
						sys.exit(57)
					hodnota_2 = all_data[i][3][1]
					typ_2 = "bool"


				elif all_data[i][3][0] == "nil":
					if all_data[i][3][1] != "nil":
						sys.exit(57)
					hodnota_2 = all_data[i][3][1]
					typ_2 = "nil"

				if typ_1 == typ_2:
					#print(all_data[i][2][1], all_data[i][3][1])
					if hodnota_1 != hodnota_2:
						#print(len(all_data))
						for z in range(len(all_data)):
							if len(all_data[z]) > 2:
								if all_data[z][1][1] == all_data[i][1][1] and all_data[z][1][1] in navesti:
									skok = z+1
						#print(skok)
						#print(all_data[skok:])
						interpretace(all_data[skok:])
						break
					else:
						continue

				else:
					sys.exit(53)


				
			else:
				sys.exit(52)
		
		## ADD -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok tak secti prvvni a druhy argument a sectenou hodnotu nastav do zadane promenne
		elif current_instruction == "ADD" :
			promenna = all_data[i][1][1]
			check_frame_function(promenna)
	
			if all_data[i][2][0] != "var" and all_data[i][2][0] != "int":
				sys.exit(53)
	
			if all_data[i][3][0] != "var" and all_data[i][3][0] != "int":
				sys.exit(53)
			
			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])
				
				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)


				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				#print(typ)
				if typ != "int":
					sys.exit(53)


	
				if hodnota_1 is None:
					sys.exit(56)
			else:
				hodnota_1 = all_data[i][2][1]
			
	
			if all_data[i][3][0] == "var":
				check_frame_function(all_data[i][3][1])
	
				hledane = all_data[i][3][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)
	
				typ = all_data[i][3][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)


				if typ != "int":
					sys.exit(53)


	
				if hodnota_1 is None:
					sys.exit(56)
			else:
				hodnota_2 = all_data[i][3][1]
	
			try:
				int(hodnota_1)
			except:
				sys.exit(57)
	
			try:
				int(hodnota_2)
			except:
				sys.exit(57)
	
			hodnota = int(hodnota_1) + int(hodnota_2)
	
			set_var_value(promenna, hodnota, "int")
	
		
		## SUB -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok tak odecti prvvni a druhy argument a odectenou hodnotu nastav do zadane promenne
		elif current_instruction == "SUB" :
			promenna = all_data[i][1][1]
			check_frame_function(promenna)
	
			if all_data[i][2][0] != "var" and all_data[i][2][0] != "int":
				sys.exit(53)
	
			if all_data[i][3][0] != "var" and all_data[i][3][0] != "int":
				sys.exit(53)
			
			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])
	
				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "int":
					sys.exit(53)
	
				if hodnota_1 is None:
					sys.exit(56)
			else:
				hodnota_1 = all_data[i][2][1]
	
	
			if all_data[i][3][0] == "var":
				check_frame_function(all_data[i][3][1])
	
				hledane = all_data[i][3][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)
	
				typ = all_data[i][3][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "int":
					sys.exit(53)
	
				if hodnota_1 is None:
					sys.exit(56)
			else:
				hodnota_2 = all_data[i][3][1]
	
			try:
				int(hodnota_1)
			except:
				sys.exit(57)
	
			try:
				int(hodnota_2)
			except:
				sys.exit(57)
	
			hodnota = int(hodnota_1) - int(hodnota_2)
	
			set_var_value(promenna, hodnota, "int")
	
		## SUB -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok tak vynasob prvvni a druhy argument a vynasobenou hodnotu nastav do zadane promenne
		elif current_instruction == "MUL" :
			promenna = all_data[i][1][1]
			check_frame_function(promenna)
	
			if all_data[i][2][0] != "var" and all_data[i][2][0] != "int":
				sys.exit(53)
	
			if all_data[i][3][0] != "var" and all_data[i][3][0] != "int":
				sys.exit(53)
			
			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])
	
				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "int":
					sys.exit(53)
	
				if hodnota_1 is None:
					sys.exit(56)
			else:
				hodnota_1 = all_data[i][2][1]
	
	
			if all_data[i][3][0] == "var":
				check_frame_function(all_data[i][3][1])
	
				hledane = all_data[i][3][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)
	
				typ = all_data[i][3][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "int":
					sys.exit(53)
	
				if hodnota_1 is None:
					sys.exit(56)
			else:
				hodnota_2 = all_data[i][3][1]
	
			try:
				int(hodnota_1)
			except:
				sys.exit(57)
	
			try:
				int(hodnota_2)
			except:
				sys.exit(57)
	
			hodnota = int(hodnota_1) * int(hodnota_2)
	
			set_var_value(promenna, hodnota, "int")
		
		## IDIV -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok (a druhy argument neni 0) tak podel prvvni argument druhym a podelenou hodnotu nastav do zadane promenne
		elif current_instruction == "IDIV" :
			promenna = all_data[i][1][1]
			check_frame_function(promenna)
	
			if all_data[i][2][0] != "var" and all_data[i][2][0] != "int":
				sys.exit(53)
	
			if all_data[i][3][0] != "var" and all_data[i][3][0] != "int":
				sys.exit(53)

			
			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])
	
				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "int":
					sys.exit(53)
	
				if hodnota_1 is None:
					sys.exit(56)
			else:
				hodnota_1 = all_data[i][2][1]
	

	
			if all_data[i][3][0] == "var":
				check_frame_function(all_data[i][3][1])

	
				hledane = all_data[i][3][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)
	
				typ = all_data[i][3][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "int":
					sys.exit(53)
	
				if hodnota_1 is None:
					sys.exit(56)
			else:
				hodnota_2 = all_data[i][3][1]

	
			try:
				int(hodnota_1)
			except:
				sys.exit(57)
	
			try:
				int(hodnota_2)
			except:
				sys.exit(57)

			if int(hodnota_2) == 0:
				sys.exit(57)
	
			hodnota = int(hodnota_1) // int(hodnota_2)

			set_var_value(promenna, hodnota, "int")
		
		## LT -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok (stejne typy, pripustne hodnoty) tak zjisti jestli hodnota1 mensi hodnota2 a vysledek uloz do zadane promenne
		elif current_instruction == "LT" :			
			check_frame_function(all_data[i][1][1])

			#print(all_data[i][2][0])

			if all_data[i][2][0] == "int":
				hodnota_1 = all_data[i][2][1]
				typ_1 = "int"
				try:
					int(hodnota_1)
				except:
					sys.exit(57)

			elif all_data[i][2][0] == "bool":
				hodnota_1 = all_data[i][2][1]
				typ_1 = "bool"
				if hodnota_1 != "false" and hodnota_1 != "true":
					sys.exit(57)

			elif all_data[i][2][0] == "string":
				hodnota_1 = all_data[i][2][1]
				typ_1 = "string"
				if not isinstance(hodnota_1, str):
					sys.exit(57)

			elif all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])

				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ_1 = all_data[i][2][1]
				ramec, typ_1 = typ_1.split("@", 1)
	
				typ_1 = get_type(typ_1, ramec)

				if typ_1 != "string" and typ_1 != "int" and typ_1 != "bool":
					sys.exit(53)

				if typ_1 == "string":
					if not isinstance(hodnota_1, str):
						sys.exit(57)

				elif typ_1 == "int":
					try:
						int(hodnota_1)
					except:
						sys.exit(57)
				elif typ_1 == "bool":
					if hodnota_1 != "true" and hodnota_1 != "false":
						sys.exit(57)
				else:
					sys.exit(53)

			else:
				sys.exit(53)



			if all_data[i][3][0] == "int":
				hodnota_2 = all_data[i][3][1]
				typ_2 = "int"
				try:
					int(hodnota_2)
				except:
					sys.exit(57)

			elif all_data[i][3][0] == "bool":
				hodnota_2 = all_data[i][3][1]
				typ_2="bool"
				if hodnota_2 != "true" and hodnota_2 != "false":
					sys.exit(57)

			elif all_data[i][3][0] == "string":
				hodnota_2 = all_data[i][3][1]
				typ_2 = "string"
				if not isinstance(hodnota_2, str):
					sys.exit(57)

			elif all_data[i][3][0] == "var":
				check_frame_function(all_data[i][3][1])

				hledane = all_data[i][3][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)

				typ_2 = all_data[i][3][1]
				ramec, typ_2 = typ_2.split("@", 1)
	
				typ_2 = get_type(typ_2, ramec)

				if typ_2 != "string" and typ_2 != "int" and typ_2 != "bool":
					sys.exit(53)

				if typ_2 == "string":
					if not isinstance(hodnota_2, str):
						sys.exit(57)
				elif typ_2 == "int":
					try:
						int(hodnota_2)
					except:
						sys.exit(57)
				elif typ_2 == "bool":
					if hodnota_2 != "true" and hodnota_2 != "false":
						sys.exit(57)
				else:
					sys.exit(53)

			else:
				sys.exit(53)


			if typ_1 != typ_2:
				sys.exit(53)

			if typ_1 == "int":
				try:
					hodnota_1 = int(hodnota_1)
				except:
					sys.exit(57)

				try:
					hodnota_2 = int(hodnota_2)
				except:
					sys.exit(57)

				vysledek = hodnota_1 < hodnota_2

			elif typ_1 == "string":
				vysledek = hodnota_1 < hodnota_2
			
			elif typ_1 =="bool":
				if hodnota_1 == "true":
					hodnota_1 = True
				else:
					hodnota_1 = False

				if hodnota_2 == "true":
					hodnota_2 = True
				else:
					hodnota_2 = False

				vysledek = hodnota_1 < hodnota_2			

			else:
				sys.exit(53)

			vysledek = str(vysledek).lower()

			set_var_value(all_data[i][1][1], vysledek, "bool")
			
		## GT -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok (stejne typy, pripustne hodnoty) tak zjisti jestli hodnota1 vetsi hodnota2 a vysledek uloz do zadane promenne

		elif current_instruction == "GT" :
			check_frame_function(all_data[i][1][1])

			#print(all_data[i][2][0])

			if all_data[i][2][0] == "int":
				hodnota_1 = all_data[i][2][1]
				typ_1 = "int"
				try:
					int(hodnota_1)
				except:
					sys.exit(57)

			elif all_data[i][2][0] == "bool":
				hodnota_1 = all_data[i][2][1]
				typ_1 = "bool"
				if hodnota_1 != "false" and hodnota_1 != "true":
					sys.exit(57)

			elif all_data[i][2][0] == "string":
				hodnota_1 = all_data[i][2][1]
				typ_1 = "string"
				if not isinstance(hodnota_1, str):
					sys.exit(57)

			elif all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])

				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ_1 = all_data[i][2][1]
				ramec, typ_1 = typ_1.split("@", 1)
				
				typ_1 = get_type(typ_1, ramec)
				
				if typ_1 != "string" and typ_1 != "int" and typ_1 != "bool":					
					sys.exit(53)


				if typ_1 == "string":
					if not isinstance(hodnota_1, str):
						sys.exit(57)

				elif typ_1 == "int":
					try:
						int(hodnota_1)
					except:
						sys.exit(57)
				elif typ_1 == "bool":
					if hodnota_1 != "true" and hodnota_1 != "false":
						sys.exit(57)
				else:

					sys.exit(53)

			else:
				sys.exit(53)


			if all_data[i][3][0] == "int":
				hodnota_2 = all_data[i][3][1]
				typ_2 = "int"
				try:
					int(hodnota_2)
				except:
					sys.exit(57)

			elif all_data[i][3][0] == "bool":
				hodnota_2 = all_data[i][3][1]
				typ_2="bool"
				if hodnota_2 != "true" and hodnota_2 != "false":
					sys.exit(57)

			elif all_data[i][3][0] == "string":
				hodnota_2 = all_data[i][3][1]
				typ_2 = "string"
				if not isinstance(hodnota_2, str):
					sys.exit(57)

			elif all_data[i][3][0] == "var":
				check_frame_function(all_data[i][3][1])

				hledane = all_data[i][3][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)

				typ_2 = all_data[i][3][1]
				ramec, typ_2 = typ_2.split("@", 1)
	
				typ_2 = get_type(typ_2, ramec)

				if typ_2 != "string" and typ_2 != "int" and typ_2 != "bool":
					sys.exit(53)

				if typ_2 == "string":
					if not isinstance(hodnota_2, str):
						sys.exit(57)

				elif typ_2 == "int":
					try:
						int(hodnota_2)
					except:
						sys.exit(57)
				elif typ_2 == "bool":
					if hodnota_2 != "true" and hodnota_2 != "false":
						sys.exit(57)
				else:
					sys.exit(53)

			else:
				sys.exit(53)


			if typ_1 != typ_2:

				sys.exit(53)

			if typ_1 == "int":
				try:
					hodnota_1 = int(hodnota_1)
				except:
					sys.exit(57)

				try:
					hodnota_2 = int(hodnota_2)
				except:
					sys.exit(57)

				vysledek = hodnota_1 > hodnota_2

			elif typ_1 == "string":
				vysledek = hodnota_1 > hodnota_2
			
			elif typ_1 =="bool":
				if hodnota_1 == "true":
					hodnota_1 = True
				else:
					hodnota_1 = False

				if hodnota_2 == "true":
					hodnota_2 = True
				else:
					hodnota_2 = False


				vysledek = hodnota_1 > hodnota_2
			else:
				sys.exit(53)

			vysledek = str(vysledek).lower()

			set_var_value(all_data[i][1][1], vysledek, "bool")

		## EQ -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok (stejne typy (nebo ruzny typ a nil), pripustne hodnoty) tak zjisti jestli hodnota1 se rovna hodnota2 a vysledek uloz do zadane promenne
		elif current_instruction == "EQ" :
			check_frame_function(all_data[i][1][1])

			#print(all_data[i][2][0])
			
			if all_data[i][2][0] == "int":
				hodnota_1 = all_data[i][2][1]
				typ_1 = "int"
				try:
					int(hodnota_1)
				except:
					sys.exit(57)

			elif all_data[i][2][0] == "bool":
				hodnota_1 = all_data[i][2][1]
				typ_1 = "bool"
				if hodnota_1 != "false" and hodnota_1 != "true":
					sys.exit(57)

			elif all_data[i][2][0] == "nil":
				hodnota_1 = all_data[i][2][1]
				typ_1 = "nil"
				if hodnota_1 != "nil":
					sys.exit(57)

			elif all_data[i][2][0] == "string":
				hodnota_1 = all_data[i][2][1]
				typ_1 = "string"
				if not isinstance(hodnota_1, str):
					sys.exit(57)

			elif all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])

				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ_1 = all_data[i][2][1]
				ramec, typ_1 = typ_1.split("@", 1)
	
				typ_1 = get_type(typ_1, ramec)

				if typ_1 != "string" and typ_1 != "int" and typ_1 != "bool" and typ_1 != "nil":
					sys.exit(53)
				

				if typ_1 == "string":
					if not isinstance(hodnota_1, str):
						sys.exit(57)

				elif typ_1 == "int":
					try:
						int(hodnota_1)
					except:
						sys.exit(57)
				elif typ_1 == "bool":
					if hodnota_1 != "true" and hodnota_1 != "false":
						sys.exit(57)

				elif typ_1 == "nil":
					if hodnota_1 != "nil":
						sys.exit(57)
				else:
					sys.exit(53)


			else:
				sys.exit(53)

			if all_data[i][3][0] == "int":
				hodnota_2 = all_data[i][3][1]
				typ_2 = "int"
				try:
					int(hodnota_2)
				except:
					sys.exit(57)

			elif all_data[i][3][0] == "bool":
				hodnota_2 = all_data[i][3][1]
				typ_2="bool"
				if hodnota_2 != "true" and hodnota_2 != "false":
					sys.exit(57)

			elif all_data[i][3][0] == "nil":
				hodnota_2 = all_data[i][3][1]
				typ_2="nil"
				if hodnota_2 != "nil":
					sys.exit(57)

			elif all_data[i][3][0] == "string":
				hodnota_2 = all_data[i][3][1]
				typ_2 = "string"
				if not isinstance(hodnota_2, str):
					sys.exit(57)

			elif all_data[i][3][0] == "var":
				check_frame_function(all_data[i][3][1])

				hledane = all_data[i][3][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)

				typ_2 = all_data[i][3][1]
				ramec, typ_2 = typ_2.split("@", 1)
	
				typ_2 = get_type(typ_2, ramec)

				
				if typ_2 != "string" and typ_2 != "int" and typ_2 != "bool" and typ_2 != "nil":
					sys.exit(53)


				if typ_2 == "string":
					if not isinstance(hodnota_2, str):
						sys.exit(57)
				elif typ_2 == "int":
					try:
						int(hodnota_2)
					except:
						sys.exit(57)
				elif typ_2 == "bool":
					if hodnota_2 != "true" and hodnota_2 != "false":
						sys.exit(57)

				elif typ_2 == "nil":
					if hodnota_2 != "nil":
						sys.exit(57)
				else:
					sys.exit(53)

			else:
				sys.exit(53)


			if typ_1 != "nil" and typ_2 != "nil":
				if typ_1 != typ_2:
					sys.exit(53)

			if typ_1 =="nil":
				if hodnota_1 == "nil" and hodnota_2 == "nil" and typ_1 == "nil" and typ_2 == "nil":
					vysledek = True
				else:
					vysledek = False

			elif typ_2 =="nil":
				if hodnota_1 == "nil" and hodnota_2 == "nil" and typ_1 == "nil" and typ_2 == "nil":
					vysledek = True
				else:
					vysledek = False

			elif typ_1 == "int":
				try:
					hodnota_1 = int(hodnota_1)
				except:
					sys.exit(53)

				try:
					hodnota_2 = int(hodnota_2)
				except:
					sys.exit(53)

				vysledek = hodnota_1 == hodnota_2

			elif typ_1 == "string":
				vysledek = hodnota_1 == hodnota_2
			
			elif typ_1 == "bool":
				if hodnota_1 == "true":
					hodnota_1 = True
				else:
					hodnota_1 = False

				if hodnota_2 == "true":
					hodnota_2 = True
				else:
					hodnota_2 = False

				vysledek = hodnota_1 == hodnota_2

			else:
				sys.exit(53)



			vysledek = str(vysledek).lower()
			set_var_value(all_data[i][1][1], vysledek, "bool")

		## AND -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok (stejne typy, pripustne hodnoty) tak vyhodnot hodnota1 AND hodnota2 a vysledek uloz do zadane promenne
		elif current_instruction == "AND" :
			check_frame_function(all_data[i][1][1])
			
			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])

				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				#print(typ)
				if typ != "bool":
					sys.exit(53)


				if hodnota_1 != "true" and hodnota_1 != "false":
					sys.exit(57)


			elif all_data[i][2][0] == "bool" :
				hodnota_1 = all_data[i][2][1]
				if hodnota_1 != "true" and hodnota_1 != "false":
					sys.exit(57)
			else:
				sys.exit(53)

			if all_data[i][3][0] == "var":
				check_frame_function(all_data[i][3][1])

				hledane = all_data[i][3][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)

				typ = all_data[i][3][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "bool":
					sys.exit(53)

				if hodnota_2 != "true" and hodnota_2 != "false":
					sys.exit(57)

			elif all_data[i][2][0] == "bool" :
				hodnota_2 = all_data[i][3][1]
				if hodnota_2 != "true" and hodnota_2 != "false":
					sys.exit(57)
			else:
				sys.exit(53)

			if hodnota_1 == "true" and hodnota_2 == "true":
				set_var_value(all_data[i][1][1], "true", "bool")
			else:
				set_var_value(all_data[i][1][1], "false", "bool")

		## OR -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok (stejne typy, pripustne hodnoty) tak vyhodnot hodnota1 OR hodnota2 a vysledek uloz do zadane promenne
		elif current_instruction == "OR" :
			check_frame_function(all_data[i][1][1])
			
			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])

				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)


				if typ != "bool":
					sys.exit(53)


				if hodnota_1 != "true" and hodnota_1 != "false":
					sys.exit(57)

			elif all_data[i][2][0] == "bool" :
				hodnota_1 = all_data[i][2][1]
				if hodnota_1 != "true" and hodnota_1 != "false":
					sys.exit(57)
			else:
				sys.exit(53)
			

			if all_data[i][3][0] == "var":

				check_frame_function(all_data[i][3][1])

				hledane = all_data[i][3][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)

				typ = all_data[i][3][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "bool":
					sys.exit(53)

				if hodnota_2 != "true" and hodnota_2 != "false":
					sys.exit(57)

			elif all_data[i][3][0] == "bool" :

				hodnota_2 = all_data[i][3][1]
				if hodnota_2 != "true" and hodnota_2 != "false":
					sys.exit(57)
			else:
				sys.exit(53)


			if hodnota_1 == "true" or hodnota_2 == "true":
				set_var_value(all_data[i][1][1], "true", "bool")
			else:
				set_var_value(all_data[i][1][1], "false", "bool")


		## STRI2INT -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok (sedi typy, pripustne hodnoty) tak pomoci funkce ord() zjisi hodnotu pismene na zadanem indexu a hodnotu uloz do promenne
		elif current_instruction == "STRI2INT" :
			
			check_frame_function(all_data[i][1][1])

			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])

				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "string":
					sys.exit(53)


			elif all_data[i][2][0] == "string":
				hodnota_1 = all_data[i][2][1]
			else:
				sys.exit(53)


			if all_data[i][3][0] == "var":
				check_frame_function(all_data[i][3][1])

				hledane = all_data[i][3][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)

				typ = all_data[i][3][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "int":
					sys.exit(53)

				try:
					hodnota_2 = int(hodnota_2)
				except:
					sys.exit(57)

			elif all_data[i][3][0] == "int":
				hodnota_2 = all_data[i][3][1]
				try:
					hodnota_2 = int(hodnota_2)
				except:
					sys.exit(57)

			else:
				sys.exit(53)

			if(hodnota_2 >= len(hodnota_1)):
				sys.exit(58)

			if(hodnota_2 < 0):
				sys.exit(58)

			hodnota_1 = ord(hodnota_1[hodnota_2])
			
			set_var_value(all_data[i][1][1], hodnota_1, "int")
		
		## CONCAT -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok (sedi typy, pripustne hodnoty) tak spoj dva retezce dohromady a vysledek uloz do promenne
		elif current_instruction == "CONCAT" :

			check_frame_function(all_data[i][1][1])

			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])

				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "string":
					sys.exit(53)

			elif all_data[i][2][0] == "string":
				try:
					hodnota_1 = all_data[i][2][1]
				except:
					hodnota_1 = ""

			else:
				sys.exit(53)

			if all_data[i][3][0] == "var":
				check_frame_function(all_data[i][3][1])

				hledane = all_data[i][3][1]

				ramec, hledane = hledane.split("@", 1)
				
				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)

				typ = all_data[i][3][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "string":
					sys.exit(53)

			elif all_data[i][3][0] == "string":
				try:
					hodnota_2 = all_data[i][3][1]
				except:
					hodnota_2 = ""

			else:
				sys.exit(53)

			hodnota_1 = hodnota_1 + hodnota_2

			set_var_value(all_data[i][1][1], hodnota_1, "string")


		## GETCHAR -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok (sedi typy, pripustne hodnoty) tak do promenne uloz pismeno z argumentu2 na indexu argument3
		elif current_instruction == "GETCHAR" :
			check_frame_function(all_data[i][1][1])

			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])

				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_1 = get_value(hledane, ramec)

				if hodnota_1 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "string":
					sys.exit(53)


			elif all_data[i][2][0] == "string":
				hodnota_1 = all_data[i][2][1]
			else:
				sys.exit(53)


			if all_data[i][3][0] == "var":
				check_frame_function(all_data[i][3][1])

				hledane = all_data[i][3][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)

				typ = all_data[i][3][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "int":
					sys.exit(53)
				try:
					hodnota_2 = int(hodnota_2)
				except:
					sys.exit(57)

			elif all_data[i][3][0] == "int":
				hodnota_2 = all_data[i][3][1]
				try:
					hodnota_2 = int(hodnota_2)
				except:
					sys.exit(57)

			else:
				sys.exit(53)

			if(hodnota_2 >= len(hodnota_1)):
				sys.exit(58)

			if(hodnota_2 < 0):
				sys.exit(58)

			hodnota_1 = hodnota_1[hodnota_2]
			
			set_var_value(all_data[i][1][1], hodnota_1, "string")
		
		## SETCHAR -> zkontruluj jestli je prvni argument definovana promenna
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## najdi typ a hodnotu druheho argumentu (pokud je var tak zjisti jestli je promenna definovana a zjisti jestli jeji hodnota i typ jsou pripustne)
		## pokud je vse ok (sedi typy, pripustne hodnoty) tak vv promenne zmen pismeno na zadanem indexu na pismeno v arg3 a vysledek uloz do stejne promenne
		elif current_instruction == "SETCHAR" :
			
			check_frame_function(all_data[i][1][1])

			hledane = all_data[i][1][1]
			ramec, hledane = hledane.split("@", 1)
		
			hodnota_1 = get_value(hledane, ramec)

			if hodnota_1 is None:
				sys.exit(56)

			typ = all_data[i][1][1]
			ramec, typ = typ.split("@", 1)

			typ = get_type(typ, ramec)

			if typ != "string":
				sys.exit(53)


			if all_data[i][2][0] == "var":
				check_frame_function(all_data[i][2][1])

				hledane = all_data[i][2][1]
				ramec, hledane = hledane.split("@", 1)

				hodnota_2 = get_value(hledane, ramec)

				if hodnota_2 is None:
					sys.exit(56)

				typ = all_data[i][2][1]
				ramec, typ = typ.split("@", 1)

				typ = get_type(typ, ramec)

				if typ != "int":
					sys.exit(53)

				try:
					hodnota_2 = int(hodnota_2)
				except:
					sys.exit(57)

			elif all_data[i][2][0] == "int":
				hodnota_2 = all_data[i][2][1]
				try:
					hodnota_2 = int(hodnota_2)
				except:
					sys.exit(57)

			else:
				sys.exit(53)

			if all_data[i][3][0] == "var":
				check_frame_function(all_data[i][3][1])

				hledane = all_data[i][3][1]
				ramec, hledane = hledane.split("@", 1)
	
				hodnota_3 = get_value(hledane, ramec)
				if hodnota_3 is None:
					sys.exit(56)

				typ = all_data[i][3][1]
				ramec, typ = typ.split("@", 1)
	
				typ = get_type(typ, ramec)

				if typ != "string":
					sys.exit(53)

				if hodnota_3 == "":
					sys.exit(58)


			elif all_data[i][3][0] == "string":
				try:
					hodnota_3 = all_data[i][3][1]
				except:
					sys.exit(58)
			else:
				sys.exit(53)

			hodnota_3 = hodnota_3[0]

			if hodnota_2 >= len(hodnota_1):
				sys.exit(58)

			if hodnota_2 < 0:
				sys.exit(58)

			if hodnota_3 is None:
				sys.exit(58)

			if not hodnota_3:
				sys.exit(58)

			hodnota_1 = list(hodnota_1)
			hodnota_1[hodnota_2] = hodnota_3
			hodnota_1 = ''.join(hodnota_1)
			set_var_value(all_data[i][1][1], hodnota_1, "string")
	
		else:
			sys.exit(32)
	
		k = k+1


## zavolani prnvi interpretace
interpretace(all_data)
#print(globalni_ramec)
