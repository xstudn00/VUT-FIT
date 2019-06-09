/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  ->  psa.h                                               |
 *|                                                                 |
 *| Autori  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/

#ifndef _PSA_H
#define _PSA_H

#include "symtable.h"
#include "code_generator.h"


extern tList* instr_list;


typedef enum {
    PSA_STAR,//0
    PSA_SLASH,//1
    PSA_PLUS,//2      
    PSA_MINUS,//3
    PSA_LESS,//4
    PSA_MORE,//5
    PSA_MORE_EQUAL,//6
    PSA_LESS_EQUAL,//7
    PSA_EQUAL,//8
    PSA_NOT_EQUAL,//9
    PSA_OBRACKET,//10
    PSA_CBRACKET,//11
    PSA_ID,//12
    PSA_DOLLAR,//13

    PSA_NONTERMINAL_E,//14
    PSA_NONTERMINAL_BARRIER,//15
    PSA_EXPR_ERROR,//16
    PSA_LEX_ERROR, //17
    PSA_SYNTAX_ERROR, //18
    PSA_SEM_ERROR,  //19
    PSA_ZERO_ERROR, //20
    PSA_NOT_ID, //21
    PSA_BUILT_FOO //22
} PSA_TYPE;

typedef enum{
	TYPE_INT,
	TYPE_DOUBLE,
	TYPE_STRING,
	TYPE_NIL,
	TYPE_NDF
} PSA_DATA_TYPE;

typedef struct tStack_structure{
    struct tStack_structure *right_ptr;
    PSA_TYPE value;
} tStack;

typedef struct{
    tStack *top;
} stack_ptr;

/*
*	Zasobnikova funkce
*	Inicializuje zasobnik
*/
void stack_init(stack_ptr* stack);

/*
*	Zasobnikova funkce
*	odstrani nejvyssi prvek a uvolni pamet
*/
int stack_pop(stack_ptr* stack);

/*
*	Zasobnikova funkce
*	prida na vrchol novy prvek s hodnotou predanou pres parametr
*/
int stack_push(stack_ptr* stack, PSA_TYPE value);

/*
*	Zasobnikova funkce
*	Odstrani vsechny prvky ze zasobniku a uvolni pamet
*/
int stack_dispose(stack_ptr* stack);

/*
*	Zasobnikova funkce
*	Najde terminal nejvysse vrcholu
*/
PSA_TYPE stack_top_terminal(stack_ptr* stack);

/*
*	Zasobnikova funkce
*	Vlozim barieru nad nejvyssi terminal a na vrchol vlozit predanou hodnotu 
*/
int push_with_barrier(stack_ptr* stack, PSA_TYPE value);

/*
*	Zasobnikova funkce
*	Pouziji jedno z pravidel precedencni syntakticke analyzy a upravim nalezite hodnoty na zasobniku
*/
int use_rule(stack_ptr* stack);

/*
*	Funkce podle parametru vyhodnoti typ noveho tokenu a vrati ho
*	Funkce take vraci pres ukazatel input_data v parametru datovy typ tokenu pro typovou kontrolu
*/
PSA_TYPE get_new_input(int dollar_input, int *id_buffer, Token** token_buffer, PSA_DATA_TYPE* input_data, char* tmp_string, char* var_name, int in_foo);

/*
*	funce kontroluje jestli jsou parametry zapsany syntakticky spravne
*	take prohledavame tabulku symbolu, jestli jsou pripadne parametry definovane
*/
int comma_check(Token* token, int option);

/*
*	kontrola spravne syntaxe volani funkce
*	kontrola jestli je volana funkce definovana
*/
int function_check(PSA_TYPE buffer[2]);

/*
*	kontrola jestli input z get_new_input neni chybovy
*	v pripade chyboveho inputu vracime chybu
*/
int input_check(PSA_TYPE input);

/*
*	typova kontrola pomoci tabulky 
*	chybne operace (napriklad scitani stringu a integeru) vraci chybu
*/
int type_check(PSA_TYPE previous_input_data, PSA_TYPE input_data);

/*
*	funkce volana z parseru
*	zkontroluje jestli je vyraz korektne zapsany
*/
int psa(int dollar_input, Token* token_parser,int id_option, tSymtable* psa_sym_table, tSymtable* local_sym_table, char* tmp_string, char* name);

#endif