/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  ->  token_struct.h                                      |
 *|                                                                 |
 *| Autori  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/


#ifndef _TOKEN_STRUCT_H
#define _TOKEN_STRUCT_H



typedef enum{
    token_type_identifier,  // id           0
	token_type_keyword,     // keyword      1
	token_type_eof,         // eof          2
	token_type_eol,         // eol          3

	token_type_int,         // int          4
	token_type_double,      // double       5
	token_type_string,      // string       6

	token_type_plus,        // +            7
	token_type_minus,       // -            8
	token_type_multiply,    // *            9
	token_type_divide,      // /           	10
	token_type_assign,      // =            11

    token_type_eq,          // ==           12
	token_type_neq,         // !=           13
	token_type_less,        // <            14
	token_type_more,        // >            15
	token_type_leq,         // <=           16
	token_type_meq,         // =>           17

	token_type_comma,       // ,            18
	token_type_quote,       // "            19
	token_type_hash,        // #            20
	token_type_obracket, 	// ( 			21
	token_type_cbracket,	// ) 			22
} token_type;

typedef enum{
	keyword_def,            // 0 
	keyword_do,             // 1
	keyword_else,           // 2
	keyword_end,            // 3
	keyword_if,             // 4
	keyword_not,            // 5
	keyword_nil,			// 6
	keyword_then,           // 7
	keyword_while,          // 8
} token_keyword;

typedef struct{
	token_keyword keyword; 	// def,do,else,end,if,not,nil,then,while
	//int integer;            // 1, 2, 3....
	//float decimal;        	// 1.44, 1.55e+2, 
    char* string; 			// nejaky string
	char* name; 			// id name
} token_value;

typedef struct {
	token_type type;      
	token_value value;
} Token;

#endif
