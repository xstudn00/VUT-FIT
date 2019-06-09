/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  ->  main.c                                              |
 *|                                                                 |
 *| Autori  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "parser.h"
#include  "code_generator.h"



int main(){
	tList *instr_list;
	string* str ;
	str = (string*)malloc(sizeof(string));

	instr_list = (tList*)malloc(sizeof(tList));

	strInit(str);
	set_global_string(str);
	InitList(instr_list);

	set_instr_list(instr_list);

    parse();   
    generate_code(instr_list);
    add_code("POPFRAME\n");
    print_global_string(str);

    free(instr_list);
    free(str);

    return 0;

}
