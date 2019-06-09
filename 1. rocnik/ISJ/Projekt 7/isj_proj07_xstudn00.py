#!/usr/bin/env python3

import math

class TooManyCallsError(Exception):
	'''TooManyCallsError trida'''
	def __init__(self, msg):
		'''inicializace'''
		super(TooManyCallsError, self).__init__(msg)

def limit_calls(max_calls=2, error_message_tail='called too often'):
	'''dekorator limit_calls'''
	def dekorator(func):
		func.promenna=0
		def wrapper(*args, **kwargs):
			func.promenna=func.promenna+1
			if func.promenna > max_calls:
				raise TooManyCallsError("function " + '"' + func.__name__ + '"' +" - "+ error_message_tail)
			return func(*args, **kwargs)
		return wrapper
	return dekorator

def ordered_merge (*args, **kwargs):
	'''funkce ordered_merge'''
	selectedBy=kwargs.get('selector', [])
	#zjistim selector
	output=[]
	#pripravim vystup
	args=list(args)
	#prevedu argumentz na list, stejny typ
	numberOfArgs=len(args)
	#spocitam argumenty
	for i in range(numberOfArgs):
		args[i]=iter(args[i])
	for i in selectedBy:
		output.append(next(args[i]))
	return output

class Log():
	'''trida LOG'''
	def __init__(self, file):
		'''inicializace'''
		self.fl=open(file, "w")
	def logging (self, msg):
		'''vypisovani zpravy'''
		self.fl.write(msg+"\n")
	def __enter__(self):
		'''pri vstupu zapis Begin'''
		self.fl.write("Begin\n")
		return self
	def __exit__(self, exc_type, exc_value, traceback):
		'''na konci souboru vypis End'''
		self.fl.write("End\n")
		self.fl.close()

