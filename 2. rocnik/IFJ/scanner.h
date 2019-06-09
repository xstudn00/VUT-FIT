/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  ->  scanner.h                                           |
 *|                                                                 |
 *| Autori  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/

#ifndef _SCANNER_H
#define _SCANNER_H

#include <stdlib.h>
#include <stdio.h>

#include "errors.h"
#include "token_struct.h"

#define SCANNER_START 200 
#define SCANNER_LINE_COMMENT 201 

#define SCANNER_IDENTIFIER_OR_KEYWORD 202
#define SCANNER_QUESTION_EXCLAMATION 203


#define SCANNER_NUMBER 204
#define SCANNER_NUMBER_POINT 205
#define SCANNER_NUMBER_DOUBLE 206
#define SCANNER_NUMBER_EXPONENT 207
#define SCANNER_NUMBER_EXPONENT_SIGN 208
#define SCANNER_NUMBER_EXPONENT_LAST 209

#define SCANNER_STRING 210
#define SCANNER_STRING_ESCAPE 211 

#define SCANNER_LESS_THAN 212
#define SCANNER_MORE_THAN 213
#define SCANNER_EOL 214
#define SCANNER_EQ_ASSIGN 215 
#define SCANNER_NEQ 216

#define SCANNER_BLOCK_EQB 218
#define SCANNER_BLOCK_EQBE 219
#define SCANNER_BLOCK_EQBEG 220
#define SCANNER_BLOCK_EQBEGI 221
#define SCANNER_BLOCK_EQBEGIN 222
#define SCANNER_BLOCK_EQBEGIN_SPACE 223
#define SCANNER_BLOCK_INSIDE 224
#define SCANNER_BLOCK_INSIDE_OR_END 225
#define SCANNER_BLOCK_EQE 226
#define SCANNER_BLOCK_EQEN 227
#define SCANNER_BLOCK_EQEND 228
#define SCANNER_BLOCK_EQEND_MORE 229


int get_new_token(Token* token);
void error_code_exit(int);

#endif
