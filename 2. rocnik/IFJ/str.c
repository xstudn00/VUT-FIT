/**
 *|=================================================================|
 *| Predmet ->  IFJ + IAL                                           |
 *| Projekt ->  Implementace prekladace imperativniho jazyka IFJ18  |
 *| Tym     ->  15                                                  |
 *| Varianta->  1.                                                  |
 *|                                                                 |
 *| Soubor  ->  str.c                                               |
 *|                                                                 |
 *| Autori  ->  Gajdos David        |  xgajdo22                     |
 *|         ->  Machacek Martin     |  xmacha73                     |
 *|         ->  Studnicka Ondrej    |  xstudn00                     |
 *|         ->  Trampeska Vaclav    |  xtramp00                     |
 *|=================================================================|
**/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "str.h"

int strInit(string *s){
    if ((s->str = (char *)malloc(8)) == NULL)
        return INTERN_ERROR;
    s->str[0] = '\0';
    s->length = 0;
    s->allocSize = 8;
    return 0;
}

void strClear(string *s){
    s->str[0] = '\0';
    s->length = 0;
    s->allocSize = 8;
}

int strAddChar(string *s1, char c){
    if (s1->length + 1 >= s1->allocSize){
        // pamet nestaci, je potreba provest realokaci
        if ((s1->str = (char *)realloc(s1->str, s1->length + 8)) == NULL)
            return 1;
        s1->allocSize = s1->length + 8;
    }
    s1->str[s1->length] = c;
    s1->length++;
    s1->str[s1->length] = '\0';
    return 0;
}

int strAddChars(string *s1, char* c){
    int i = 0;
    while(c[i] != '\0'){ 
    if (s1->length + 1 >= s1->allocSize){
        // pamet nestaci, je potreba provest realokaci
        if ((s1->str = (char *)realloc(s1->str, s1->length + 8)) == NULL)
            return 1;
        s1->allocSize = s1->length + 8;
    }
    s1->str[s1->length] = c[i];
    s1->length++;
    s1->str[s1->length] = '\0';
    i++;
    }
    return 0;
}
int strCmpConstStr(string *s1, char* s2){
    return strcmp(s1->str, s2);
}

void strFree(string *s)
{
   free(s->str);
}
