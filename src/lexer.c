/*
   --------------------------------
      MarkdownLatex  
      lexer.c
   -------------------------------- 

   Copyright 2014 Jacob Causon

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "lexer.h"
#include <ctype.h>

Symbol lex(char* string)
{
    int i = 0;
    
    while(string[i] != '\0') {
        //printf("i=%d\n",i);
        if(string[i] == '\\' && string[i+1] != '\\') {
            Symbol s = {i, ESCAPE};
            return s;
        } else if(string[i] == ' ' && string[i+1] == ' ' && string[i+2] == '\0') {
            Symbol s = {i, LINE_BREAK};
            return s;
        } else if( string[i] == '<' && string[i+1] == '!' && string[i+2] == '-' && string[i+3] == '-'  ) {
            Symbol s = {i, COMMENT_OPEN};
            return s;
        } else if( string[i] == '-' && string[i+1] == '-' && string[i+2] == '>') {
            Symbol s = {i, COMMENT_CLOSE};
            return s;
        } else if( (string[i] == '-' && string[i+1] == '-' && string[i+2] == '-') ||
                        (string[i] == '*' && string[i+1] == '*' && string[i+2] == '*') ) {
            Symbol s = {i, HORIZONTAL_RULE};
            return s;
        }else if(string[i] == '#' && string[i+1] != '#') {
            Symbol s = {i, H1};
            return s;
        } else if(string[i] == '#' && string[i+1] == '#' && string[i+2] != '#' ) {
            Symbol s = {i, H2};
            return s;
        } else if(string[i] == '#' && string[i+1] == '#' && string[i+2] == '#' && string[i+3] != '#' ) {
            Symbol s = {i, H3};
            return s;
        } else if( string[i] == '*' && string[i+1] != '*' ) {
            Symbol s = {i, ITALIC};
            return s;
        } else if( string[i] == '*' && string[i+1] == '*' ) {
            Symbol s = {i, BOLD};
            return s;
        } else if( string[i] == '~' && string[i+1] == '~' ) {
            Symbol s = {i, STRIKETHROUGH};
            return s;
        } else if( string[i] == '+' || string[i] == '-' ) {
            Symbol s = {i, ITEMIZE};
            return s;
        } else if(string[i] == '_' && string[i+1] != '_') {
            Symbol s = {i, UNDERLINE};
            return s;
        } else if(string[i] == '`' && string[i+1] == '`' && string[i+2] == '`') {
            Symbol s = {i, CODE};
            return s;
        } else if(isdigit(string[i])) {
            //Get the next non number digit
            int j=i;
            while(isdigit(string[j])) j++;
            //j++;
            if(string[j] == '.' || string[j] == ')') {
                Symbol s = {i, ENUMERATE};
                return s; 
            } 
        } else if(string[i] == '|') {
            Symbol s = {i, TABLE_COL_SEP};
            return s;
        } else if (string[i+1] == '"') { 
            if(string[i] == ' ') {
                Symbol s = {i+1, QUOTE_LEFT};
                return s;
            } else {
                Symbol s = {i+1, QUOTE_RIGHT};
                return s;
            }
        } 
        i++;
    }
    
    Symbol s = {0, NONE};
    return s;
}
