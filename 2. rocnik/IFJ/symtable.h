/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  ->  symtable.h                                          |
 *|                                                                 |
 *| Autori  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/
#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

#include "str.h" 

typedef enum{
	node_type_var,
	node_type_func,

} tNodeType;

typedef enum{
	node_dt_int,
	node_dt_double,
	node_dt_string,
	node_dt_nil,
	node_dt_default,
} tDataType;

typedef struct tBSTNode{
	char* Key;
	tNodeType node_type; //node->node_type == 0 --> tDataVar* data; data = (tDataVar* )node->Data
	void* Data;
	struct tBSTNode * lptr;
	struct tBSTNode * rptr;
} *tBSTNodePtr;

void bst_init(tBSTNodePtr *);
tBSTNodePtr bst_search(tBSTNodePtr, char*);
int bst_insert(tBSTNodePtr *, char*, void*, tNodeType);
void replace_by_right_most(tBSTNodePtr *, tBSTNodePtr);
void bst_delete(tBSTNodePtr *, char*);
int bst_dispose(tBSTNodePtr *);

/* ----------------------------------------SYMTABLE-----------------------------------------------*/


typedef struct var{
	tDataType dataType;
} tDataVar;

typedef struct function{
	tDataType returnType;
	string params; 
	int n_params;
} tDataFunc;

typedef struct symtable{
	tBSTNodePtr root;
} tSymtable;


void init_symtable(tSymtable*);

int insert_var_symtable(tSymtable*, char*);
int insert_func_symtable(tSymtable*, char*);
int insert_var_data(tSymtable*, char*, tDataType);
tBSTNodePtr search_symtable(tSymtable*, char*);
int insert_func_data(tSymtable*,char*,tDataType,int);
void delete_symtable(tSymtable*, char*);
void dispose_symtable(tSymtable*);


#endif
