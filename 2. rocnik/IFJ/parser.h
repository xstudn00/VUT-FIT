/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  ->  parser.h                                            |
 *|                                                                 |
 *| Autori  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/


#ifndef _PARSER_H
#define _PARSER_H

#include "token_struct.h"
#include "symtable.h"
#include "code_generator.h"

tList* instr_list; //list instrukcí sdílený mei parserem a psa

extern tInstrOperand operand1; //globální proměnné pro operandy
extern tInstrOperand operand2; //globální proměnné pro operandy
extern tInstrOperand operand3; //globální proměnné pro operandy

void set_instr_list(tList* list);
int print_params(int in_foo, int bracket);
int built_in_foo(char* name, int in_foo);
int expr(Token* token_buffer, char* tmp_string, int in_foo, char* var_name);
int parse();
int prog();
int code();
int stat_list();
int stat_list_else();
int stat_list_end(int in_foo);
int stat();
int assign_expr(int found, char* tmp_stringn, int in_foo);
int params(int *params_count);
int next_param(int *params_count);
int item();

#endif
