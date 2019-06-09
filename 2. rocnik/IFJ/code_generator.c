/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  -> code_generator.c                                     |
 *|                                                                 |
 *| Autoři  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/

#define ARR_SIZE 255


#include "code_generator.h"

string* str;

void str_to_ascii(char *str,string* s){
    int i = 0;
    char tmp_str[10];
    while(str[i] != '\0'){
        if(str[i] < 33 || str[i] == 35 || str[i] == 92){
            sprintf(tmp_str,"\\%03d",str[i]);
            strAddChars(s,tmp_str);
        }
        else{
        strAddChar(s,str[i]);
        }
        i++;
    }
}

void write_instr_no_operand(tList *instruction_list, instr_type ins_type){
    generate_instruction(instruction_list,ins_type,NULL,NULL,NULL);
}

void write_instr_one_operand(tList *instruction_list, instr_type ins_type, tInstrOperand element1){
    string operand1;
    strInit(&operand1);
    // Operand je proměnná
    if(element1.data_type == dt_identificator) {
        // Operand je pomocná proměnná
        if (element1.isRetval) {
            strClear(&operand1);
            strAddChars(&operand1, "GF@%retval");
        }
        else if (element1.frame == FR_LF) {
            strClear(&operand1);
            strAddChars(&operand1, "LF@");
            strAddChars(&operand1, element1.str.str);
        }
        else if (element1.frame == FR_GF) {
            strClear(&operand1);
            strAddChars(&operand1, "GF@");
            strAddChars(&operand1, element1.str.str);

        }
        else{
            strClear(&operand1);
            strAddChars(&operand1, "TF@");
            strAddChars(&operand1, element1.str.str);
        }
    }
    // Operand je konstanta
    else if(element1.data_type == dt_int){
        strClear(&operand1);
        strAddChars(&operand1, "int@");
        strAddChars(&operand1, element1.str.str);
    }
    else if(element1.data_type == dt_double){
        char tmp_str[ARR_SIZE];
        strClear(&operand1);
        strAddChars(&operand1, "float@");
        double dbl = atof(element1.str.str);
        sprintf(tmp_str,"%a",dbl);
        strAddChars(&operand1, tmp_str);
    }
    else if(element1.data_type == dt_string){
        string tmp_str2;
        strInit(&tmp_str2);
        strClear(&operand1);
        strAddChars(&operand1, "string@");
        str_to_ascii(element1.str.str,&tmp_str2);
        strAddChars(&operand1, tmp_str2.str);
    }
    else if(element1.data_type == dt_nil){
        strClear(&operand1);
        strAddChars(&operand1, "nil@nil");
    }
    // Operand je návěští
    if(element1.isLabel) {
        strClear(&operand1);
        strAddChars(&operand1, "$");
        strAddChars(&operand1, element1.str.str);
    }

    generate_instruction(instruction_list, ins_type, operand1.str, NULL, NULL);
}

void write_instr_two_operand(tList *instruction_list, instr_type ins_type, tInstrOperand element1, tInstrOperand element2){
    string operand1;
    strInit(&operand1);

    // Operand je proměnná
    if(element1.data_type == dt_identificator) {
        // Operand je pomocná proměnná
        if (element1.isRetval) {
            strClear(&operand1);
            strAddChars(&operand1, "GF@%retval");
        }
        else if (element1.frame == FR_LF) {
            strClear(&operand1);
            strAddChars(&operand1, "LF@");
            strAddChars(&operand1, element1.str.str);
        }
        else if (element1.frame == FR_GF) {
            strClear(&operand1);
            strAddChars(&operand1, "GF@");
            strAddChars(&operand1, element1.str.str);

        }
        else{
            strClear(&operand1);
            strAddChars(&operand1, "TF@");
            strAddChars(&operand1, element1.str.str);
        }
    }
    // Operand je konstanta
    else if(element1.data_type == dt_int){
        strClear(&operand1);
        strAddChars(&operand1, "int@");
        strAddChars(&operand1, element1.str.str);
    }
    else if(element1.data_type == dt_double){
        char tmp_str[ARR_SIZE];
        strClear(&operand1);
        strAddChars(&operand1, "float@");
        double dbl = atof(element1.str.str);
        sprintf(tmp_str,"%a",dbl);
        strAddChars(&operand1, tmp_str);
    }
    else if(element1.data_type == dt_string){
        string tmp_str2;
        strInit(&tmp_str2);
        strClear(&operand1);
        strAddChars(&operand1, "string@");
        str_to_ascii(element1.str.str,&tmp_str2);
        strAddChars(&operand1, tmp_str2.str);
    }
    else if(element1.data_type == dt_nil){
        strClear(&operand1);
        strAddChars(&operand1, "nil@nil");
    }
    // Operand je návěští
    if(element1.isLabel) {
        strClear(&operand1);
        strAddChars(&operand1, "$");
        strAddChars(&operand1, element1.str.str);
    }

    string operand2;
    strInit(&operand2);
    // Operand je proměnná
    if(element2.data_type == dt_identificator) {
        // Operand je pomocná proměnná
        if (element2.isRetval) {
            strClear(&operand2);
            strAddChars(&operand2, "GF@%retval");
        }
        else if (element2.frame == FR_LF) {
            strClear(&operand2);
            strAddChars(&operand2, "LF@");
            strAddChars(&operand2, element2.str.str);
        }
        else if (element2.frame == FR_GF) {
            strClear(&operand2);
            strAddChars(&operand2, "GF@");
            strAddChars(&operand2, element2.str.str);

        }
        else{
            strClear(&operand2);
            strAddChars(&operand2, "TF@");
            strAddChars(&operand2, element2.str.str);
        }
    }
    //operand je input_type
    else if(element2.input_type == input_int){
        strClear(&operand2);
        strAddChars(&operand2, "int");
    }
    else if(element2.input_type == input_string){
        strClear(&operand2);
        strAddChars(&operand2, "string");
    }
    else if(element2.input_type == input_double){
        strClear(&operand2);
        strAddChars(&operand2, "double");
    }
    else if(element2.input_type == input_bool){
        strClear(&operand2);
        strAddChars(&operand2, "bool");
    }
    // Operand je konstanta
    else if(element2.data_type == dt_int){
        strClear(&operand2);
        strAddChars(&operand2, "int@");
        strAddChars(&operand2, element2.str.str);
    }
    else if(element2.data_type == dt_double){
        char tmp_str[ARR_SIZE];
        strClear(&operand2);
        strAddChars(&operand2, "float@");
        double dbl = atof(element2.str.str);
        sprintf(tmp_str,"%a",dbl);
        strAddChars(&operand2, tmp_str);
    }
    else if(element2.data_type == dt_string){
        string tmp_str2;
        strInit(&tmp_str2);
        strClear(&operand2);
        strAddChars(&operand2, "string@");
        str_to_ascii(element2.str.str,&tmp_str2);
        strAddChars(&operand2, tmp_str2.str);
    }
    else if(element2.data_type == dt_nil){
        strClear(&operand2);
        strAddChars(&operand2, "nil@nil");
    }

    generate_instruction(instruction_list, ins_type, operand1.str, operand2.str, NULL);
}

void write_instr_three_operand(tList *instruction_list, instr_type ins_type, tInstrOperand element1, tInstrOperand element2,tInstrOperand element3){
    string operand1;
    strInit(&operand1);

    // Operand je proměnná
    if(element1.data_type == dt_identificator) {
        // Operand je pomocná proměnná
        if (element1.isRetval) {
            strClear(&operand1);
            strAddChars(&operand1, "GF@%retval");
        }
        else if (element1.frame == FR_LF) {
            strClear(&operand1);
            strAddChars(&operand1, "LF@");
            strAddChars(&operand1, element1.str.str);
        }
        else if (element1.frame == FR_GF) {
            strClear(&operand1);
            strAddChars(&operand1, "GF@");
            strAddChars(&operand1, element1.str.str);

        }
        else{
            strClear(&operand1);
            strAddChars(&operand1, "TF@");
            strAddChars(&operand1, element1.str.str);
        }
    }
    // Operand je konstanta
    else if(element1.data_type == dt_int){
        strClear(&operand1);
        strAddChars(&operand1, "int@");
        strAddChars(&operand1, element1.str.str);
    }
    else if(element1.data_type == dt_double){
        char tmp_str[ARR_SIZE];
        strClear(&operand1);
        strAddChars(&operand1, "float@");
        double dbl = atof(element1.str.str);
        sprintf(tmp_str,"%a",dbl);
        strAddChars(&operand1, tmp_str);
    }
    else if(element1.data_type == dt_string){
        string tmp_str2;
        strInit(&tmp_str2);
        strClear(&operand1);
        strAddChars(&operand1, "string@");
        str_to_ascii(element1.str.str,&tmp_str2);
        strAddChars(&operand1, tmp_str2.str);
    }
    else if(element1.data_type == dt_nil){
        strClear(&operand1);
        strAddChars(&operand1, "nil@nil");
    }
    // Operand je návěští
    if(element1.isLabel) {
        strClear(&operand1);
        strAddChars(&operand1, "$");
        strAddChars(&operand1, element1.str.str);
    }

    string operand2;
    strInit(&operand2);
    // Operand je proměnná
    if(element2.data_type == dt_identificator) {
        // Operand je pomocná proměnná
        if (element2.isRetval) {
            strClear(&operand2);
            strAddChars(&operand2, "GF@%retval");
        }
        else if (element2.frame == FR_LF) {
            strClear(&operand2);
            strAddChars(&operand2, "LF@");
            strAddChars(&operand2, element2.str.str);
        }
        else if (element2.frame == FR_GF) {
            strClear(&operand2);
            strAddChars(&operand2, "GF@");
            strAddChars(&operand2, element2.str.str);

        }
        else{
            strClear(&operand2);
            strAddChars(&operand2, "TF@");
            strAddChars(&operand2, element2.str.str);
        }
    }
    //operand je input_type
    else if(element2.input_type == input_int){
        strClear(&operand2);
        strAddChars(&operand2, "int");
    }
    else if(element2.input_type == input_string){
        strClear(&operand2);
        strAddChars(&operand2, "string");
    }
    else if(element2.input_type == input_double){
        strClear(&operand2);
        strAddChars(&operand2, "double");
    }
    else if(element2.input_type == input_bool){
        strClear(&operand2);
        strAddChars(&operand2, "bool");
    }
    // Operand je konstanta
    else if(element2.data_type == dt_int){
        strClear(&operand2);
        strAddChars(&operand2, "int@");
        strAddChars(&operand2, element2.str.str);
    }
    else if(element2.data_type == dt_double){
        char tmp_str[ARR_SIZE];
        strClear(&operand2);
        strAddChars(&operand2, "float@");
        double dbl = atof(element2.str.str);
        sprintf(tmp_str,"%a",dbl);
        strAddChars(&operand2, tmp_str);
    }
    else if(element2.data_type == dt_string){
        string tmp_str2;
        strInit(&tmp_str2);
        strClear(&operand2);
        strAddChars(&operand2, "string@");
        str_to_ascii(element2.str.str,&tmp_str2);
        strAddChars(&operand2, tmp_str2.str);
    }
    else if(element2.data_type == dt_nil){
        strClear(&operand2);
        strAddChars(&operand2, "nil@nil");
    }

    string operand3;
    strInit(&operand3);
    // Operand je proměnná
    if(element3.data_type == dt_identificator) {
        // Operand je pomocná proměnná
        if (element3.isRetval) {
            strClear(&operand3);
            strAddChars(&operand3, "GF@%retval");
        }
        else if (element3.frame == FR_LF) {
            strClear(&operand3);
            strAddChars(&operand3, "LF@");
            strAddChars(&operand3, element3.str.str);
        }
        else if (element3.frame == FR_GF) {
            strClear(&operand3);
            strAddChars(&operand3, "GF@");
            strAddChars(&operand3, element3.str.str);

        }
        else{
            strClear(&operand3);
            strAddChars(&operand3, "TF@");
            strAddChars(&operand3, element3.str.str);
        }
    }
    // Operand je konstanta
    else if(element3.data_type == dt_int){
        strClear(&operand3);
        strAddChars(&operand3, "int@");
        strAddChars(&operand3, element3.str.str);
    }
    else if(element3.data_type == dt_double){
        char tmp_str[ARR_SIZE];
        strClear(&operand3);
        strAddChars(&operand3, "float@");
        double dbl = atof(element3.str.str);
        sprintf(tmp_str,"%a",dbl);
        strAddChars(&operand3, tmp_str);
    }
    else if(element3.data_type == dt_string){
        string tmp_str2;
        strInit(&tmp_str2);
        strClear(&operand3);
        strAddChars(&operand3, "string@");
        str_to_ascii(element3.str.str,&tmp_str2);
        strAddChars(&operand3, tmp_str2.str);
    }
    else if(element3.data_type == dt_nil){
        strClear(&operand3);
        strAddChars(&operand3, "nil@nil");
    }

    generate_instruction(instruction_list, ins_type, operand1.str, operand2.str, operand3.str);
}

void generate_instruction(tList *instruction_list, instr_type ins_type, void *operand1, void *operand2, void *operand3){
    tInstr instruction;

    instruction.ins_type = ins_type;
    instruction.operand1 = operand1;
    instruction.operand2 = operand2;
    instruction.operand3 = operand3;

    InsertLast(instruction_list, instruction);
}

tInstrOperand generate_operand(tInstrOperand operand, char* value, tDatatype type, frame frame, int isRetval, int isLabel, input_type input_tp){
    operand.str.str = value;
    operand.data_type = type;
    operand.frame = frame;
    operand.isRetval = isRetval;
    operand.isLabel = isLabel;
    operand.input_type = input_tp;
    return operand;
}

void generate_code(tList* instruction_list){
    tInstr* instruction;
    instruction = (tInstr*)malloc(sizeof(tInstr));
        
    char tmp_str[ARR_SIZE];

    First(instruction_list);

    while(Active(instruction_list)){        
        tmp_str[0] = '\0';
        
        Copy(instruction_list,instruction);

        switch(instruction->ins_type){
            //Instrukce bez operandů
            case I_CREATEFRAME: add_code("CREATEFRAME\n"); break;
            case I_PUSHFRAME: add_code("PUSHFRAME\n"); break;
            case I_POPFRAME: add_code("POPFRAME\n"); break;
            case I_RETURN: add_code("RETURN\n"); break;
            case I_CLEARS: add_code("CLEARS\n"); break;
            case I_ADDS: add_code("ADDS\n"); break;
            case I_SUBS: add_code("SUBS\n"); break;
            case I_MULS: add_code("MULS\n"); break;
            case I_DIVS: add_code("DIVS\n"); break;
            case I_IDIVS: add_code("IDIVS\n"); break;
            case I_LTS: add_code("LTS\n"); break;
            case I_GTS: add_code("GTS\n"); break;
            case I_EQS: add_code("EQS\n"); break;
            case I_ANDS: add_code("ANDS\n"); break;
            case I_ORS: add_code("ORS\n"); break;
            case I_NOTS: add_code("NOTS\n"); break;
            case I_INT2FLOATS: add_code("INT2FLOATS\n"); break;
            case I_FLOAT2INTS: add_code("FLOAT2INTS\n"); break;
            case I_INT2CHARS: add_code("INT2CHARS\n"); break;
            case I_STRI2INTS: add_code("STRI2INTS\n"); break;
            case I_BREAK: add_code("BREAK\n"); break;

            //Instrukce s jedním operandem
            case I_DEFVAR: sprintf(tmp_str,"DEFVAR %s\n",(char*)instruction->operand1); break;
            case I_CALL: sprintf(tmp_str,"CALL %s\n",(char*)instruction->operand1); break;
            case I_PUSHS: sprintf(tmp_str,"PUSHS %s\n",(char*)instruction->operand1); break;
            case I_POPS: sprintf(tmp_str,"POPS %s\n",(char*)instruction->operand1); break;
            case I_WRITE: sprintf(tmp_str,"WRITE %s\n",(char*)instruction->operand1); break;
            case I_LABEL:sprintf(tmp_str,"LABEL %s\n",(char*)instruction->operand1); break;
            case I_JUMP: sprintf(tmp_str,"JUMP %s\n",(char*)instruction->operand1); break;
            case I_JUMPIFEQS: sprintf(tmp_str,"JUMPIFEQS %s\n",(char*)instruction->operand1); break;
            case I_JUMPIFNEQS: sprintf(tmp_str,"JHUMPIFNEQS %s\n",(char*)instruction->operand1); break;
            case I_EXIT: sprintf(tmp_str,"EXIT %s\n",(char*)instruction->operand1); break;
            case I_DPRINT: sprintf(tmp_str,"DPRINT %s\n",(char*)instruction->operand1); break;

            //Instrukce se dvěma operandy
            case I_MOVE: sprintf(tmp_str,"MOVE %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2); break;
            case I_INT2FLOAT: sprintf(tmp_str,"INT2FLOAT %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2); break;
            case I_FLOAT2INT: sprintf(tmp_str,"FLOAT2INT %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2); break;
            case I_INT2CHAR: sprintf(tmp_str,"INT2CHAR %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2); break;
            case I_READ: sprintf(tmp_str,"READ %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2); break;
            case I_STRLEN: sprintf(tmp_str,"STRLEN %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2); break;
            case I_TYPE: sprintf(tmp_str,"TYPE %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2); break;
            
            //Instukce se třemi operandy
            case I_ADD: sprintf(tmp_str,"ADD %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_SUB: sprintf(tmp_str,"SUB %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_MUL: sprintf(tmp_str,"MUL %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_DIV: sprintf(tmp_str,"DIV %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_IDIV: sprintf(tmp_str,"IDIV %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_LT: sprintf(tmp_str,"LT %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_GT: sprintf(tmp_str,"GT %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_EQ: sprintf(tmp_str,"EQ %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_AND: sprintf(tmp_str,"AND %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_OR: sprintf(tmp_str,"OR %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_NOT: sprintf(tmp_str,"NOT %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_STRI2INT: sprintf(tmp_str,"STRI2INT %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_CONCAT: sprintf(tmp_str,"CONCAT %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_GETCHAR: sprintf(tmp_str,"GETCHAR %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_SETCHAR: sprintf(tmp_str,"SETCHAR %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_JUMPIFEQ: sprintf(tmp_str,"JUMPIFEQ %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;
            case I_JUMPIFNEQ: sprintf(tmp_str,"JUMPIFNEQ %s %s %s\n",(char*)instruction->operand1,(char*)instruction->operand2,(char*)instruction->operand3); break;

            default: break;
        }
        //zápis do globální proměnné na vypsání kódu
        add_code(tmp_str);
        //posun na další prvek
        Succ(instruction_list);
    }
}


/*----------------instrukce fce------------------------*/

void set_global_string(string *s){
    str = s;
}

void print_global_string(string *s){
    printf("%s", s->str);
}

void print_header(){
    add_code(".IFJcode18\n"   \
           "JUMP $main\n\n");

}

int add_code(char * code){
    if(!strAddChars(str,code)) return 0;
    return 1;
}


/*---------------------funkce zabudovanych funkci-------------------------------*/

void print_built_in_functions(){
    print_function_inputs();
    print_function_inputf();
    print_function_inputi();

    print_function_chr();
    print_function_length();
    print_function_ord();
    print_function_substr();
    print_label_main();
}

void print_function_inputs(){
    add_code("\nLABEL $inputs\n"    \
           "PUSHFRAME\n"    \
           "DEFVAR LF@%retval\n"   \
           "READ LF@%retval string\n" \
           "POPFRAME\n"        \
           "RETURN\n");
}

void print_function_inputf(){
    add_code("\nLABEL $inputf\n"    \
           "PUSHFRAME\n"    \
           "DEFVAR LF@%retval\n"   \
           "READ LF@%retval float\n" \
           "POPFRAME\n"        \
           "RETURN\n");
}

void print_function_inputi(){
    add_code("\nLABEL $inputi\n"    \
           "PUSHFRAME\n"    \
           "DEFVAR LF@%retval\n"   \
           "READ LF@%retval int\n" \
           "POPFRAME\n"        \
           "RETURN\n");
}

void print_function_ord(){
    add_code("\nLABEL $ord\n"                                                         \
            "PUSHFRAME\n"                                                           \
            "DEFVAR LF@cond\n"
            "DEFVAR LF@param1$type\n"                                               \
            "TYPE LF@param1$type LF@%param1\n"\
            "EQ LF@cond LF@param1$type string@string\n"\
            "JUMPIFNEQ $ord$exit LF@cond bool@true\n"                                       \

            "DEFVAR LF@param2$type\n"                                               \
            "TYPE LF@param2$type LF@%param2\n"\
            "EQ LF@cond LF@param2$type string@float\n"\
            "JUMPIFEQ $ord$type_change LF@cond bool@true\n"                                     \
            "EQ LF@cond LF@param2$type string@int\n"\
            "JUMPIFNEQ $ord$exit LF@cond bool@true\n"                                       \

            "LABEL $ord$continue\n" \
            "DEFVAR LF@%retval\n"                                                   \
            "MOVE LF@%retval nil@nil\n"                                         \
            "DEFVAR LF@cond_length\n"                                               \
            "LT LF@cond_length LF@%param2 int@0\n"                                      \
            "JUMPIFEQ $ord$return LF@cond_length bool@true\n"                       \
            "DEFVAR LF@length_str\n"                                                \
            "CREATEFRAME\n"                                                     \
            "DEFVAR TF@%param1\n"                                                       \
            "MOVE TF@%param1 LF@%param1\n"                                                  \
            "CALL $length\n"                                                        \
            "MOVE LF@length_str TF@%retval\n"                                       \
            "SUB LF@length_str LF@length_str int@1\n"                               \
            "GT LF@cond_length LF@%param2 LF@length_str\n"                              \
            "JUMPIFEQ $ord$return LF@cond_length bool@true\n"                       \
            "STRI2INT LF@%retval LF@%param1 LF@%param2\n"                                   \
            "POPFRAME\n"                                                            \
            "RETURN\n"

            "LABEL $ord$return\n"                                                   \
            "MOVE LF@%retval nil@nil\n"\
            "POPFRAME\n"\
            "RETURN\n"\

           "LABEL $ord$type_change\n" \
           "FLOAT2INT LF@%param2 LF@%param2\n"\
           "JUMP $ord$continue\n"                                       \

           "LABEL $ord$exit\n" \
           "EXIT int@4\n"\
    );
}
void print_function_chr(){
    add_code("\nLABEL $chr\n"                                                           \
           "PUSHFRAME\n"                                                            \
           "DEFVAR LF@%retval\n"                                                    \
           "MOVE LF@%retval string@\n"                                          \
           "DEFVAR LF@cond_range\n"\
           "DEFVAR LF@param1$type\n"\
           "TYPE LF@param1$type LF@%param1\n"\
           "EQ LF@cond_range LF@param1$type string@int\n"\
           "JUMPIFEQ $chr$no_typechange LF@cond_range bool@true\n"                                              \
           "FLOAT2INT LF@%param1 LF@%param1\n"\
           "LABEL $chr$no_typechange\n"\
           "LT LF@cond_range LF@%param1 int@0\n"                                        \
           "JUMPIFEQ $chr$return_exit LF@cond_range bool@true\n"                        \
           "GT LF@cond_range LF@%param1 int@255\n"                                      \
           "JUMPIFEQ $chr$return_exit LF@cond_range bool@true\n"                        \
           "JUMP $chr$return\n"\

           "LABEL $chr$return_exit\n"                                                   \
           "EXIT int@4\n"\

           "LABEL $chr$return\n" \
           "INT2CHAR LF@%retval LF@%param1\n"                                       \
           "POPFRAME\n"                                                         \
           "RETURN\n"
    );
}

void print_function_substr(){
  add_code("\nLABEL $substr\n"                  \
           "PUSHFRAME\n"                     \
           "DEFVAR LF@%retval\n"                 \
           "MOVE LF@%retval string@\n"             \
           "DEFVAR LF@length_str\n"                \
           "CREATEFRAME\n"                   \
           "DEFVAR TF@%param1\n"                   \

           "DEFVAR LF@cond\n"                        \
           "DEFVAR LF@param1$type\n"                        \
           "TYPE LF@param1$type LF@%param1\n"\
           "EQ LF@cond LF@param1$type string@string\n"\
           "JUMPIFNEQ $substr$exit LF@cond bool@true\n"                   \

           "DEFVAR LF@param2$type\n"                        \
           "TYPE LF@param2$type LF@%param2\n"\
           "EQ LF@cond LF@param2$type string@float\n"\
           "JUMPIFEQ $substr$type_change_param2 LF@cond bool@true\n"                   \
           "EQ LF@cond LF@param2$type string@int\n"\
           "JUMPIFNEQ $substr$exit LF@cond bool@true\n"                   \

           "LABEL $substr$continue_param2\n"\
           "DEFVAR LF@param3$type\n"                        \
           "TYPE LF@param3$type LF@%param3\n"\
           "EQ LF@cond LF@param3$type string@float\n"\
           "JUMPIFEQ $substr$type_change_param3 LF@cond bool@true\n"                   \
           "EQ LF@cond LF@param3$type string@int\n"\
           "JUMPIFNEQ $substr$exit LF@cond bool@true\n"                   \

           "LABEL $substr$continue\n" \
           "MOVE TF@%param1 LF@%param1\n"                  \
           "CALL $length\n"                    \
           "MOVE LF@length_str TF@%retval\n"           \
           "DEFVAR LF@ret_cond\n"                  \
           "LT LF@ret_cond LF@length_str int@0\n"          \
           "JUMPIFEQ $substr$return_nil LF@ret_cond bool@true\n"   \
           "EQ LF@ret_cond LF@length_str int@0\n"          \
           "JUMPIFEQ $substr$return_nil LF@ret_cond bool@true\n"   \
           "LT LF@ret_cond LF@%param2 int@0\n"             \
           "JUMPIFEQ $substr$return_nil LF@ret_cond bool@true\n"   \
           "GT LF@ret_cond LF@%param2 LF@length_str\n"         \
           "JUMPIFEQ $substr$return_nil LF@ret_cond bool@true\n"   \
           "LT LF@ret_cond LF@%param3 int@0\n"             \
           "JUMPIFEQ $substr$return_nil LF@ret_cond bool@true\n"   \
           "DEFVAR LF@max_n\n"                 \
           "MOVE LF@max_n LF@length_str\n"           \
           "SUB LF@max_n LF@max_n LF@%param2\n"              \
           "GT LF@ret_cond LF@%param3 LF@max_n\n"                  \
           "JUMPIFEQ $substr$edit_n LF@ret_cond bool@true\n" 
           "JUMP $substr$process\n"                        \
           "LABEL $substr$edit_n\n"                        \
           "MOVE LF@%param3 LF@max_n\n"                        \
           "LABEL $substr$process\n"
           "DEFVAR LF@index\n"                 \
           "MOVE LF@index LF@%param2\n"                \
           "DEFVAR LF@char\n"                    \
           "DEFVAR LF@process_loop_cond\n"           \
           "LABEL $substr$process_loop\n"              \
           "GETCHAR LF@char LF@%param1 LF@index\n"           \
           "CONCAT LF@%retval LF@%retval LF@char\n"        \
           "ADD LF@index LF@index int@1\n"           \
           "SUB LF@%param3 LF@%param3 int@1\n"               \
           "GT LF@process_loop_cond LF@%param3 int@0\n"        \
           "JUMPIFEQ $substr$process_loop LF@process_loop_cond bool@true\n"    \
           "JUMP $substr_return\n"                                     \
           "LABEL $substr$return_nil\n"                \
           "MOVE LF@retval nil@nil\n"                      \
           "LABEL $substr_return\n"                                \
           "POPFRAME\n"                      \
           "RETURN\n"


  "LABEL $substr$type_change_param2\n" \
  "FLOAT2INT LF@%param2 LF@%param2\n"\
  "JUMP $substr$continue_param2\n"                   \

  "LABEL $substr$type_change_param3\n" \
  "FLOAT2INT LF@%param3 LF@%param3\n"\
  "JUMP $substr$continue\n"                   \

  "LABEL $substr$exit\n" \
  "EXIT int@4\n\n");
}

void print_function_length(){
    add_code("\nLABEL $length\n"             \
           "PUSHFRAME\n"                  \
           "DEFVAR LF@%retval\n"         \
           "DEFVAR LF@cond\n"\
           "DEFVAR LF@param1$type\n" \
           "TYPE LF@param1$type LF@%param1\n"\
           "EQ LF@cond LF@param1$type string@string\n"\
           "JUMPIFNEQ $length$exit LF@cond bool@true\n"                        \
           "STRLEN LF@%retval LF@%param1\n"   \
           "POPFRAME\n"                  \
           "RETURN\n" \

           "LABEL $length$exit\n" \
           "EXIT int@4\n"\
           );
}
void print_label_main(){
    add_code("LABEL $main\n"\
             "DEFVAR GF@%retval\n" \
             "CREATEFRAME\n" \
             "PUSHFRAME\n");
}

/*----------------------------funkce dvousmerne vazaneho seznamu----------------------------*/
void Error() {
printf ("*ERROR* The program has performed an illegal operation.\n");
return;
}

void InitList (tList *L) {
if(L == NULL)
  return;


L->Act = NULL;
L->First = NULL;
print_header();
print_built_in_functions();
}

void DisposeList (tList *L) {
//Neexistuje-li seznam, vytiskne chybu
if(L == NULL){
  Error();
  return;
}

if(L->First == NULL)
  return;

tInstrPtr elemPtr;

while(L->First != NULL){
elemPtr = L->First;
L->First = elemPtr->ptr;
free(elemPtr);
}

L->First = NULL;
L->Act = NULL;

return;
}

void InsertLast(tList *L, tInstr instruction) {
//Kontrola, zdali seznam existuje
if(L == NULL)
    return;

tInstrPtr newElemPtr = (tInstrPtr) malloc(sizeof(struct tElem));

//Kontrola vytvoření nového prvku
if(newElemPtr == NULL){
    Error();
    return;
}

if(L->First == NULL){
    newElemPtr->instruction = instruction;
    newElemPtr->ptr = NULL;
    L->First = newElemPtr;
    L->Act = L->First;
}
else{
newElemPtr->instruction = instruction;
newElemPtr->ptr = NULL;
L->Act->ptr = newElemPtr;
Succ(L);
}

return;
}

void First (tList *L) {
L->Act = L->First;
return;
}

void Copy (tList *L, tInstr* instruction) {
//Kontrola prázdného seznamu a aktivity seznamu
if((L == NULL) || (L->Act == NULL)){
  Error();
  return;
}
(*instruction) = L->Act->instruction;

return;
}

void Succ (tList *L) {
//Kontrola aktivity seznamu
if(L->Act != NULL){
  tInstrPtr elemPtr;
  elemPtr = L->Act->ptr;
  L->Act = elemPtr;
}
return;
}

int Active (tList *L) {
return (L->Act != NULL) ? 1 : 0;
}