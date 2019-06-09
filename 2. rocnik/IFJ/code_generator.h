/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  -> code_generator.h                                     |
 *|                                                                 |
 *| Autori  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/


#ifndef _CODE_GENERATOR_H
#define _CODE_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"

/*----------------------ENUMS------------------------------*/
typedef enum{
    FR_TF,
    FR_LF,
    FR_GF,
    FR_NONE,
} frame;

typedef enum{
    dt_string,
    dt_int,
    dt_double,
    dt_nil,
    dt_identificator,
    dt_none,
} tDatatype;

typedef enum{
    input_int,
    input_string,
    input_double,
    input_bool,
    input_none,
}input_type;

typedef enum{
    //Práce s rámci, volání funkcí
    I_MOVE,
    I_CREATEFRAME,
    I_PUSHFRAME,
    I_POPFRAME,
    I_DEFVAR,
    I_CALL,
    I_RETURN,

    //Práce s datovým zásobníkem
    I_PUSHS,
    I_POPS,
    I_CLEARS,
    
    //Aritmetické, relační, booleovské a konverzní instrukce
    I_ADD,
    I_SUB,
    I_MUL,
    I_DIV,
    I_IDIV,
    I_ADDS,
    I_SUBS,
    I_MULS,
    I_DIVS,
    I_IDIVS,
    I_LT,
    I_GT,
    I_EQ,
    I_LTS,
    I_GTS,
    I_EQS,
    I_AND,
    I_OR,
    I_NOT,
    I_ANDS,
    I_ORS,
    I_NOTS,
    I_INT2FLOAT,
    I_FLOAT2INT,
    I_INT2CHAR,
    I_STRI2INT,
    I_INT2FLOATS,
    I_FLOAT2INTS,
    I_INT2CHARS,
    I_STRI2INTS,

    //Vstupně-výstupní instrukce
    I_READ,
    I_WRITE,

    //Práce s řetězci
    I_CONCAT,
    I_STRLEN,
    I_GETCHAR,
    I_SETCHAR,
    
    //Práce s typy
    I_TYPE,

    //Instrukce pro řízení toku programu
    I_LABEL,
    I_JUMP,
    I_JUMPIFEQ,
    I_JUMPIFNEQ,
    I_JUMPIFEQS,
    I_JUMPIFNEQS,
    I_EXIT,

    //Ladicí instrukce
    I_BREAK,
    I_DPRINT,
} instr_type;

/*------------------struktura instrukce-------------------------------*/
typedef struct instr_operand{
    string str;
    tDatatype data_type;
    frame frame;
    int isRetval;
    int isLabel;
    input_type input_type;
}tInstrOperand;

typedef struct {
    instr_type ins_type;
    void* operand1;
    void* operand2;
    void* operand3;
} tInstr;

/*--------------------------struktura listu-------------------------------*/
typedef struct tElem {                 /* prvek dvousměrně vázaného seznamu */ 
    tInstr instruction;         /* užitečná data */
    struct tElem *ptr;        /* ukazatel na následující prvek seznamu */
} *tInstrPtr;

typedef struct {                                  /* dvousměrně vázaný seznam */
    struct tElem* First;                      /* ukazatel na první prvek seznamu */
    struct tElem* Act;                     /* ukazatel na aktuální prvek seznamu */
} tList;


/*----------------------instrukce fci-------------------------------------*/
//Převede znaky z řetězce s hodnotou <33,=35 a =92 na escape sekvenci v podobě \xyz, kde xyz je číslo od 000 do 255
void str_to_ascii(char*,string*);
//Vygeneruje bezoperandovou zadanou instrukci
void write_instr_no_operand(tList *instruction_list, instr_type ins_type);
//Vygeneruje jednooperandovou zadanou instrukci s daty operandu
void write_instr_one_operand(tList *instruction_list, instr_type ins_type, tInstrOperand element1);
//Vygeneruje dvouoperandovou zadanou instrukci s daty operandů
void write_instr_two_operand(tList *instruction_list, instr_type ins_type, tInstrOperand element1, tInstrOperand element2);
//Vygeneruje tříoperandovou zadanou instrukci s daty operandů
void write_instr_three_operand(tList *instruction_list, instr_type ins_type, tInstrOperand element1, tInstrOperand element2,tInstrOperand element3);
tInstrOperand generate_operand(tInstrOperand operand, char* value, tDatatype type, frame frame, int isTMP, int isLabel, input_type input_tp);
void generate_instruction(tList *, instr_type, void *, void *, void *);
void generate_code(tList*);

void set_global_string(string *);
void print_global_string(string *);
void print_header();
int  add_code(char *);

/*-------------------vestavene funkce-----------------------------*/

void print_built_in_functions();

void print_function_inputi();
void print_function_inputf();
void print_function_inputs();
void print_function_print_id(char*);
void print_function_print_const(char*,int);
void print_function_ord();
void print_function_chr();
void print_function_substr();
void print_function_length();
void print_label_main();

/*--------------------dvousmerne vazany seznam funkce----------------------------*/

void Error();
void InitList (tList *);
void DisposeList (tList *);
void First (tList *);
void Copy (tList *, tInstr*);
void Succ (tList *);
int  Active (tList *);
void InsertLast(tList *,tInstr);




tInstrOperand operand1; //globální proměnné pro operandy
tInstrOperand operand2; //globální proměnné pro operandy
tInstrOperand operand3; //globální proměnné pro operandy

#endif