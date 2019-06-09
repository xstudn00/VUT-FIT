/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  ->  scanner.c					                        |
 *|                                                                 |
 *| Autori  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "scanner.h"
#include "str.h"

// funkce pro vypisovani spravneho chyboveho stavu
void error_code_exit(int error_code){
    fprintf(stderr, "%d\n", error_code);
    //free
    exit(error_code);
}

bool is_first_token = true;

// nacte do "token" informace o danem tokenu jako jeho typ, hodnotu, u typu ID urcuje i hodnotu name
// je volana hlavne z syntakticke analyzy
int get_new_token(Token* token){
    string str;
    strInit(&str);
    char c;
    int scanner_state = SCANNER_START;

    strClear(&str);    
    while(1){
        c = (char) getc(stdin);	
        
        //printf("char: %c\n", c);
       
        switch(scanner_state){
            // v tomto stavu se bude rozrazovat smer behu teto funkce.
            // nacteny char bude vyhovovat jedne z techto moznosti a do te se vnori
            case (SCANNER_START):
                if(c == '\n'){
                    token->type = token_type_eol;
                    is_first_token = true;
                    return 0;
                }
                // char or '_' 
                else if(isalpha(c) || c == '_'){
                    strAddChar(&str, c);
                    scanner_state = SCANNER_IDENTIFIER_OR_KEYWORD;
                }
                else if(isdigit((int)c)){
                    strAddChar(&str, c);
                    scanner_state = SCANNER_NUMBER;
                }
                else if(isspace(c)){
                    is_first_token = false;
                    scanner_state = SCANNER_START;
                }
                else if(c == '#'){
                    scanner_state = SCANNER_LINE_COMMENT;
                }
                else if (c == '!'){
                    strAddChar(&str, c);
                    scanner_state = SCANNER_NEQ;
                }
                else if(c == '"'){
                    scanner_state = SCANNER_STRING;
                }
                else if (c == '<'){
                    scanner_state = SCANNER_LESS_THAN;
                }
                else if (c == '>'){
                    scanner_state = SCANNER_MORE_THAN;
                }
                else if (c == '='){
                    strAddChar(&str, c);
                    scanner_state = SCANNER_EQ_ASSIGN;
                }
                else if (c == '+'){  
                    token->type = token_type_plus;
                    return 0;
                }
                else if (c == '-'){
                    token->type = token_type_minus;
                    return 0;
                }
                else if (c == '*'){
                    token->type = token_type_multiply;
                    return 0;
                }
                else if (c == '/'){
                    token->type = token_type_divide;
                    return 0;
                }
                else if (c == ','){
                    token->type = token_type_comma;
                    return 0;
                }
				else if (c == '('){
					token->type = token_type_obracket;
					return 0;
				}
				else if (c == ')'){
					token->type = token_type_cbracket;
					return 0;
				}
                else if (c == EOF){
                    token->type = token_type_eof;   
                    return 0;
                }
                else
                    error_code_exit(LEXEM_ERROR);
                break;

            // muze byt porovnani, prirazeni nebo zacatek komentare
            case (SCANNER_EQ_ASSIGN):
                if (c == '='){
                    token->type = token_type_eq;
                    strAddChar(&str, c);
                    return 0;
                }
                else if (c == 'b' && is_first_token){
                        scanner_state = SCANNER_BLOCK_EQB;
                }
                else{
                    ungetc(c, stdin);
                    token->type = token_type_assign;
                    return 0;
                }
                break;

            // buze byt jen znak e, protoze se minule nacetlo '=' 
            // a taky protoze byla nastavena promenna is_first_token na true
            case (SCANNER_BLOCK_EQB):
                if (c == 'e'){
                    scanner_state = SCANNER_BLOCK_EQBE;
                }
                else{
                    error_code_exit(LEXEM_ERROR);
                }
                break;

            //muze byt jet znak g, protoze =be
            case (SCANNER_BLOCK_EQBE):
                if (c == 'g'){
                    scanner_state = SCANNER_BLOCK_EQBEG;
                }
                else{
                    error_code_exit(LEXEM_ERROR);
                }
                break;

            //muze byt jen znak i, protoze =beg
            case (SCANNER_BLOCK_EQBEG):
                if (c == 'i'){
                    scanner_state = SCANNER_BLOCK_EQBEGI;
                }
                else{
                    error_code_exit(LEXEM_ERROR);
                }
                break;

            //muze byt jen znak n, protoze =begi
            case (SCANNER_BLOCK_EQBEGI):
                if (c == 'n'){
                    scanner_state = SCANNER_BLOCK_EQBEGIN;
                }
                else{
                    error_code_exit(LEXEM_ERROR);
                }
                break;

            // je uz nactene =begin, ted se kontroluje jestli nasleduje mezera nebo EOL
            // =begin bez mezery nebo EOLu je chyba
            case (SCANNER_BLOCK_EQBEGIN): 
                if (c == '\n'){
                    is_first_token = true;
                    scanner_state = SCANNER_BLOCK_INSIDE;
                }
                else if (isspace(c)){
                    scanner_state = SCANNER_BLOCK_EQBEGIN_SPACE;               
                }
                else {
                    error_code_exit(LEXEM_ERROR);
                }
                break;

            // dokud nenajde EOL tak se nacitaji dalsi a dalsi chary do komentare
            case (SCANNER_BLOCK_EQBEGIN_SPACE):
                if (c == '\n'){
                    is_first_token = true;
                    scanner_state = SCANNER_BLOCK_INSIDE;  
                }
                else if (c == EOF){
                    error_code_exit(LEXEM_ERROR);
                }           
                break;
            
            // kdyz nastane EOL, znamena to, ze muze byt =end, protoze ten musi byt vzdy hned za novym radkem
            // znovu se kontroluje, jestli je is_first_token a taky, jestli je char '='
            // jestli ani EOl ani '=', vrati se na case, ve kterem se kontroluje EOL
            case (SCANNER_BLOCK_INSIDE):
                if (c == '\n'){
                    is_first_token = true;
                    break;  
                }
                else if (c == '=' && is_first_token){
                    is_first_token = false;
                    scanner_state = SCANNER_BLOCK_INSIDE_OR_END;
                }
                else if (c == EOF){
                    error_code_exit(LEXEM_ERROR);
                }
                else{
                    ungetc(c, stdin);
                    scanner_state = SCANNER_BLOCK_EQBEGIN_SPACE; 
                }           
                break;
            
            // podobne jako u =begin, nacita jednotlive chary
            // ale tentokat, pokud ten char neodpovida charum z =end, vrati se na case s kontrolovanim EOLu
            case (SCANNER_BLOCK_INSIDE_OR_END):
                if (c == 'e'){
                    scanner_state = SCANNER_BLOCK_EQE;
                }
                else if (c == EOF){
                    error_code_exit(LEXEM_ERROR);
                }  
                else{
                    ungetc(c, stdin);
                    scanner_state = SCANNER_BLOCK_EQBEGIN_SPACE;
                }         
                break;

            // ^^^
            case (SCANNER_BLOCK_EQE):
                if (c == 'n'){
                    
                    scanner_state = SCANNER_BLOCK_EQEN;
                }
                else if (c == EOF){
                    error_code_exit(LEXEM_ERROR);
                }  
                else{
                    ungetc(c, stdin);
                    scanner_state = SCANNER_BLOCK_EQBEGIN_SPACE;
                }         
                break;

            // ^^^
            case (SCANNER_BLOCK_EQEN):
                if (c == 'd'){
                    scanner_state = SCANNER_BLOCK_EQEND;
                }
                else if (c == EOF){
                    error_code_exit(LEXEM_ERROR);
                }  
                else{
                    ungetc(c, stdin);
                    scanner_state = SCANNER_BLOCK_EQBEGIN_SPACE;
                }         
                break;

            // kontroluje se jestli je char EOL, jestli totiz ano, tak je konec a vrati se token typ EOL
            // pokud je space, znamena to, ze je za =end mezera a budou tam jeste nejake chary
            case (SCANNER_BLOCK_EQEND):
                if (c == '\n'){
                    is_first_token = true;
                    token->type = token_type_eol;
                    return 0;
                }
                else if (isspace(c)){
                    scanner_state = SCANNER_BLOCK_EQEND_MORE;
                }
                else{
                    ungetc(c, stdin);
                    scanner_state = SCANNER_BLOCK_EQBEGIN_SPACE;
                }   
                break;

            // nacitani dalsich charu dokud neni EOL
            case (SCANNER_BLOCK_EQEND_MORE):
                if (c == '\n'){
                    is_first_token = true;
                    token->type = token_type_eol;
                    return 0;
                }
                else if (c == EOF){
                    error_code_exit(LEXEM_ERROR);
                }
                break;

            // nacitani charu dokud char neni cislo ani pismenko ani _
            // potom se porovna s klicovyma slovama, pokud se nerovna ani jedno, prejde na ID
            // tam se kontroluje jestli nezacina nahodou velkym pismenem, a taky, jestli konci ?/!
            case (SCANNER_IDENTIFIER_OR_KEYWORD):
                
                if (isalnum(c) || c =='_'){
                    strAddChar(&str, c);
                }
                else{                    
                    if (strCmpConstStr(&str, "def") == 0){
                        ungetc(c, stdin);
                        token->type = token_type_keyword;
                        token->value.keyword = keyword_def;
                        return 0;
                    } 
                    else if (strCmpConstStr(&str, "do") == 0){
                        ungetc(c, stdin);
                        token->type = token_type_keyword;
                        token->value.keyword = keyword_do;
                        return 0;
                    } 
                    else if (strCmpConstStr(&str, "else") == 0){
                        ungetc(c, stdin);
                        token->type = token_type_keyword;
                        token->value.keyword = keyword_else;
                        return 0;
                    }
                    else if (strCmpConstStr(&str, "end") == 0){
                        ungetc(c, stdin);
                        token->type = token_type_keyword;
                        token->value.keyword = keyword_end;
                        return 0;
                    }
                    else if (strCmpConstStr(&str, "if") == 0){
                        ungetc(c, stdin);
                        token->type = token_type_keyword;
                        token->value.keyword = keyword_if;
                        return 0;
                    } 
                    else if (strCmpConstStr(&str, "not") == 0){
                        ungetc(c, stdin);
                        token->type = token_type_keyword;
                        token->value.keyword = keyword_not;
                        return 0;
                    }
                    else if (strCmpConstStr(&str, "nil") == 0){
                        ungetc(c, stdin);
                        token->type = token_type_keyword;
                        token->value.keyword = keyword_nil;
                        return 0;
                    } 
                    else if (strCmpConstStr(&str, "then") == 0){
                        ungetc(c, stdin);
                        token->type = token_type_keyword;
                        token->value.keyword = keyword_then;
                        return 0;
                    } 
                    else if (strCmpConstStr(&str, "while") == 0){
                        ungetc(c, stdin);
                        token->type = token_type_keyword;
                        token->value.keyword = keyword_while;
                        return 0;
                    }
                    else{
                        if (str.length >= 1 && isupper(str.str[0])){
                            error_code_exit(LEXEM_ERROR);
                        }
                        
                        if (c == '?' || c == '!'){
                            strAddChar(&str, c);
                            scanner_state = SCANNER_QUESTION_EXCLAMATION;
                        }
                        else{
                            ungetc(c, stdin);
                            token->value.name = str.str;
                            token->type = token_type_identifier;
                            is_first_token = false;
                            return 0;
                        }
                    }
                }
                break;

            // co vsechno muze pokracovat za promennou, ktera ma na konci ?/!..
            // jakmile ho zkontroluju tak ho zahodim
            case(SCANNER_QUESTION_EXCLAMATION):
                if (c == '+'    || 
                    c == '-'    || 
                    c == '*'    || 
                    c == '/'    || 
                    isspace(c)  || 
                    c == '('    || 
                    c == '='    ||
                    c == '>'    ||
                    c == '<'    ){

                    ungetc(c, stdin);
                    token->type = token_type_identifier;
                    token->value.name = str.str;
                    return 0;
                }
                else{
                    error_code_exit(LEXEM_ERROR);
                }
                break;

            // kontrola pro cislo, desetinne cislo a taky, ze cislo nesmi zacinat napr 001
            case (SCANNER_NUMBER):
                if (isdigit(c)){
                    strAddChar(&str, c);
                }
                else if(c == '.'){
                    scanner_state = SCANNER_NUMBER_POINT;
                    strAddChar(&str, c);
                }
                else if(c == 'e'){
                    scanner_state = SCANNER_NUMBER_EXPONENT;
                    strAddChar(&str, c);
                }
                else if (c == '_' || isalpha(c)){
                    error_code_exit(LEXEM_ERROR);
                }
                else{
                    if (str.length > 1 && (str.str[0] == '0')){
                        error_code_exit(LEXEM_ERROR);
                    }
                    else{
                        ungetc(c, stdin);
                        token->type = token_type_int;
                        token->value.string = str.str;
                        //token->value.integer = atoi(str.str);     
                        return 0;
                    }
                }
                break; 
            
            //  nasla se tecka, znamena to, ze bude destinne cislo
            case (SCANNER_NUMBER_POINT):
                if (isdigit(c)){
                    strAddChar(&str, c);
                    scanner_state = SCANNER_NUMBER_DOUBLE;
                }
                else 
                    error_code_exit(LEXEM_ERROR);
                break;

            // kontrola jestli neni zapis jako exponenciala
            // jestli ne, prevede se str.str na ciselnou hodnotu
            case (SCANNER_NUMBER_DOUBLE):
                if (isdigit(c)){
                    strAddChar(&str, c);
                }
                else if(c == 'e'){
                    scanner_state = SCANNER_NUMBER_EXPONENT;
                    strAddChar(&str, c);
                }
                else{
                    ungetc(c, stdin);
                    token->type = token_type_double;
                    token->value.string = str.str;
                    //token->value.decimal = atof((&str)->str);
                    
                    return 0;                  
                }
                break;
            
            // exponent.. muze byt zapsan jako 14e-20 nebo 14e+12 nebo jen 14e2
            case (SCANNER_NUMBER_EXPONENT):
                if(isdigit(c)){
                    scanner_state = SCANNER_NUMBER_EXPONENT_LAST;
                    strAddChar(&str, c);
                }
                else if(c == '-' || c == '+'){
                    scanner_state = SCANNER_NUMBER_EXPONENT_SIGN;
                    strAddChar(&str, c);
                }
                else 
                    error_code_exit(LEXEM_ERROR);
                break;
            
            //nacteni cisel za znaminkem
            case (SCANNER_NUMBER_EXPONENT_SIGN):
                if (isdigit(c)){
                    strAddChar(&str, c);
                    scanner_state = SCANNER_NUMBER_EXPONENT_LAST;
                }
                else 
                    error_code_exit(LEXEM_ERROR);
                break;

            case (SCANNER_NUMBER_EXPONENT_LAST):
				if (isdigit(c)){
                    strAddChar(&str, c);
				}
				else{
					ungetc(c, stdin);
					strAddChar(&str, c);
                    token->type = token_type_double;
                    token->value.string = str.str;
                    //token->value.decimal = atof(str.str);
                    return 0;  
				}  
				break;
        
            // nacitani stringu. char musi byt v ASCII hodnote vetsi nez 31
            // existuje i zapis jako \n, \t, \s...
            case (SCANNER_STRING):
                if (c < 32){
                    error_code_exit(LEXEM_ERROR);
                }
                else if (c == '\\'){
                    strAddChar(&str, c);
                    scanner_state = SCANNER_STRING_ESCAPE;
                }
                else if(c == '"'){
                    token->type = token_type_string;
                    token->value.string = str.str;
                    return 0;
                }
                else {
                    strAddChar(&str, c);
                }
                break;

            // co vsechno muze byt za nactenym '\'
            case (SCANNER_STRING_ESCAPE):
                if (c == '"' || 
                    c == 'n' || 
                    c == 't' || 
                    c == 's' || 
                    c == 'x' ||
                    c == '\\'){
                    strAddChar(&str, c);
                    scanner_state = SCANNER_STRING;
                }
                else{
                    error_code_exit(LEXEM_ERROR);
                }
                break;

            // < nebo <=
            case (SCANNER_LESS_THAN):
                if(c == '='){
                    token->type = token_type_leq; 
                    return 0;
                }
                else{
                    ungetc(c, stdin);
                    token->type = token_type_less;
                    return 0;
                }
                break;

            // > nebo >=
            case(SCANNER_MORE_THAN):
                if (c == '='){
                    token->type = token_type_meq;
                    return 0;
                }
                else{
                    ungetc(c, stdin);
                    token->type = token_type_more;
                    return 0;
                }
                break;

            // dokud nenarazis na konec radku tak proste nacitej dalsi chary
            // LINE COMMENT zacina znakem #
            case(SCANNER_LINE_COMMENT):
                if (c == '\n'){
					token->type = token_type_eol;
                    return 0;
                }
                else if (c == EOF){
                    token->type = token_type_eof;
                    return 0;
                }
                break;

            // !=
            case(SCANNER_NEQ):
                if (c == '='){
                    strAddChar(&str, c);
                    token->type = token_type_neq;
                    return 0;
                }
                else{
                    error_code_exit(LEXEM_ERROR);
                }
                break;
        }
    }
    return 0;
}
