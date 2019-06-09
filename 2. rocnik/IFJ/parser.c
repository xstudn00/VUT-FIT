/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  ->  parser.c                                            |
 *|                                                                 |
 *| Autori  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/

#include "scanner.h"
#include "error.h"
#include "symtable.h"
#include "psa.h"
#include "parser.h"

#include <string.h>

Token* token;
tSymtable* sym_table;
tSymtable* local_sym_table;
tDataType last_return_type;
char* last_ID;


void set_instr_list(tList* list){
	instr_list = list;
}

int is_func(char* name){

	char last;

 	last = name[(strlen(name)-1)];

 	if (last == '?' || last == '!' )
 		return 1;

 	else
    	return 0;
}

int print_params(int in_foo, int bracket){


	if(token->type == token_type_comma){

		get_new_token(token);

		tDatatype dt;

		if(token->type == token_type_int){

			dt = dt_int;
		}
		else if(token->type == token_type_double){

			dt = dt_double;
		}
		else if(token->type == token_type_string){

			dt = dt_string;
		}
		else if(token->type == token_type_keyword && token->value.keyword == keyword_nil){

			dt = dt_nil;
		}
		else if(token->type == token_type_identifier){

    		tBSTNodePtr node;
       		node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

	    		if (in_foo){
	     		node = search_symtable(local_sym_table, token->value.name);
	    		}				
	    		else{
	    			node = search_symtable(sym_table, token->value.name);
	    		}

	    		if (node == NULL){
	    			return SEMANTIC_ERROR;
	    		}
	    		else{
	    			if(node->node_type == node_type_func){
	    				return SYNTAX_ERROR;
	    			}
	    		}

			dt = dt_identificator;
		}
		else{
			return SYNTAX_ERROR;
		}


		if(dt == dt_identificator){
			operand1 = generate_operand(operand1,token->value.name,dt,FR_LF,0,0,input_none);
		}
		else{
			operand1 = generate_operand(operand1,token->value.string,dt,FR_NONE,0,0,input_none);
		}
		write_instr_one_operand(instr_list,I_WRITE,operand1);

		get_new_token(token);

		int err = print_params(in_foo, bracket);
		return err;

	}
	else if((token->type == token_type_cbracket) && (bracket == 1)){
		get_new_token(token);

		if(token->type == token_type_eol){
			return 0;
		}
		else{
			return SYNTAX_ERROR;
		}

	}
	else if((token->type == token_type_eol) && (bracket == 0)){
		return 0;
	}
	else{
		return SYNTAX_ERROR;
	}

	return SYNTAX_ERROR;
}


int built_in_foo(char* name, int in_foo){
	// a = inputi
	if ( strcmp(name, "inputi") == 0){

		if (last_ID){			
			write_instr_no_operand(instr_list,I_CREATEFRAME);
			operand1 = generate_operand(operand1,"inputi",dt_none,FR_NONE,0,1,input_none);
			write_instr_one_operand(instr_list,I_CALL,operand1);

			operand1 = generate_operand(operand1,last_ID,dt_identificator,FR_LF,0,0,input_none);
			operand2 = generate_operand(operand2,"%retval",dt_identificator,FR_TF,0,0,input_none);
			write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
		}
		else{
			operand1 = generate_operand(operand1,"inputi",dt_none,FR_NONE,0,1,input_none);
			write_instr_one_operand(instr_list,I_CALL,operand1);
		}

		get_new_token(token);

		if (token->type == token_type_eol){
			return 100;	
		}
		else if(token->type == token_type_obracket){
			get_new_token(token);

			if(token->type == token_type_cbracket){
				get_new_token(token);
				if(token->type == token_type_eol){
					return 100;
				}
				else{
					return SYNTAX_ERROR;
				}
			}
			else{
				return SYNTAX_ERROR;
			}
		}
		else{
			return SYNTAX_ERROR;
		}

		return 100;			
	}
	else if( strcmp(name, "inputs") == 0){

		if (last_ID){
			write_instr_no_operand(instr_list,I_CREATEFRAME);
			operand1 = generate_operand(operand1,"inputs",dt_none,FR_NONE,0,1,input_none);
			write_instr_one_operand(instr_list,I_CALL,operand1);

			operand1 = generate_operand(operand1,last_ID,dt_identificator,FR_LF,0,0,input_none);
			operand2 = generate_operand(operand2,"%retval",dt_identificator,FR_TF,0,0,input_none);
			write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
		}
		else{
			operand1 = generate_operand(operand1,"inputs",dt_none,FR_NONE,0,1,input_none);
			write_instr_one_operand(instr_list,I_CALL,operand1);
		}

		get_new_token(token);

		if (token->type == token_type_eol){
			return 102;	
		}
		else if(token->type == token_type_obracket){
			get_new_token(token);

			if(token->type == token_type_cbracket){
				get_new_token(token);
				if(token->type == token_type_eol){
					return 102;
				}
				else{
					return SYNTAX_ERROR;
				}
			}
			else{
				return SYNTAX_ERROR;
			}
		}
		else{
			return SYNTAX_ERROR;
		}

		return 102;
	}
	else if( strcmp(name, "inputf") == 0){

		if (last_ID){
			write_instr_no_operand(instr_list,I_CREATEFRAME);
			operand1 = generate_operand(operand1,"inputf",dt_none,FR_NONE,0,1,input_none);
			write_instr_one_operand(instr_list,I_CALL,operand1);

			operand1 = generate_operand(operand1,last_ID,dt_identificator,FR_LF,0,0,input_none);
			operand2 = generate_operand(operand2,"%retval",dt_identificator,FR_TF,0,0,input_none);
			write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
		}
		else{
			operand1 = generate_operand(operand1,"inputf",dt_none,FR_NONE,0,1,input_none);
			write_instr_one_operand(instr_list,I_CALL,operand1);
		}

		get_new_token(token);

		if (token->type == token_type_eol){
			return 101;	
		}
		else if(token->type == token_type_obracket){
			get_new_token(token);

			if(token->type == token_type_cbracket){
				get_new_token(token);
				if(token->type == token_type_eol){
					return 101;
				}
				else{
					return SYNTAX_ERROR;
				}
			}
			else{
				return SYNTAX_ERROR;
			}
		}
		else{
			return SYNTAX_ERROR;
		}

		return 101;
	}
	else if( strcmp(name, "ord") == 0){
		
		get_new_token(token);
		int bracket = 0;

		if(token->type == token_type_obracket){
			bracket = 1;
			get_new_token(token);
		}

		if(token->type == token_type_string || token->type == token_type_identifier){


			if(token->type == token_type_identifier){

	    		tBSTNodePtr node;
	       		node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

	    		if (in_foo){
	    			node = search_symtable(local_sym_table, token->value.name);
	    		}				
	    		else{
	    			node = search_symtable(sym_table, token->value.name);
	    		}

	    		if (node == NULL){
	    			return SEMANTIC_ERROR;
	    		}
	    		else{
	    			if(node->node_type == node_type_func){
	    				return SYNTAX_ERROR;
	    			}

		       		tDataVar* data;
		       		data = node->Data;

		       		if(data->dataType != node_dt_string){
		       			return SEMANTIC_ERROR;
		       		}
	    		}
	    	}

	    	write_instr_no_operand(instr_list,I_CREATEFRAME);
	    	operand1 = generate_operand(operand1,"%param1",dt_identificator,FR_TF,0,0,input_none);
	    	write_instr_one_operand(instr_list,I_DEFVAR,operand1);

	    	operand1 = generate_operand(operand1,"%param1",dt_identificator,FR_TF,0,0,input_none);
	    	if(token->type == token_type_identifier)
	    		operand2 = generate_operand(operand2,token->value.name,dt_identificator,FR_TF,0,0,input_none);
	    	else
	    		operand2 = generate_operand(operand2,token->value.string,dt_string,FR_NONE,0,0,input_none);

	    	write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);

	    	get_new_token(token);
	    	if (token->type == token_type_comma){


	    		get_new_token(token);

			 	if(token->type == token_type_int || token_type_double || token->type == token_type_identifier){

					if(token->type == token_type_identifier){

			    		tBSTNodePtr node;
			       		node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

			    		if (in_foo){
			    			node = search_symtable(local_sym_table, token->value.name);
			    		}				
			    		else{
			    			node = search_symtable(sym_table, token->value.name);
			    		}

			    		if (node == NULL){
			    			return SEMANTIC_ERROR;
			    		}
			    		else{
			    			if(node->node_type == node_type_func){
			    				return SYNTAX_ERROR;
			    			}

				       		tDataVar* data;
				       		data = node->Data;

				       		if(data->dataType != node_dt_int){
				       			return SEMANTIC_ERROR;
				       		}
			  			}   			
	    			}

			  		operand1 = generate_operand(operand1,"%param2",dt_identificator,FR_TF,0,0,input_none);
			  		write_instr_one_operand(instr_list,I_DEFVAR,operand1);

			  		operand1 = generate_operand(operand1,"%param2",dt_identificator,FR_TF,0,0,input_none);
			  		if(token->type == token_type_identifier)
			  			operand2 = generate_operand(operand2,token->value.name,dt_identificator,FR_TF,0,0,input_none);
			  		else
			  			operand2 = generate_operand(operand2,token->value.string,dt_int,FR_NONE,0,0,input_none);

			  		write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);


			  		operand1 = generate_operand(operand1,"ord",dt_none,FR_NONE,0,1,input_none);
			  		write_instr_one_operand(instr_list,I_CALL,operand1);

			  		operand1 = generate_operand(operand1,"",dt_identificator,FR_NONE,1,0,input_none);
			  		operand2 = generate_operand(operand2,"%retval",dt_identificator,FR_NONE,0,0,input_none);
			  		write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);

			  		if(last_ID){	  			
			    		operand2 = generate_operand(operand2,"",dt_identificator,FR_NONE,1,0,input_none);
			    		operand1 = generate_operand(operand1,last_ID,dt_identificator,FR_LF,0,0,input_none);
			    		write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
			  		}


			  		get_new_token(token);

			  		if (bracket){
			  			if(token->type == token_type_cbracket){
			  				get_new_token(token);
			  				if(token->type != token_type_eol)
			  					return SYNTAX_ERROR;
			  				else{
			  					return 104;
			  				}
			  			}
			  			else{
			  				return SYNTAX_ERROR;
			  			}
			  		}
			  		else{
			  			if(token->type != token_type_eol)
			  				return SYNTAX_ERROR;
			  			else{
			  				return 104;
			  			}			    			
			  		}

	    		}

				else if(token->type == token_type_string || (token->type == token_type_keyword && token->value.keyword == keyword_nil) ){
					return SEMANTIC_ERROR;
				}
	    		else{
	    			return SYNTAX_ERROR;
	    		}

	    	}
	    	else{
	    		return SYNTAX_ERROR;
	    	}

		}
		else if(token->type == token_type_int || token_type_double || (token->type == token_type_keyword && token->value.keyword == keyword_nil) ){
			return SEMANTIC_ERROR;
		}
		else{
			return SYNTAX_ERROR;
		}

		return 104;
	}
	else if( strcmp(name, "chr") == 0){

		get_new_token(token);
		if(token->type == token_type_obracket){

			get_new_token(token);
			if(token->type == token_type_int || token->type == token_type_double || token->type == token_type_identifier ){
	
				tDatatype dt;

				if(token->type == token_type_int){
					dt = dt_int;
				}
				else if(token->type == token_type_double){
					dt = dt_double;
				}
				else{

    				tBSTNodePtr node;
       				node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

	       			if (in_foo){
	        			node = search_symtable(local_sym_table, token->value.name);
	       			}				
	       			else{
	       				node = search_symtable(sym_table, token->value.name);
	       			}

	       			if (node == NULL){
	       				return SEMANTIC_ERROR;
	       			}
	       			else{
	       				if(node->node_type == node_type_func){
	       					return SYNTAX_ERROR;
	       				}
	       			}

					dt = dt_identificator;
				}

				//chr (param)
				write_instr_no_operand(instr_list,I_CREATEFRAME);
				operand1 = generate_operand(operand1,"%param1",dt_identificator,FR_TF,0,0,input_none);
				write_instr_one_operand(instr_list,I_DEFVAR,operand1);
				operand1 = generate_operand(operand1,"%param1",dt_identificator,FR_TF,0,0,input_none);
				if (dt == dt_identificator){
					operand2 = generate_operand(operand2,token->value.name,dt,FR_LF,0,0,input_none);
				}
				else{
					operand2 = generate_operand(operand2,token->value.string,dt,FR_TF,0,0,input_none);
				}
				write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
				operand1 = generate_operand(operand1,"chr",dt,FR_NONE,0,1,input_none);					
				write_instr_one_operand(instr_list,I_CALL,operand1);
				//navrácení hodnoty z funkce				
				operand2 = generate_operand(operand2,"%retval",dt_identificator,FR_TF,0,0,input_none);				
				operand1 = generate_operand(operand1,"",dt_identificator,FR_NONE,1,0,input_none);
				write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);

				if (last_ID){	
					operand1 = generate_operand(operand1,last_ID,dt_identificator,FR_LF,0,0,input_none);				
					operand2 = generate_operand(operand2,"",dt_identificator,FR_NONE,1,0,input_none);
					write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
				}

				get_new_token(token);

				if (token->type == token_type_cbracket){

					get_new_token(token);

					if(token->type == token_type_eol){

						return 102;
					}
					else{
						return SYNTAX_ERROR;
					}

				}
				else if(token->type == token_type_comma){
					get_new_token(token);

					if(token->type == token_type_int || token->type == token_type_double || token->type == token_type_string || token->type == token_type_identifier || (token->type == token_type_keyword && token->value.keyword == keyword_nil) ){
						return SEMANTIC_ERROR;
					}
					else{
						return SYNTAX_ERROR;
					}
				}

				else{
					return SYNTAX_ERROR;
				}
			}

			else if(token->type == token_type_string || (token->type == token_type_keyword && token->value.keyword == keyword_nil) ){
				return SEMANTIC_ERROR;
			}
			else{
				return SYNTAX_ERROR;
			}
		}

		else if(token->type == token_type_int || token->type == token_type_double || token->type == token_type_identifier){

			tDatatype dt;

			if(token->type == token_type_int){
				dt = dt_int;
			}
			else if(token->type == token_type_double){
				dt = dt_double;
			}
			else{

    			tBSTNodePtr node;
       			node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

	       		if (in_foo){
	        		node = search_symtable(local_sym_table, token->value.name);
	       		}				
	       		else{
	       			node = search_symtable(sym_table, token->value.name);
	       		}

	       		if (node == NULL){
	       			return SEMANTIC_ERROR;
	       		}
	       		else{
	       			if(node->node_type == node_type_func){
	       				return SYNTAX_ERROR;
	       			}
	       		}

				dt = dt_identificator;
			}


			//chr param
			write_instr_no_operand(instr_list,I_CREATEFRAME);
			operand1 = generate_operand(operand1,"%param1",dt_identificator,FR_TF,0,0,input_none);
			write_instr_one_operand(instr_list,I_DEFVAR,operand1);
			operand1 = generate_operand(operand1,"%param1",dt_identificator,FR_TF,0,0,input_none);				
			if (dt == dt_identificator){
				operand2 = generate_operand(operand2,token->value.name,dt,FR_LF,0,0,input_none);
			}
			else{
				operand2 = generate_operand(operand2,token->value.string,dt,FR_TF,0,0,input_none);
			}
			write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
			operand1 = generate_operand(operand1,"chr",dt,FR_NONE,0,1,input_none);					
			write_instr_one_operand(instr_list,I_CALL,operand1);
			//navrácení hodnoty z funkce				
			operand2 = generate_operand(operand2,"%retval",dt_identificator,FR_TF,0,0,input_none);				
			operand1 = generate_operand(operand1,"",dt_identificator,FR_NONE,1,0,input_none);
			write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);

			if (last_ID){	
				operand1 = generate_operand(operand1,last_ID,dt_identificator,FR_LF,0,0,input_none);				
				operand2 = generate_operand(operand2,"",dt_identificator,FR_NONE,1,0,input_none);
				write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
			}

			get_new_token(token);

			if (token->type == token_type_eol){

				return 102;

			}
			else if(token->type == token_type_comma){
				get_new_token(token);

				if(token->type == token_type_int || token->type == token_type_double || token->type == token_type_string || token->type == token_type_identifier || (token->type == token_type_keyword && token->value.keyword == keyword_nil) ){
					return SEMANTIC_ERROR;
				}
				else{
					return SYNTAX_ERROR;
				}
			}
			else{
				return SYNTAX_ERROR;
			}
		}
		else if(token->type == token_type_string || (token->type == token_type_keyword && token->value.keyword == keyword_nil) ){
			return SEMANTIC_ERROR;
		}
		else{
			return SYNTAX_ERROR;
		}

		return 102;
	}

	else if( strcmp(name, "substr") == 0){

		get_new_token(token);

		int bracket = 0; 
		if(token->type == token_type_obracket){
			bracket = 1;
			get_new_token(token);
		}

		if(token->type == token_type_string || token->type == token_type_identifier){

			if (token->type == token_type_identifier){
	    		tBSTNodePtr node;
	    		node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

		   		if (in_foo){
		    		node = search_symtable(local_sym_table, token->value.name);
		   		}				
		   		else{
		   			node = search_symtable(sym_table, token->value.name);
		   		}

		   		if (node == NULL){
		   			return SEMANTIC_ERROR;
		   		}
		   		else{
		   			if(node->node_type == node_type_func){
		   				return SYNTAX_ERROR;
		   			}

		   			tDataVar* data;
		   			data = node->Data;

		   			if(data->dataType != node_dt_string){
		   				return SEMANTIC_ERROR;
		   			}
		   		}
			}

			write_instr_no_operand(instr_list,I_CREATEFRAME);
			operand1 = generate_operand(operand1,"%param1",dt_identificator,FR_TF,0,0,input_none);
			write_instr_one_operand(instr_list,I_DEFVAR,operand1);
			operand1 = generate_operand(operand1,"%param1",dt_identificator,FR_TF,0,0,input_none);				
			if (token->type == token_type_identifier){
				operand2 = generate_operand(operand2,token->value.name,dt_identificator,FR_LF,0,0,input_none);
			}
			else{
				operand2 = generate_operand(operand2,token->value.string,dt_string,FR_TF,0,0,input_none);
			}
			write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);


			get_new_token(token);

			if (token->type != token_type_comma){
				return SYNTAX_ERROR;
			}

			get_new_token(token);
			if(token->type == token_type_int || token->type == token_type_double || token->type == token_type_identifier){

				if(token->type == token_type_identifier){

					tBSTNodePtr node;
			   		node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

					if (in_foo){
						node = search_symtable(local_sym_table, token->value.name);
					}				
					else{
						node = search_symtable(sym_table, token->value.name);
					}

					if (node == NULL){
						return SEMANTIC_ERROR;
					}
					else{
						if(node->node_type == node_type_func){
							return SYNTAX_ERROR;
						}

			       		tDataVar* data;
			       		data = node->Data;

			       		if(data->dataType != node_dt_int){
			       			return SEMANTIC_ERROR;
			       		}
					}   			
	    		}

				operand1 = generate_operand(operand1,"%param2",dt_identificator,FR_TF,0,0,input_none);
				write_instr_one_operand(instr_list,I_DEFVAR,operand1);
				operand1 = generate_operand(operand1,"%param2",dt_identificator,FR_TF,0,0,input_none);				
				if(token->type == token_type_identifier){
					operand2 = generate_operand(operand2,token->value.name,dt_identificator,FR_LF,0,0,input_none);
				}
				else if(token->type == token_type_int){
					operand2 = generate_operand(operand2,token->value.string,dt_int,FR_TF,0,0,input_none);
				}
				else{
					operand2 = generate_operand(operand2,token->value.string,dt_double,FR_TF,0,0,input_none);					
				}
				write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);

				get_new_token(token);

	    		if (token->type != token_type_comma){
					return SYNTAX_ERROR;
				}
				get_new_token(token);

				if(token->type == token_type_int || token->type == token_type_double || token->type == token_type_identifier){

					if(token->type == token_type_identifier){

						tBSTNodePtr node;
				   		node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

						if (in_foo){
							node = search_symtable(local_sym_table, token->value.name);
						}				
						else{
							node = search_symtable(sym_table, token->value.name);
						}

						if (node == NULL){
							return SEMANTIC_ERROR;
						}
						else{
							if(node->node_type == node_type_func){
								return SYNTAX_ERROR;
							}

				       		tDataVar* data;
				       		data = node->Data;

				       		if(data->dataType != node_dt_int){
				       			return SEMANTIC_ERROR;
				       		}
						}   			
		    		}


		    		operand1 = generate_operand(operand1,"%param3",dt_identificator,FR_TF,0,0,input_none);
					write_instr_one_operand(instr_list,I_DEFVAR,operand1);
					operand1 = generate_operand(operand1,"%param3",dt_identificator,FR_TF,0,0,input_none);				
					if(token->type == token_type_identifier){
						operand2 = generate_operand(operand2,token->value.name,dt_identificator,FR_LF,0,0,input_none);
					}
					else if(token->type == token_type_int){
						operand2 = generate_operand(operand2,token->value.string,dt_int,FR_TF,0,0,input_none);
					}
					else{
						operand2 = generate_operand(operand2,token->value.string,dt_double,FR_TF,0,0,input_none);					
					}
					write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
					operand1 = generate_operand(operand1,"substr",dt_none,FR_NONE,0,1,input_none);
					write_instr_one_operand(instr_list,I_CALL,operand1);

					operand1 = generate_operand(operand1,"",dt_identificator,FR_NONE,1,0,input_none);
					operand2 = generate_operand(operand2,"%retval",dt_identificator,FR_TF,0,0,input_none);
					write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);

					if (last_ID){	
					operand1 = generate_operand(operand1,last_ID,dt_identificator,FR_LF,0,0,input_none);				
					operand2 = generate_operand(operand2,"",dt_identificator,FR_NONE,1,0,input_none);
					write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
					}	

			  		get_new_token(token);

			  		if (bracket){
			  			if(token->type == token_type_cbracket){
			  				get_new_token(token);
			  				if(token->type != token_type_eol)
			  					return SYNTAX_ERROR;
			  				else{
			  					return 102;
			  				}
			  			}
			  			else{
			  				return SYNTAX_ERROR;
			  			}
			  		}
			  		else{
			  			if(token->type != token_type_eol)
			  				return SYNTAX_ERROR;
			  			else{
			  				return 102;
			  			}			    			
			  		}

				}
				else if(token->type == token_type_string || (token->type == token_type_keyword && token->value.keyword == keyword_nil)){
					return SEMANTIC_ERROR;
				}
				else{
					return SYNTAX_ERROR;
				}

			}
			else if(token->type == token_type_string || (token->type == token_type_keyword && token->value.keyword == keyword_nil)){
				return SEMANTIC_ERROR;
			}
			else{
				return SYNTAX_ERROR;
			}
		}

		else if(token->type == token_type_int || token->type == token_type_double || (token->type == token_type_keyword && token->value.keyword == keyword_nil)){
				return SEMANTIC_ERROR;
		}	
		else{
			return SYNTAX_ERROR;
		}


		///GENERUJ 
		return 102;
	}
	else if( strcmp(name, "length") == 0){
		
		get_new_token(token);

		if(token->type == token_type_obracket){

			get_new_token(token);
			if(token->type == token_type_string || token->type == token_type_identifier){

				if (token->type == token_type_identifier){
	    			tBSTNodePtr node;
	       			node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

		       		if (in_foo){
		        		node = search_symtable(local_sym_table, token->value.name);
		       		}				
		       		else{
		       			node = search_symtable(sym_table, token->value.name);
		       		}

		       		if (node == NULL){
		       			return SEMANTIC_ERROR;
		       		}
		       		else{
		       			if(node->node_type == node_type_func){
		       				return SYNTAX_ERROR;
		       			}

		       			tDataVar* data;
		       			data = node->Data;

		       			if(data->dataType != node_dt_string){
		       				return SEMANTIC_ERROR;
		       			}
		       		}
				}


				write_instr_no_operand(instr_list,I_CREATEFRAME);
				operand1 = generate_operand(operand1,"%param1",dt_identificator,FR_TF,0,0,input_none);
				write_instr_one_operand(instr_list,I_DEFVAR,operand1);
				operand1 = generate_operand(operand1,"%param1",dt_identificator,FR_TF,0,0,input_none);
				if(token->type == token_type_identifier){
					operand2 = generate_operand(operand2,token->value.name,dt_identificator,FR_TF,0,0,input_none);
				}
				else{
					operand2 = generate_operand(operand2,token->value.string,dt_string,FR_TF,0,0,input_none);
				}
				write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
				operand1 = generate_operand(operand1,"length",dt_none,FR_NONE,0,1,input_none);					
				write_instr_one_operand(instr_list,I_CALL,operand1);

				operand2 = generate_operand(operand2,"%retval",dt_identificator,FR_TF,0,0,input_none);				
				operand1 = generate_operand(operand1,"",dt_identificator,FR_NONE,1,0,input_none);
				write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);

				if (last_ID){	
					operand1 = generate_operand(operand1,last_ID,dt_identificator,FR_LF,0,0,input_none);				
					operand2 = generate_operand(operand2,"",dt_identificator,FR_NONE,1,0,input_none);
					write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
				}


				get_new_token(token);
				if (token->type == token_type_cbracket){

					get_new_token(token);

					if(token->type != token_type_eol)
						return SYNTAX_ERROR;
					else{
						return 100;
					}
				}
				else{
					return SYNTAX_ERROR;
				}
			}
			else if(token->type == token_type_int || token->type == token_type_double || (token->type == token_type_keyword && token->value.keyword == keyword_nil)){
				return SEMANTIC_ERROR;
			}
			else{
				return SYNTAX_ERROR;
			}


		}
		else if(token->type == token_type_string || token->type == token_type_identifier){


			if (token->type == token_type_identifier){

    			tBSTNodePtr node;
       			node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

	       		if (in_foo){
	        		node = search_symtable(local_sym_table, token->value.name);
	       		}				
	       		else{
	       			node = search_symtable(sym_table, token->value.name);
	       		}

	       		if (node == NULL){
	       			return SEMANTIC_ERROR;
	       		}
	       		else{
	       			if(node->node_type == node_type_func){
	       				return SYNTAX_ERROR;
	       			}

	       			tDataVar* data;
	       			data = node->Data;

	       			if(data->dataType != node_dt_string){
	       				return SEMANTIC_ERROR;
	       			}
	       		}
			}


			write_instr_no_operand(instr_list,I_CREATEFRAME);
			operand1 = generate_operand(operand1,"%param1",dt_identificator,FR_TF,0,0,input_none);
			write_instr_one_operand(instr_list,I_DEFVAR,operand1);
			operand1 = generate_operand(operand1,"%param1",dt_identificator,FR_TF,0,0,input_none);
			if(token->type == token_type_identifier){
				operand2 = generate_operand(operand2,token->value.name,dt_identificator,FR_TF,0,0,input_none);
			}
			else{
				operand2 = generate_operand(operand2,token->value.string,dt_string,FR_TF,0,0,input_none);
			}
			write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
			operand1 = generate_operand(operand1,"length",dt_none,FR_NONE,0,1,input_none);					
			write_instr_one_operand(instr_list,I_CALL,operand1);

			operand2 = generate_operand(operand2,"%retval",dt_identificator,FR_TF,0,0,input_none);				
			operand1 = generate_operand(operand1,"",dt_identificator,FR_NONE,1,0,input_none);
			write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);

			if (last_ID){	
				operand1 = generate_operand(operand1,last_ID,dt_identificator,FR_LF,0,0,input_none);				
				operand2 = generate_operand(operand2,"",dt_identificator,FR_NONE,1,0,input_none);
				write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
			}

			get_new_token(token);
			if(token->type != token_type_eol){
				return SYNTAX_ERROR;
			}
			else{
				return 100;
			}

		}
		else if(token->type == token_type_int || token->type == token_type_double || (token->type == token_type_keyword && token->value.keyword == keyword_nil)){
			return SEMANTIC_ERROR;
		}
		else{
			return SYNTAX_ERROR;
		}


		return 1;
	}
	else if( strcmp(name, "print") == 0){

		int bracket;

		get_new_token(token);	

		if(token->type == token_type_obracket){
			tDatatype dt;
			bracket = 1;


			get_new_token(token);

			if(token->type == token_type_int){

				dt = dt_int;
			}
			else if(token->type == token_type_double){

				dt = dt_double;
			}
			else if(token->type == token_type_string){

				dt = dt_string;
			}
			else if(token->type == token_type_keyword && token->value.keyword == keyword_nil){

				dt = dt_nil;
			}
			else if(token->type == token_type_identifier){

    			tBSTNodePtr node;
       			node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

	       		if (in_foo){
	        		node = search_symtable(local_sym_table, token->value.name);
	       		}				
	       		else{
	       			node = search_symtable(sym_table, token->value.name);
	       		}

	       		if (node == NULL){
	       			return SEMANTIC_ERROR;
	       		}
	       		else{
	       			if(node->node_type == node_type_func){
	       				return SYNTAX_ERROR;
	       			}
	       		}

				dt = dt_identificator;

			}
			else{
				return SYNTAX_ERROR;
			}


			//generuju a volam print_params()
			if(dt == dt_identificator){
				operand1 = generate_operand(operand1,token->value.name,dt,FR_LF,0,0,input_none);
			}
			else{
				operand1 = generate_operand(operand1,token->value.string,dt,FR_NONE,0,0,input_none);
			}
			write_instr_one_operand(instr_list,I_WRITE,operand1);


			get_new_token(token);

			int err = print_params(in_foo, bracket);
			if(err == 0){
				return 103;
			}
			else
				return err;

		}
		else{
			bracket = 0;

			tDatatype dt;
			
			if(token->type == token_type_int){

				dt = dt_int;
			}
			else if(token->type == token_type_double){

				dt = dt_double;
			}
			else if(token->type == token_type_string){

				dt = dt_string;
			}
			else if(token->type == token_type_keyword && token->value.keyword == keyword_nil){

				dt = dt_nil;
			}
			else if(token->type == token_type_identifier){

    			tBSTNodePtr node;
       			node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

	       		if (in_foo){
	        		node = search_symtable(local_sym_table, token->value.name);
	       		}				
	       		else{
	       			node = search_symtable(sym_table, token->value.name);
	       		}

	       		if (node == NULL){
	       			return SEMANTIC_ERROR;
	       		}
	       		else{
	       			if(node->node_type == node_type_func){
	       				return SYNTAX_ERROR;
	       			}
	       		}

				dt = dt_identificator;
			}
			else{
				return SYNTAX_ERROR;
			}


			//generuju a volam print_params()
			if(dt == dt_identificator){
				operand1 = generate_operand(operand1,token->value.name,dt,FR_LF,0,0,input_none);
			}
			else{
				operand1 = generate_operand(operand1,token->value.string,dt,FR_NONE,0,0,input_none);
			}
			write_instr_one_operand(instr_list,I_WRITE,operand1);

			get_new_token(token);

			int err = print_params(in_foo, bracket);


			if(err == 0){
				return 103;
			}
			else
				return err;
		}

		return 103;
	}
	else //nenasel jsem funkci
		return 0;
}

int expr(Token* token_buffer, char* tmp_string, int in_foo, char* var_name){

	int result;
	
	result = psa(1, token_buffer,0,sym_table,local_sym_table,tmp_string, var_name);		//1 protoze ocekavame koncovy znak EOL
	
	if(result == 0){
		if(in_foo){
			last_return_type = node_dt_nil;
		}
		return 0;
	}
	else if (result < 100){

		if(in_foo){
			dispose_symtable(local_sym_table);
			error_code_exit(result);
		}
		else{
			error_code_exit(result);
		}
	}

	if (in_foo){
		if (result == 100){
			last_return_type = node_dt_int;
		}
		else if (result == 101){
			last_return_type = node_dt_double;
		}
		else if (result == 102){
			last_return_type = node_dt_string;

		}
		else if (result == 103){
			last_return_type = node_dt_nil;
		}
	}
	return 0;
}

// 	int
// 	string
// 	double
// 	nil
int item(){
	if (token->type == token_type_int){
		return 100;		
	}
	else if(token->type == token_type_double){
		return 101;
	}
	else if (token->type == token_type_string){
		return 102;
	}
	else if ((token->type == token_type_keyword) && (token->value.keyword == keyword_nil)){
		return 103;
	}
	else{
		return SYNTAX_ERROR;
	}
}

//	) EOL
// 	, ID <next-param>
int next_param(int *params_count){

	if (token->type == token_type_cbracket){
		get_new_token(token);
		if (token->type == token_type_eol){
		}
		else{
			dispose_symtable(local_sym_table);
			error_code_exit(SYNTAX_ERROR);
		}
	}
	else if (token->type == token_type_comma){
		get_new_token(token);
		if (!(token->type == token_type_identifier)){

			dispose_symtable(local_sym_table);
			error_code_exit(SYNTAX_ERROR);
		}


		insert_var_symtable(local_sym_table, token->value.name);


		(*params_count)++;
		get_new_token(token);
		next_param(params_count);
	}
	else{
		dispose_symtable(local_sym_table);
		error_code_exit(SYNTAX_ERROR);
	}
	return 0;
}

//	= <expr>
// 	<expr>
int assign_expr(int found, char* tmp_string, int in_foo){
	int result;

	if (token->type == token_type_assign){

		char* var_name = token->value.name;

		last_ID = token->value.name;

		if ( is_func(var_name) == 1){
			return SYNTAX_ERROR;
		}

		result = built_in_foo(token->value.name, in_foo);

		if(result >=100){
			error_code_exit( SYNTAX_ERROR);
		}
		else if (result > 0 && result <100){
			error_code_exit(result);
		}
          
        if(in_foo){
        	if(search_symtable(local_sym_table, token->value.name) == NULL){

		        operand1 = generate_operand(operand1,token->value.name,dt_identificator,FR_LF,0,0,input_none);
		        write_instr_one_operand(instr_list,I_DEFVAR,operand1);       		
        	}
        }
        else{
        	if(search_symtable(sym_table, token->value.name) == NULL){

		        operand1 = generate_operand(operand1,token->value.name,dt_identificator,FR_LF,0,0,input_none);
		        write_instr_one_operand(instr_list,I_DEFVAR,operand1);       		
       		}
       	}


		tBSTNodePtr node;
        node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));


		if (tmp_string != NULL){
			if(in_foo){
				if(search_symtable(local_sym_table, tmp_string)  != NULL){


					delete_symtable(local_sym_table, tmp_string);
				}
			}
			else{
				if(search_symtable(sym_table, tmp_string)  != NULL){
					delete_symtable(sym_table, tmp_string);
				}
			}
		}

		if(in_foo){
			node = search_symtable(local_sym_table, var_name);
		}
		else{
			node = search_symtable(sym_table, var_name);		
		}

		if(node == NULL){
			if(in_foo){
				insert_var_symtable(local_sym_table, var_name);
			}
			else{
				insert_var_symtable(sym_table, var_name);
			}
		}
		else{
			if(node->node_type == node_type_func){
					return SYNTAX_ERROR;
			}
		}

		if (in_foo){
			if ((result = psa(1, NULL, 0, sym_table, local_sym_table, NULL, token->value.name)) < 100){	//1 protoze koncovy stav cekame EOL
				dispose_symtable(local_sym_table);
				dispose_symtable(sym_table);
				error_code_exit(result);
			}
		}
		else{
			if ((result = psa(1, NULL, 0, sym_table, local_sym_table, NULL, token->value.name)) < 100){	//1 protoze koncovy stav cekame EOL
				dispose_symtable(sym_table);
				error_code_exit(result);
			}
		}

		last_ID = NULL;

		if (result == 100){

			if (in_foo){
				last_return_type = node_dt_int;
				insert_var_data(local_sym_table, var_name, node_dt_int);
			}
			else{
				insert_var_data(sym_table, var_name, node_dt_int);
			}
		}
		else if (result == 101){
			if (in_foo){
				last_return_type = node_dt_double;
				insert_var_data(local_sym_table, var_name, node_dt_double);
			}
			else{
				insert_var_data(sym_table, var_name, node_dt_double);
			}
		}
		else if (result == 102){
			if (in_foo){
				last_return_type = node_dt_string;
				insert_var_data(local_sym_table, var_name, node_dt_string);
			}
			else{
				insert_var_data(sym_table, var_name, node_dt_string);
			}
		}
		else if (result == 103){
			if (in_foo){
				last_return_type = node_dt_nil;
				insert_var_data(local_sym_table, var_name, node_dt_nil);
			}
			else{
				insert_var_data(sym_table, var_name, node_dt_nil);
			}
		}
			

	}
	else if (token->type == token_type_obracket ||
			 token->type == token_type_plus ||
			 token->type == token_type_minus ||
			 token->type == token_type_multiply ||
			 token->type == token_type_divide){

		Token* token_buffer = malloc(sizeof(Token));
		token_buffer->type = token->type;


		if (found == 0){ 
			if(in_foo){
				dispose_symtable(local_sym_table);
				dispose_symtable(sym_table);
				error_code_exit(SEMANTIC_ERROR);
			}
			else{
				dispose_symtable(sym_table);
				error_code_exit(SEMANTIC_ERROR);
			}
		}


		result = expr(token_buffer, tmp_string, in_foo, token->value.name);
		free(token_buffer);
		return result;
	}
	else if(token->type == token_type_identifier || item() >= 100){	

		if (found == 0){
			return SEMANTIC_ERROR;
		}


		tBSTNodePtr node;
        node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

		if(token->type == token_type_identifier){
			if (in_foo){
				if(search_symtable(local_sym_table, token->value.name) == 0 )
					return SEMANTIC_ERROR;
			}
			else{
				if( (node = search_symtable(sym_table, token->value.name) )== 0 )
					return SEMANTIC_ERROR;

				else {
					if(node->node_type == node_type_func){
						return SYNTAX_ERROR;
					}
				}				
			}

		}

		Token* token_buffer = malloc(sizeof(Token));
		token_buffer->type = token->type;
		token_buffer->value = token->value;

		if (tmp_string != NULL){
			if(in_foo){
				if(search_symtable(local_sym_table, tmp_string)  != NULL){
					delete_symtable(sym_table, tmp_string);
				}
			}
			else{
				if(search_symtable(sym_table, tmp_string)  != NULL){
				delete_symtable(sym_table, tmp_string);
				}
			}
		}

		result = expr(token_buffer, NULL, in_foo, NULL);

		free(token_buffer);
		return result;
	}

	else if(token->type == token_type_eol){
	
		if (in_foo){
			if ( search_symtable(local_sym_table, token->value.name) == NULL){
				return SEMANTIC_ERROR;
			}
		}
		else{
			if ( search_symtable(sym_table, token->value.name) == NULL){
				return SEMANTIC_ERROR;
			}			
		}

		return 0;
	}
	else{
		Token* token_buffer = malloc(sizeof(Token));
		token_buffer->type = token->type;
		token_buffer->value = token->value;

		if(found == 1){
			if(in_foo){
				delete_symtable(local_sym_table, tmp_string);
			}
			else{
				delete_symtable(sym_table, tmp_string);
			}
		}
		else {
			free(token_buffer);
			return SEMANTIC_ERROR;
		}

		if (in_foo){
			if( (result = psa(1, token_buffer, 2, sym_table, local_sym_table, tmp_string, NULL)) < 100){
				dispose_symtable(local_sym_table);
				dispose_symtable(sym_table);
				error_code_exit(result);
			}
		}
		else{
			if( (result = psa(1, token_buffer, 2, sym_table, local_sym_table, tmp_string, NULL)) < 100){
				dispose_symtable(sym_table);
				error_code_exit(result);		
			}
		}
		
		free(token_buffer);
		return 0;
	}
	return 0;
}

// 	ID <assign-expr>
// 	if <expr> EOL <stat-list-else> EOL <stat-list-end> EOL
// 	while <expr> EOL <stat-list-end> EOL
//	( <expr>
//	<item> <expr>
int stat(int in_foo){
	int result;
	
	if (token->type == token_type_identifier){

		result = built_in_foo(token->value.name, in_foo);
        if (result > 0 && result < 100){
        	error_code_exit(result);
        }
        else if (result == 100){
        	last_return_type = node_dt_int;
        	return 0;
        }
        else if (result == 101){
        	last_return_type = node_dt_double;
        	return 0;
        }        
        else if (result == 102){
        	last_return_type = node_dt_string;
        	return 0;        
        }        
        else if (result == 103){
        	last_return_type = node_dt_nil;
        	return 0;
        }
        else if (result == 104){
        	last_return_type = node_dt_default;
        	return 0;        
        }

		tBSTNodePtr node;
        node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));
        if (in_foo){
        	node = search_symtable(local_sym_table, token->value.name);
        }
        else{
        	node = search_symtable(sym_table, token->value.name);
        }

        int found = 0;

        if (node){
        	found = 1;

			char * tmp_string ;
			tmp_string = (char*)malloc(sizeof(char));
			strcpy(tmp_string,"not_a_buffer_kappa");

			if (in_foo){
				insert_var_symtable(local_sym_table, tmp_string);
			}
			else{
				insert_var_symtable(sym_table, tmp_string);
			}

	        tDataVar* data;
	        data = (tDataVar*)node->Data;


	        if(data->dataType == node_dt_int){

	        	if (in_foo){
	            	insert_var_data(local_sym_table, tmp_string, node_dt_int);
	        	}
	        	else{
	            	insert_var_data(sym_table, tmp_string, node_dt_int);
	        	}
	        }
	        else if(data->dataType == node_dt_double){
	            if (in_foo){
	            	insert_var_data(local_sym_table, tmp_string, node_dt_double);            
	            }
	            else{
	            	insert_var_data(sym_table, tmp_string, node_dt_double);           	
	            }
	        }
	        else if(data->dataType == node_dt_string){
	            if (in_foo){
	            	insert_var_data(local_sym_table, tmp_string, node_dt_string);
	            }
	            else{
	            	insert_var_data(sym_table, tmp_string, node_dt_string);	            	
	            }
	        }
	        else if(data->dataType == node_dt_nil){
	            if(in_foo){
	            	insert_var_data(local_sym_table, tmp_string, node_dt_nil);
	            }
		        else{
	            	insert_var_data(sym_table, tmp_string, node_dt_nil);		        	
		        }    
	        }
			else if(data->dataType == node_dt_default){
	            if(in_foo){
	            	insert_var_data(local_sym_table, tmp_string, node_dt_default);
	            }
		        else{
	            	insert_var_data(sym_table, tmp_string, node_dt_default);		        	
		        }    
	        }

			get_new_token(token);
			if ((result = assign_expr(found, tmp_string, in_foo)) !=0 ){
				error_code_exit(result);	
			}

		}
		else{
			get_new_token(token);
			if ((result = assign_expr(found, NULL, in_foo)) !=0 ) error_code_exit(result);	
		}

	}
	// if <expr> EOL <stat-list-else> EOL <stat-list-end> EOL
	else if (token->type == token_type_keyword && token->value.keyword == keyword_if){

		
		result = psa(2, NULL, 0, sym_table, local_sym_table, NULL, NULL);

		if (result < 100) 
			error_code_exit(result);

		get_new_token(token);
		if (token->type == token_type_eol){

			get_new_token(token);
			stat_list_else(in_foo);
			get_new_token(token);

			if (token->type == token_type_eol){

				
				get_new_token(token);
				stat_list_end(in_foo);
				get_new_token(token);

				if (in_foo){
					last_return_type = node_dt_nil;
				}

				if (token->type == token_type_eol){
				}
				else{
					error_code_exit(SYNTAX_ERROR);
				}	
			}
			else{
				error_code_exit(SYNTAX_ERROR);
			}
		}
		else{
			error_code_exit(SYNTAX_ERROR);
		}
	}
	// 	while <expr> EOL <stat-list-end> EOL
	else if(token->type == token_type_keyword && token->value.keyword == keyword_while){

		result = psa(3, NULL, 0, sym_table, local_sym_table, NULL, NULL);

		if (result < 100) //3 protoze ocekavame koncovy znak do 
			error_code_exit(result);	

		get_new_token(token);

		if (token->type == token_type_eol){

			get_new_token(token);
			stat_list_end(in_foo);
			get_new_token(token);

			last_return_type = node_dt_nil;

			if (token->type == token_type_eol){
			}
			else{
				error_code_exit(SYNTAX_ERROR);
			}
		}
		else{
			error_code_exit(SYNTAX_ERROR);
		}
	}
	//	( <expr>
	else if (token->type == token_type_obracket){
		Token* token_buffer = token;

		result = psa(1, token_buffer, 1, sym_table, local_sym_table, NULL, NULL);		//1 protoze ocekavame koncovy znak EOL, 1 protoze pred ( neni ID

		if (result < 100){ //3 protoze ocekavame koncovy znak do 
			if(in_foo){
				dispose_symtable(local_sym_table);
				dispose_symtable(sym_table);
				error_code_exit(result);
			}
			else{
				dispose_symtable(sym_table);
				error_code_exit(result);				
			}
		}

		if (in_foo){
			if (result == 100){
				last_return_type = node_dt_int;
			}
			else if (result == 101){
				last_return_type = node_dt_double;
			}
			else if (result == 102){
				last_return_type = node_dt_string;

			}
			else if (result == 103){
				last_return_type = node_dt_nil;
			}
		}
	}

	//	<item> <expr>
	else if ( (result = item() ) >= 100){

		char * tmp_string;
		tmp_string = (char*)malloc(sizeof(char));
		strcpy(tmp_string,"not_a_buffer_kappa");

		if (in_foo){
			insert_var_symtable(local_sym_table,tmp_string);
		}
		else{
			insert_var_symtable(sym_table,tmp_string);
		}

		switch(result){
			case 100: 
				if(in_foo){
					insert_var_data(local_sym_table, tmp_string, node_dt_int);
				}
				else{
					insert_var_data(sym_table, tmp_string, node_dt_int);
				}
				
				//Print_tree(sym_table->root);
				break;
			case 101: 
				if(in_foo){
					insert_var_data(local_sym_table, tmp_string, node_dt_double);
				}
				else{
					insert_var_data(sym_table, tmp_string, node_dt_double);
				}			
				break;

			case 102:
				if(in_foo){
					insert_var_data(local_sym_table, tmp_string, node_dt_string);
				}
				else{
					insert_var_data(sym_table, tmp_string, node_dt_string);
				}	
				break;

			case 103: 
				if(in_foo){
					insert_var_data(local_sym_table, tmp_string, node_dt_nil);
				}
				else{
					insert_var_data(sym_table, tmp_string, node_dt_nil);
				}
				break;

			default:
				break;
		}


		if(in_foo){
			result = psa(1,NULL,2, sym_table, local_sym_table, tmp_string, NULL);

			if (result < 100){
					dispose_symtable(local_sym_table);
					dispose_symtable(sym_table);
					error_code_exit(result);	
			}

		}
		else{
			result = psa(1,NULL,2, sym_table,local_sym_table, tmp_string, NULL);		

			if (result < 100){
					dispose_symtable(sym_table);
					error_code_exit(result);	
			}
	
		}

		if (in_foo){
			if (result == 100){
				last_return_type = node_dt_int;
			}
			else if (result == 101){
				last_return_type = node_dt_double;
			}
			else if (result == 102){
				last_return_type = node_dt_string;

			}
			else if (result == 103){
				last_return_type = node_dt_nil;
			}
		}
	}
	else{
		error_code_exit(SYNTAX_ERROR);
	}
	return 0;
}

// 	EOL <stat-list-end>
//	end
// 	<stat> <stat-list-end>
int stat_list_end(int in_foo){


	if (token->type == token_type_eol){
		get_new_token(token);
		stat_list_end(in_foo);
	}
	if (token->type == token_type_keyword && token->value.keyword == keyword_end){
	}
	else if (stat(in_foo) == 0){
		get_new_token(token);
		stat_list_end(in_foo);
	}
	else{
		error_code_exit(SYNTAX_ERROR);
	}
	return 0;
}

// 	EOL <stat-list-else>
// 	else
// 	<stat> <stat-list-else>
int stat_list_else(int in_foo){

	if(token->type == token_type_eol){
		get_new_token(token);
		stat_list_else(in_foo);
	}
	if(token->type == token_type_keyword && token->value.keyword == keyword_else){
	}
	else if (stat(in_foo) == 0){
		get_new_token(token);
		stat_list_else(in_foo);
	}
	else{
		error_code_exit(SYNTAX_ERROR);
	}
	return 0;
}

// 	EOF
// 	EOL <code>
// 	<stat> <code>
int stat_list(){
	int in_foo = 0;

	if (token->type == token_type_eof){

	}
	else if (token->type == token_type_eol){
		get_new_token(token);
		code();
	}
	else if (stat(in_foo) == 0){
		get_new_token(token);
		code();	
	}
	else{
		error_code_exit(SYNTAX_ERROR);
	}
	return 0;
}

// 	) EOL
//	ID <next-param>
int params(int *params_count){


	if (token->type == token_type_cbracket){
		get_new_token(token);
		if (token->type == token_type_eol){

		}
		else{
			error_code_exit(SYNTAX_ERROR);
		}
	}
	else if (token->type == token_type_identifier){
		(*params_count)++;

		insert_var_symtable(local_sym_table, token->value.name);

		//Print_tree(local_sym_table->root);

		get_new_token(token);
		next_param(params_count);
	}
	else{
		error_code_exit(SYNTAX_ERROR);
	}
	return 0;
}

//	def ID ( <item_list> <next_param> <stat-list-end> EOL <code>
//	<stat-list>
int code(){
	//	def ID ( <item_list> <next_param> <stat-list-end> EOL <code>
	if (token->type == token_type_keyword && token->value.keyword == keyword_def){
		get_new_token(token);
		if (token->type == token_type_identifier){

			get_new_token(token);
			if (token->type == token_type_obracket){

				if (search_symtable(sym_table, token->value.name)){
					error_code_exit(SEMANTIC_ERROR);
				}

				int in_foo = 1;
				char* foo_name = token->value.name;
				
				insert_func_symtable(sym_table, foo_name);

				int params_count = 0;

				local_sym_table = (tSymtable*)malloc(sizeof(tSymtable));
				
				//inicializace lokalni tabulky symbolu
				init_symtable(local_sym_table);

				get_new_token(token);
				params(&params_count);


			

				last_return_type = node_dt_nil;

				get_new_token(token);
				stat_list_end(in_foo);		
			
				insert_func_data(sym_table, foo_name, last_return_type, params_count);


				last_return_type = node_dt_nil;
				params_count = 0;

				get_new_token(token);
				if (token->type == token_type_eol){

					//EOL, probublam zpet na code()

				}
				else{
					error_code_exit(SYNTAX_ERROR);
				}

				//vlozim return value

				in_foo = 0;
				dispose_symtable(local_sym_table);

				get_new_token(token);
				code();	
			}
			else{
				error_code_exit(SYNTAX_ERROR);
			}	
		}
		else{
			error_code_exit(SYNTAX_ERROR);
		}
	}
	else if (stat_list() == 0){
		
	}
	else{
		error_code_exit(SYNTAX_ERROR);
	}
	return 0;
}

// 	<code>
int prog(){
	get_new_token(token);
	code();
	return 0;
}

int parse(){

	token = malloc(sizeof(Token));
	sym_table = (tSymtable*)malloc(sizeof(tSymtable));

	init_symtable(sym_table);

	prog();

    return 0;
}