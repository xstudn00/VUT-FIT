/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  ->  psa.c                                               |
 *|                                                                 |
 *| Autori  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/

#include "scanner.h"
#include "psa.h"
#include "errors.h"
#include "symtable.h"
#include "parser.h"

const char prec_table[14][14] = {
//     *     /   +    -    <    >    >=  <=   ==   !=    (    )    ID   $
    { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // PSA_STAR
    { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // PSA_SLASH
    { '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // PSA_PLUS
    { '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '>'}, // PSA_MINUS
    { '<', '<', '<', '<', 'X', 'X', 'X', 'X', '>', '>', '<', '>', '<', '>'}, // PSA_LESS
    { '<', '<', '<', '<', 'X', 'X', 'X', 'X', '>', '>', '<', '>', '<', '>'}, // PSA_MORE
    { '<', '<', '<', '<', 'X', 'X', 'X', 'X', '>', '>', '<', '>', '<', '>'}, // PSA_MORE_EQUAL
    { '<', '<', '<', '<', 'X', 'X', 'X', 'X', '>', '>', '<', '>', '<', '>'}, // PSA_LESS_EQUAL
    { '<', '<', '<', '<', '<', '<', '<', '<', 'X', 'X', '<', '>', '<', '>'}, // PSA_EQUAL
    { '<', '<', '<', '<', '<', '<', '<', '<', 'X', 'X', '<', '>', '<', '>'}, // PSA_NOT_EQUAL
    { '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', 'X'}, // PSA_OBRACKET
    { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', '>'}, // PSA_CBRACKET
    { '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', '>'}, // PSA_ID
    { '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'X', '<', 'E'}  // PSA_DOLLAR
};

const char data_type_table[5][5] = {
//    i    d    s    n   ndf 
    {'K', 'K', 'E', 'E', 'K'}, //  i   
    {'K', 'K', 'E', 'E', 'K'}, //  d 
    {'E', 'E', 'K', 'E', 'K'}, //  s
    {'E', 'E', 'E', 'K', 'K'}, //  n
    {'K', 'K', 'K', 'K', 'K'}  //  ndf
};

tSymtable* psa_sym_table;
tSymtable* psa_local_sym_table;
int first_id_loop;

void stack_init(stack_ptr* stack){
    stack->top = NULL;
}

int stack_pop(stack_ptr* stack){
    tStack *tmp_ptr;
    if(stack->top != NULL){
        tmp_ptr = stack->top;
        stack->top = stack->top->right_ptr;
        free(tmp_ptr);
        return 0;
    }
    else{
        return 1;
    }
}

int stack_push(stack_ptr* stack, PSA_TYPE value){
    tStack *tmp_ptr = malloc(sizeof(struct tStack_structure));
    if(tmp_ptr != NULL){
        tmp_ptr->value = value;
        tmp_ptr->right_ptr = stack->top;
        stack->top = tmp_ptr;
        return 0;
    }
    else{
        return 1;
    }
}

int stack_dispose(stack_ptr* stack){
    while(stack->top != NULL){
        stack_pop(stack);
    }
    return 0;
}

PSA_TYPE stack_top_terminal(stack_ptr* stack){
    tStack* current;

    if (stack->top == NULL){
        return PSA_SYNTAX_ERROR;
    }
    current = stack->top;

    while (1){
        if (current->value == PSA_DOLLAR){
            return PSA_DOLLAR;
        } 
        else if (current->value == PSA_NONTERMINAL_E){
            current = current->right_ptr;
        }
        else{
            return current->value;
        }
    }
}

int push_with_barrier(stack_ptr* stack, PSA_TYPE value){
    //dá < před prvni terminal
    if (stack->top == NULL){
        return PSA_SYNTAX_ERROR;
    }
    
    if(stack->top->value != PSA_NONTERMINAL_E){
        stack_push(stack, PSA_NONTERMINAL_BARRIER);
        stack_push(stack, value);
        return 0;
    }
    else{
        if (stack->top->right_ptr->value != PSA_NONTERMINAL_E){
            tStack *tmp_ptr = malloc(sizeof(struct tStack_structure));
            tmp_ptr->value = PSA_NONTERMINAL_BARRIER;
            tmp_ptr->right_ptr = stack->top->right_ptr;
            stack->top->right_ptr = tmp_ptr;
            stack_push(stack, value);
            return 0;
        }

        else
            return 1;
    }
}


int use_rule(stack_ptr* stack){
    // E -> <a
    if (stack->top->value == PSA_ID &&
        stack->top->right_ptr->value == PSA_NONTERMINAL_BARRIER){

        stack_pop(stack);   //pop ID
        stack_pop(stack);   //pop <
        stack_push(stack, PSA_NONTERMINAL_E);   //push E
    }
    // E -> E +-*/<>=!= E
    else if(stack->top->value == PSA_NONTERMINAL_E && 
        (
        stack->top->right_ptr->value == PSA_PLUS ||
        stack->top->right_ptr->value == PSA_MINUS||
        stack->top->right_ptr->value == PSA_STAR ||
        stack->top->right_ptr->value == PSA_SLASH ||
        stack->top->right_ptr->value == PSA_MORE ||
        stack->top->right_ptr->value == PSA_MORE_EQUAL ||
        stack->top->right_ptr->value == PSA_LESS ||
        stack->top->right_ptr->value == PSA_LESS_EQUAL ||
        stack->top->right_ptr->value == PSA_EQUAL ||
        stack->top->right_ptr->value == PSA_NOT_EQUAL
        ) &&
        stack->top->right_ptr->right_ptr->value == PSA_NONTERMINAL_E &&
        stack->top->right_ptr->right_ptr->right_ptr->value == PSA_NONTERMINAL_BARRIER)
        {
            stack_pop(stack);   //pop E
            stack_pop(stack);   //pop +-*/
            stack_pop(stack);   //pop E
            stack_pop(stack);   //pop <
            stack_push(stack, PSA_NONTERMINAL_E);
        }
    // E -> (E)
    else if(stack->top->value == PSA_CBRACKET &&
            stack->top->right_ptr->value == PSA_NONTERMINAL_E &&
            stack->top->right_ptr->right_ptr->value ==  PSA_OBRACKET &&
            stack->top->right_ptr->right_ptr->right_ptr->value ==  PSA_NONTERMINAL_BARRIER)
            {
                stack_pop(stack);   //pop )
                stack_pop(stack);   //pop E
                stack_pop(stack);   //pop (
                stack_pop(stack);   //pop <
                stack_push(stack, PSA_NONTERMINAL_E);
            }
    else
        return SYNTAX_ERROR;

    return 0;
}

PSA_TYPE get_new_input(int dollar_input, int *id_buffer,Token** token_buffer, PSA_DATA_TYPE* input_data, char* tmp_string, char* var_name, int in_foo){
    
    Token* token;
    token = malloc(sizeof(Token));
 
    static int zero_divide;
    static instr_type last_operand;

    if(*token_buffer == NULL && *id_buffer == 0){
        if (get_new_token(token) !=0)
            return PSA_LEX_ERROR;
    }
    else if(*id_buffer == 1){
        token->type = token_type_identifier;

        if(tmp_string == NULL){
            return SYNTAX_ERROR;
        }


        tBSTNodePtr node;
        node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

        node = search_symtable(psa_local_sym_table, tmp_string);


        tDataVar* data;
        data = (tDataVar*)node->Data;

        if(data->dataType == node_dt_int){;
            *input_data = TYPE_INT;
            delete_symtable(psa_local_sym_table, tmp_string);
        }
        else if(data->dataType == node_dt_double){
            *input_data = TYPE_DOUBLE;
            delete_symtable(psa_local_sym_table, tmp_string);
        }
        else if(data->dataType == node_dt_string){
            *input_data = TYPE_STRING;
            delete_symtable(psa_local_sym_table, tmp_string);
        }
        else if(data->dataType == node_dt_nil){
            *input_data = TYPE_NIL;
            delete_symtable(psa_local_sym_table, tmp_string);
        }
        else if(data->dataType == node_dt_default){
            *input_data = TYPE_NDF;
            delete_symtable(psa_local_sym_table, tmp_string);
        }
        else {
            delete_symtable(psa_local_sym_table, tmp_string);
            return PSA_EXPR_ERROR;
        }

        *id_buffer = 0;
        return PSA_ID;
        
    }
    else{
        token->type = (*token_buffer)->type;
        *token_buffer = NULL;
    }

    switch (token->type){
        case token_type_multiply: 
            zero_divide = 0;
            last_operand = I_MUL;
            return PSA_STAR;

        case token_type_divide:
            last_operand = I_DIV;
            zero_divide = 1;
            return PSA_SLASH;

        case token_type_plus:
            last_operand = I_ADD;
            zero_divide = 0;
            return PSA_PLUS;

        case token_type_minus:
            last_operand = I_SUB;
            zero_divide = 0;
            return PSA_MINUS;
            
        case token_type_less:
            zero_divide = 0;
            return PSA_LESS;
            
        case token_type_more:
            zero_divide = 0;
            return PSA_MORE;
            
        case token_type_meq:
            zero_divide = 0;
            return PSA_MORE_EQUAL;
            
        case token_type_leq:
            zero_divide = 0;
            return PSA_LESS_EQUAL;
            
        case token_type_eq:
            zero_divide = 0;
            return PSA_EQUAL;
            
        case token_type_neq:
            zero_divide = 0;
            return PSA_NOT_EQUAL;
            
        case token_type_obracket:
            zero_divide = 0;
            return PSA_OBRACKET;
            
        case token_type_cbracket:
            zero_divide = 0;
            return PSA_CBRACKET;
         
        case token_type_int:
                if (zero_divide == 1 && (atoi(token->value.string) == 0)){
                    return PSA_ZERO_ERROR;
                }
                else{
                    if( var_name != NULL){
                        if(first_id_loop == 0){
                            operand1 = generate_operand(operand1,var_name,dt_identificator,FR_LF,0,0,input_none);
                            operand2 = generate_operand(operand2,token->value.string,dt_int,FR_NONE,0,0,input_none);
                            write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
                            first_id_loop++;
                        }
                        else if (last_operand >= I_ADD && last_operand <= I_DIV){
                            operand1 = generate_operand(operand1, var_name, dt_identificator, FR_LF, 0, 0, input_none);
                            operand2 = generate_operand(operand2, var_name, dt_identificator, FR_LF, 0, 0, input_none);
                            operand3 = generate_operand(operand3, token->value.string, dt_int, FR_LF, 0, 0, input_none);
                            write_instr_three_operand(instr_list,last_operand,operand1,operand2,operand3);
                        }
                    }

                    *input_data = TYPE_INT;
                    return PSA_NOT_ID;
                }

        case token_type_string:
                if( var_name != NULL){
                    if(first_id_loop == 0){
                        operand1 = generate_operand(operand1,var_name,dt_identificator,FR_LF,0,0,input_none);
                        operand2 = generate_operand(operand2,token->value.string,dt_string,FR_NONE,0,0,input_none);
                        write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
                        first_id_loop++;
                    }
                    else if (last_operand == I_ADD){

                        operand1 = generate_operand(operand1, var_name, dt_identificator, FR_LF, 0, 0, input_none);
                        operand2 = generate_operand(operand2, var_name, dt_identificator, FR_LF, 0, 0, input_none);
                        operand3 = generate_operand(operand3, token->value.string, dt_string, FR_LF, 0, 0, input_none);
                        write_instr_three_operand(instr_list,I_CONCAT,operand1,operand2,operand3);
                    }
                    else{
                        return PSA_SEM_ERROR;
                    }
                }

            *input_data = TYPE_STRING;
            return PSA_NOT_ID;

        case token_type_double:
                if( var_name != NULL){
                    if(first_id_loop == 0){
                        operand1 = generate_operand(operand1,var_name,dt_identificator,FR_LF,0,0,input_none);
                        operand2 = generate_operand(operand2,token->value.string,dt_double,FR_NONE,0,0,input_none);
                        write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
                        first_id_loop++;
                    }
                    else if (last_operand >= I_ADD && last_operand <= I_DIV){
                        operand1 = generate_operand(operand1, var_name, dt_identificator, FR_LF, 0, 0, input_none);
                        operand2 = generate_operand(operand2, var_name, dt_identificator, FR_LF, 0, 0, input_none);
                        operand3 = generate_operand(operand3, token->value.string, dt_double, FR_LF, 0, 0, input_none);
                        write_instr_three_operand(instr_list,last_operand,operand1,operand2,operand3);
                    }
                }

            *input_data = TYPE_DOUBLE;
            return PSA_NOT_ID;

        case token_type_identifier:
            zero_divide = 0;
 
            tBSTNodePtr node;
            node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));

            int result;

            result = built_in_foo(token->value.name, in_foo);


            if (result > 0 && result < 100){
                error_code_exit(result);
            }
            else if (result == 100){
                *input_data = TYPE_INT;
                return PSA_BUILT_FOO;
            }
            else if (result == 101){
                *input_data = TYPE_DOUBLE;
                return PSA_BUILT_FOO;
            }        
            else if (result == 102){
                *input_data = TYPE_STRING;
                return PSA_BUILT_FOO;
            }        
            else if (result == 103){

                *input_data = TYPE_NIL;
                return PSA_BUILT_FOO;
            }
            else if (result == 104){
                *input_data = TYPE_NDF;
                return PSA_BUILT_FOO;
            }

            
            if( var_name != NULL){
              if(first_id_loop == 0){
                  operand1 = generate_operand(operand1,var_name,dt_identificator,FR_LF,0,0,input_none);
                  operand2 = generate_operand(operand2,token->value.name,dt_identificator,FR_NONE,0,0,input_none);
                  write_instr_two_operand(instr_list,I_MOVE,operand1,operand2);
              }
              else if (last_operand >= I_ADD && last_operand <= I_DIV){
                  operand1 = generate_operand(operand1, var_name, dt_identificator, FR_LF, 0, 0, input_none);
                  operand2 = generate_operand(operand2, var_name, dt_identificator, FR_LF, 0, 0, input_none);
                  operand3 = generate_operand(operand3, token->value.name, dt_identificator, FR_LF, 0, 0, input_none);
                  write_instr_three_operand(instr_list,last_operand,operand1,operand2,operand3);
              }
            }

            if (tmp_string== NULL){

                node = search_symtable(psa_local_sym_table, token->value.name);
                if (node == NULL){

                    node = search_symtable(psa_sym_table, token->value.name);

                    if(node == NULL){
                        return PSA_SEM_ERROR;
                    } 
                    else if (node->node_type == node_type_var){
                        return PSA_SEM_ERROR;
                    }            
                }
            }
            else{
            }    

            tDataVar* data;
            data = (tDataVar*)node->Data;



            if(data->dataType == node_dt_int){
                *input_data = TYPE_INT;
                 return PSA_ID; 
            }
            else if(data->dataType == node_dt_double){
                *input_data = TYPE_DOUBLE;
                 return PSA_ID; 
            }
            else if(data->dataType == node_dt_string){
                *input_data = TYPE_STRING;
                 return PSA_ID; 
            }
            else if(data->dataType == node_dt_nil){
                *input_data = TYPE_NIL;
                 return PSA_ID; 
            }
            else if(data->dataType == node_dt_default){
                *input_data = TYPE_NDF;
                return PSA_ID;
            }
            else {
                return PSA_SEM_ERROR;
            }

            break;
            /*****************************************************/               
            
        case token_type_keyword:
            zero_divide = 0;
            if (dollar_input == 2  && token->value.keyword == keyword_then){
                return PSA_DOLLAR;
            }
            else if (dollar_input == 3  && token->value.keyword == keyword_do){
                return PSA_DOLLAR;
            }
            else if(token->value.keyword == keyword_nil){
                *input_data = TYPE_NIL;
                return PSA_ID;
            }
            else {
                return PSA_SYNTAX_ERROR;
            }

        case token_type_eol:
            zero_divide = 0;
            if (dollar_input == 1){
                return PSA_DOLLAR;
            }

            else {
                return PSA_SYNTAX_ERROR;
            }
        default:
            return PSA_SYNTAX_ERROR;
    }

    return PSA_SYNTAX_ERROR;
}

PSA_TYPE function_buffer[2];
int comma_check(Token* token, int option){
    int result;

    tBSTNodePtr node;
    node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));


    if(token->type == token_type_comma){
        if((result = get_new_token(token)) !=0)
            return result;


        if(token->type == token_type_identifier){
            node = search_symtable(psa_local_sym_table, token->value.name);

            if(node != NULL){
                if(node->node_type == node_type_func){
                    return SEMANTIC_ERROR;
                }
            }
        }

        switch(token->type){

            case token_type_identifier:
            case token_type_int:
            case token_type_string:
            case token_type_double: 
                if((result = get_new_token(token)) !=0)
                    return result;

                result = comma_check(token, option);
                
                return result;   

            default:
                return SYNTAX_ERROR;
        }
    }
    else if(token->type == token_type_eol && option == 0){
        return 0;
    }
    else if(token->type == token_type_cbracket && option ==1){

        if((result = get_new_token(token)) !=0)
            return result;

        if(token->type == token_type_eol){
            return 0;
        }

        return SYNTAX_ERROR;
    }

    else{
        return SYNTAX_ERROR;
    }
}

int function_check(PSA_TYPE buffer[2]){
    int result;
    int option;
    // a a
    if(buffer[1] == PSA_ID){ 
        option = 0;

        Token* token;
        token = malloc(sizeof(Token));
        if((result = get_new_token(token)) !=0)
            return result;///////////////////////////////TODO


        result = comma_check(token,option);
        free(token);
        return result;

    }
    // a (
    else if(buffer[1] == PSA_OBRACKET){
        option = 1;

        Token* token;
        token = malloc(sizeof(Token));
        if((result = get_new_token(token)) !=0)
            return result;   

        if (token->type == token_type_identifier ||
            token->type == token_type_int ||
            token->type == token_type_double ||
            token->type == token_type_string
            ){

                Token* token;
                token = malloc(sizeof(Token));
                if((result = get_new_token(token)) !=0)
                    return result;


                result = comma_check(token, option);

                free(token);
                return result;
        }
        else if (token->type == token_type_cbracket){
                Token* token;
                token = malloc(sizeof(Token));
                if((result = get_new_token(token)) !=0)
                    return result;

                if(token->type == token_type_eol){
                    return 0;
                }
                else return SYNTAX_ERROR;
        }
        else    
        return SYNTAX_ERROR;
    }
    else{
        return SYNTAX_ERROR;
    }

}


int input_check(PSA_TYPE input){
    if (input == PSA_EXPR_ERROR){
        return EXPR_ERROR;
    }
    else if(input == PSA_LEX_ERROR){
        return LEXEM_ERROR;
    }
    else if(input == PSA_SYNTAX_ERROR){
        return SYNTAX_ERROR;
    }
    else if(input == PSA_SEM_ERROR){
        return SEMANTIC_ERROR;
    }
    else if(input == PSA_ZERO_ERROR){
        return DIVISON_BY_ZERO;
    }
    else
        return 0;
}

int type_check(PSA_TYPE previous_input_data, PSA_TYPE input_data){
    switch (data_type_table[previous_input_data][input_data]){
    case 'K':
        return 0;

    case 'E':
        return EXPR_ERROR;

    default:
        return EXPR_ERROR;
    }
}

int psa(int dollar_input, Token* token_parser, int id_option,tSymtable* sym_table, tSymtable* local_sym_table, char* tmp_string, char *name){
    int err = 0;
    char *var_name = name;
    Token *token_buffer = token_parser;
    int id_buffer = 0;

    int loop = 0;
    first_id_loop = 0;

    int in_foo;


    //nastaveni sym_table a zjisteni jestli jsem ve funkci nebo ne
    if(local_sym_table == NULL){
        psa_local_sym_table = sym_table;
        psa_sym_table = sym_table;
        in_foo = 0;
    }
    else{
        in_foo = 1;
        psa_local_sym_table = local_sym_table;
        psa_sym_table = sym_table;
    }
 

    if (token_buffer != NULL){

        /// funkce a (
        if (token_buffer->type == token_type_obracket && 
            dollar_input == 1 &&
            id_option == 0){

            function_buffer[0] = PSA_ID;
            function_buffer[1] = PSA_OBRACKET;
            err = function_check(function_buffer);

            return err;
        }
        // funkce a a
        else if (dollar_input == 1 && 
                    id_option == 0 &&
                (token_buffer->type == token_type_identifier ||
                 token_buffer->type == token_type_string ||
                 token_buffer->type == token_type_int ||
                 token_buffer->type == token_type_double ||
                 token_buffer->type == token_type_keyword  //mam zajistene z parseru, ze bude nil
                )){


            function_buffer[0] = PSA_ID;
            function_buffer[1] = PSA_ID;
            err = function_check(function_buffer);
            return err;
        }
        /// vyraz a +
        else if ( (id_option == 2 || id_option == 0 ) &&
                (token_buffer->type == token_type_plus ||
                 token_buffer->type == token_type_minus ||
                 token_buffer->type == token_type_divide ||
                 token_buffer->type == token_type_multiply)){


                id_buffer = 1;
        }
        // (
        else if(id_option != 1){
            return SYNTAX_ERROR;
        }
    }
    else if (id_option == 2){
        id_buffer = 1;
    }

    //inicializace promennych na syntax a semantickou analyzu
    PSA_TYPE input;
    PSA_TYPE stack_value;

    PSA_DATA_TYPE input_data;
    PSA_DATA_TYPE previous_input_data;



    if(token_buffer == NULL){   /// kontrola a=EOL bez prirazeni

        input = get_new_input(dollar_input, &id_buffer,&token_buffer, &input_data, tmp_string, var_name, in_foo);

        if ((err = input_check(input)) != 0)
            return err;

        if( input == PSA_BUILT_FOO){
            return (input_data+100);
        }

        tmp_string = NULL;

        if(dollar_input == 1 && id_option == 0){
            if (input == PSA_DOLLAR)
                return SYNTAX_ERROR;
        }
    }
    else{
        input = get_new_input(dollar_input, &id_buffer,&token_buffer, &input_data,tmp_string, var_name, in_foo); 
        tmp_string = NULL;
        if ((err = input_check(input)) != 0)
            return err;
    }

    //inicializace navratove hodnoty pro funkce
    int foo_return_value = 0;

    if ((err = input_check(input)) != 0)
        return err;
    else if(input == PSA_ID || input == PSA_NOT_ID){

        if(dollar_input == 1 && id_option == 0){
            foo_return_value = input_data;
            input = PSA_ID;
        }

        else{

            foo_return_value = input_data;
            function_buffer[0]=input;
            input = PSA_ID;           
        }
    }


    //inicializace stacku
    stack_ptr* stack;
    stack = malloc(sizeof(stack_ptr));

    stack_init(stack);

    stack_push(stack, PSA_DOLLAR);
    stack_value = stack_top_terminal(stack);


    //alogirtmus syntax analyzy vyrazu
    while(input != PSA_DOLLAR || stack_value != PSA_DOLLAR){

        loop++;

        //pretypovani int vyrazu pokud najdu double
        if((previous_input_data) == TYPE_DOUBLE){
            input_data = TYPE_DOUBLE;
        }

   
        switch(prec_table[stack_value][input]){
            case '=':
                stack_push(stack,input);
                previous_input_data = input_data;
                input = get_new_input(dollar_input, &id_buffer,&token_buffer, &input_data, tmp_string, var_name, in_foo);

                //jedna se o hodnotu/string
                if (input == PSA_NOT_ID){
                    input = PSA_ID;
                }

                if ((err = input_check(input)) != 0)
                    return err;               
                
                if (type_check(previous_input_data, input_data) == EXPR_ERROR)
                    return EXPR_ERROR;
                
                if ((err = input_check(input)) != 0)
                    return err;

                //nove nastaveni navratove hodnoty
                err = 100 + input_data;
                break;

            case '<':
                push_with_barrier(stack,input);
                int bracket_check;
                if( input == PSA_OBRACKET || input == PSA_CBRACKET){
                         bracket_check = 1;

                }
                previous_input_data = input_data;
                input = get_new_input(dollar_input, &id_buffer,&token_buffer, &input_data, tmp_string, var_name, in_foo);

                if ((err = input_check(input)) != 0)
                    return err;


                if (input == PSA_NOT_ID){
                    input = PSA_ID;
                }

                if(input == PSA_ID){
                    function_buffer[1] = PSA_ID;
                }

                // pokud na konci ma byt eol
                if( dollar_input == 1){
                    if( function_buffer[0] == PSA_ID && function_buffer[1] == PSA_ID){
                                 err = function_check(function_buffer);

                                if (err == 0){
                                    return (foo_return_value+100);
                                }
                                return err;                   
                    }
                }


                if ((dollar_input != 1 && id_option != 2) && (dollar_input != 2 && dollar_input != 3)){

                    if (bracket_check != 1 ){
                        if (function_buffer[0] != PSA_ID){
                            if (type_check(previous_input_data, input_data) == EXPR_ERROR)
                                return EXPR_ERROR;

                            if ((err = input_check(input)) != 0)
                                 return err;
                        }
                        else if (function_buffer[1] == PSA_ID){
                            err = function_check(function_buffer);

                            if (err == 0){
                                return (foo_return_value+100);
                            }
                            return err;
                        }

                        else{
                            return SEMANTIC_ERROR;
                        }                        
                    }
                }

                bracket_check = 0;
                err = 100 + input_data;
                break;
            case '>':

                //pouzivam pravidlo
                if (use_rule(stack) == SYNTAX_ERROR)
                    err = SYNTAX_ERROR;
                break;

            case 'X':

                //muze nastat kontrola funkce
                if((input == PSA_OBRACKET || input == PSA_ID) && dollar_input == 1 ){

                    if(function_buffer[0] == PSA_NOT_ID){
                        err = SYNTAX_ERROR;
                    }
                    else{
                        if (loop == 2){
                            function_buffer[1] = input;
                        }

                        else{
                            return SYNTAX_ERROR;
                        }
                        err = function_check(function_buffer);

                        if (err == 0){
                            return (foo_return_value+100);
                        }
                        return err;                       
                        stack_dispose(stack);
                        free(stack);
                        return err;
                    }
                }

                //pokud se nejedna o funkci
                err = SYNTAX_ERROR;
                break;
            default:
                break;
        }

        //ukonceni loopu v pripade chyby
        if (err == EXPR_ERROR || err == SYNTAX_ERROR || err == SEMANTIC_ERROR || err == INTERN_ERROR)
            break;

        //nejvyssi terminal
        stack_value = stack_top_terminal(stack);

        //ukonceni loopu v pripade chyby
        if( stack_value == PSA_SEM_ERROR){  
            err = SEMANTIC_ERROR;          
            break;
        }

        //
        if (previous_input_data >=0 && previous_input_data <5 && input_data >=0 && input_data <5 ){

            if (type_check(previous_input_data, input_data) == EXPR_ERROR)
                    return EXPR_ERROR;
        }
    }
    stack_dispose(stack);
    free(stack);

    return err;
}