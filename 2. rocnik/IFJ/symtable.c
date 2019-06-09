/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  ->  symtable.c                                          |
 *|                                                                 |
 *| Autori  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/



#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "symtable.h"
#include "scanner.h"

/*-----------------	Funkce binarniho stromu----------------------*/
void bst_init(tBSTNodePtr *root_ptr){
	*root_ptr = NULL;
}

//vrati nalezeny pointer na item ; pokud neni nalezeny vraci NULL
tBSTNodePtr bst_search(tBSTNodePtr root_ptr, char* K){
	if(root_ptr == NULL){
		return NULL;
	}
	else{
		if(strcmp(root_ptr->Key, K) > 0){
			return bst_search(root_ptr->rptr, K);
		}
		else if(strcmp(root_ptr->Key, K) < 0){
			return bst_search(root_ptr->lptr, K);
		}
		else{
			return root_ptr;
		}
	}
}

//pokud selze malloc, vaci INTERN_ERROR ; jinak vrati uspech -> 0
int bst_insert(tBSTNodePtr* root_ptr, char* K, void* Data, tNodeType node_type){
	if((*root_ptr) != NULL && root_ptr != NULL){
		if(strcmp((*root_ptr)->Key, K) == 0){
			(*root_ptr)->Data = Data;
			return 0;
		}
		else{
			if(strcmp((*root_ptr)->Key, K) > 0){
				bst_insert(&((*root_ptr)->rptr), K, Data, node_type);
				return 0;
			}
			else if(strcmp((*root_ptr)->Key, K) < 0){
				bst_insert(&((*root_ptr)->lptr), K, Data, node_type);
				return 0;
			}
		}
	}
	else{
		tBSTNodePtr item;
		item = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

		if(item == NULL){
			return INTERN_ERROR;
		}
		else{
			item->Key = K;
			item->Data = Data;
			item->lptr = NULL;
			item->rptr = NULL;
			item->node_type = node_type;			
			(*root_ptr) = item;

			return 0;
		}

		return 0;
	}
	return 0;
}

void replace_by_right_most(tBSTNodePtr *root_ptr, tBSTNodePtr ptr_replaced){
	if((*root_ptr)->rptr != NULL){
		replace_by_right_most(&((*root_ptr)->rptr), ptr_replaced);
	}
	else{
		ptr_replaced->Data = (*root_ptr)->Data;
		ptr_replaced->Key = (*root_ptr)->Key;

		tBSTNodePtr deleted_item = *root_ptr;
		*root_ptr = (*root_ptr)->lptr;
		free(deleted_item);
	}
}

void bst_delete(tBSTNodePtr *root_ptr, char* K){

	if((*root_ptr) && root_ptr){
		if(strcmp((*root_ptr)->Key, K) > 0){
			bst_delete(&((*root_ptr)->rptr), K);
		}
		else if(strcmp((*root_ptr)->Key, K) < 0){
			bst_delete(&((*root_ptr)->lptr), K);
		}
		else{
			if(((*root_ptr)->lptr == NULL) && ((*root_ptr)->rptr == NULL)){
				//free((*root_ptr)->Key);
				free((*root_ptr)->Data);
				free(*root_ptr);
				*root_ptr = NULL;
			}
			else if(((*root_ptr)->lptr != NULL) && ((*root_ptr)->rptr == NULL)){
				//free((*root_ptr)->Key);
				free((*root_ptr)->Data);
				free(*root_ptr);
				*root_ptr = (*root_ptr)->lptr;
			}
			else if(((*root_ptr)->lptr == NULL) && ((*root_ptr)->rptr != NULL)){
				//free((*root_ptr)->Key);
				free((*root_ptr)->Data);
				free(*root_ptr);
				*root_ptr = (*root_ptr)->rptr;
			}
			else{
				replace_by_right_most(root_ptr, (*root_ptr)->lptr);
			}
		}
	}
}

int bst_dispose(tBSTNodePtr *root_ptr){
	if((*root_ptr) == NULL){
		return INTERN_ERROR;
	}
	bst_dispose(&(*root_ptr)->lptr);
	bst_dispose(&(*root_ptr)->rptr);

	free((*root_ptr)->Key);
	(*root_ptr)->Key = NULL;
	
	if((*root_ptr)->node_type == node_type_func){
		tDataFunc* data;
		data = (*root_ptr)->Data;

		strClear(&(data->params));
		strFree(&(data->params));
		data->params.str = NULL;		
	}

	free((*root_ptr)->Data);
	(*root_ptr)->Data = NULL;

	free(*root_ptr);
	*root_ptr = NULL;

	return 0;	
}

/*-----------------------funkce symtable------------------------------*/

void init_symtable(tSymtable* Table){
	bst_init(&(Table->root));
}

int insert_func_symtable(tSymtable* Table, char* Key){
	tDataFunc * dataPointer;
	dataPointer = (tDataFunc*) malloc(sizeof(tDataFunc));

	if(dataPointer == NULL){
		return INTERN_ERROR;
	}

	string params;
	if(strInit(&params)) return INTERN_ERROR;
	dataPointer->returnType = node_dt_default;
	dataPointer->params = params;
	bst_insert(&(Table->root), Key, dataPointer, node_type_func);

	return 0;
}


int insert_var_symtable(tSymtable* Table, char* Key){
	tDataVar* dataPointer;
	dataPointer = (tDataVar*)malloc(sizeof(tDataVar));

	if(dataPointer == NULL){
		return INTERN_ERROR;
	}

	dataPointer->dataType = node_dt_default;

	bst_insert(&(Table->root), Key, dataPointer, node_type_var);

	return 0;
}

int insert_var_data(tSymtable* Table,char* Key,tDataType dataType){
	tBSTNodePtr node;
	tDataVar* data;

	if((node = search_symtable(Table,Key)) == NULL){
		return 1;
	}

	data = (tDataVar*)node->Data;
	data->dataType = dataType;

	return 0;

	
}

int insert_func_data(tSymtable* Table,char* Key,tDataType returnType,int param_count){
	tBSTNodePtr node;
	tDataFunc* data;

	if((node = search_symtable(Table,Key)) == NULL){
		return 1;
	}

	data = (tDataFunc*)node->Data;
	data->returnType = returnType;
	data->n_params = param_count;

	return 0;
}

tBSTNodePtr search_symtable(tSymtable* Table, char* Key){
	return bst_search(Table->root, Key);
}

void delete_symtable(tSymtable* Table, char* Key){
	bst_delete(&(Table->root), Key);
}

void dispose_symtable(tSymtable* Table){
	bst_dispose(&(Table->root));
}
