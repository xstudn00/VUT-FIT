#!/usr/bin/env python3

import collections
import itertools

def first_nonrepeating(string):
    '''Funkce vraci prvni neopakujici se znak, pokud takovy znak neexistuje tak vraci None'''
    if type (string) != str:
      return None
    #pokud neni string vrat None
    c = collections.Counter(string)
    for char in string:
      if c[char] == 1:
        return char   
    #funkce spocita stejna pismena ve stringu a vrati prvni pismeno s jednim vyskytem

def combine4(numbers, result):
    '''Funkce vrati vsechny mozne rovnice (kombinace cisel, zavorek a znaminek), jejich vysledek odpovida zadanemu cislu, tohle je asi READONLY zapis'''
    list1=list(itertools.permutations(numbers))
    #vytvoreni vsech moznych kombinaci cisel
    ress=[]
    #pripraveni pole pro konecne vysledky
    for y in list1:
      for expr in expressions(list(y)):
        expr = ''.join(str(x) for x in expr)
        bla=('{}'.format(expr))
        new_list=[]
        new_list.append(bla)
        #pro vsechny kombinace cisel vytvorim vsechny kombinace znamenek
        for x in new_list:
            new_var=(parenthesize(x))
            #kombinace vsech moznych uzavorkovani
            for idk in list(new_var):
                try:
                    another=eval(idk)
                    if another == result:
                        ress.append(idk)
                        #do pole ress ukladam vsechny rovnice ktere odpovidaji vysledku
                except ZeroDivisionError:
                    pass
    return unique(ress)
    #vracim unikatni rovnice

 
def unique(list1):    
    '''Funkce vrati vsechnz unikatni hodnoty'''

    unique_list = []
    #prazdne pole
     
    for x in list1:
        # ukladam vsechna reseni ktere se jeste neobjevili v novem poli
        if x not in unique_list:
            unique_list.append(x)
    
    return unique_list    
    

def parenthesize(string):
    '''Funkce pro vytvoreni vsech moznych uzavorkovani'''
    operators = ['+','-','*','/']
    depth = len([s for s in string if s in operators]) 
    #pocet znamenek ve stringu
    if depth == 0:
        return [string]
    if depth== 1:
        return ['('+ string + ')']
    #podminky pro ukonceni rekurze

    answer = []
    for index, symbol in enumerate(string):
        if symbol in operators:
            left = string[:index]
            right = string[(index+1):]
            #indexy kam maji byt umisteny zavorky

            strings = [lt  + symbol + rt 
                           for lt in parenthesize(left) 
                           for rt in parenthesize(right) ]
            answer.extend(strings)
    return list(answer) 
    #vracim vsechny zavorky     



operators = ['+', '*', '-', '/']
def expressions(values):
    '''Funkce pro vytvoreni vsech moznych kombinaci znamenek'''
    if len(values) == 1:
        yield values
        
    for i in range(len(values)):

        forward = values[:]
        val = forward.pop(i)

        
        for op in operators:
            for rest in expressions(forward):
                yield [val, op] + rest
