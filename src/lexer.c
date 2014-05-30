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
    Symbol s = {0, NONE};
    int i = 0;
    
    while(string[i] != '\0' && s.type == NONE) {
        //printf("i=%d\n",i);
        if(string[i] == '\\' && string[i+1] != '\\') {
            s.loc = i;
            s.type = ESCAPE;
        } else if(string[i]=='>') {
            s.loc = i;
            s.type = QUOTE_BLOCK;
        } else if(string[i] == ' ' && string[i+1] == ' ' && string[i+2] == '\0') {
            s.loc = i;
            s.type = LINE_BREAK;
        } else if( string[i] == '<' && string[i+1] == '!' && string[i+2] == '-' && string[i+3] == '-'  ) {
            s.loc = i;
            s.type = COMMENT_OPEN;
        } else if( string[i] == '-' && string[i+1] == '-' && string[i+2] == '>') {
            s.loc = i;
            s.type = COMMENT_CLOSE;
        } else if( (string[i] == '-' && string[i+1] == '-' && string[i+2] == '-') ||
                        (string[i] == '*' && string[i+1] == '*' && string[i+2] == '*') ) {
            s.loc = i;
            s.type = HORIZONTAL_RULE;
        }else if(string[i] == '#' && string[i+1] != '#') {
            s.loc = i;
            s.type = H1;
        } else if(string[i] == '#' && string[i+1] == '#' && string[i+2] != '#' ) {
            s.loc = i;
            s.type = H2;
        } else if(string[i] == '#' && string[i+1] == '#' && string[i+2] == '#' && string[i+3] != '#' ) {
            s.loc = i;
            s.type = H3;
        } else if(string[i] == '=' && string[i+1] == '=' && string[i+2] == '=') {
            s.loc = i;
            s.type = H1_LINE;
        } else if(string[i] == '-' && string[i+1] == '-' && string[i+2] == '-') { //ERROR (This will never == true as is HORIZ rule)
            s.loc = i;
            s.type = H2_LINE;
        } else if( string[i] == '*' && string[i+1] != '*' ) {
            s.loc = i;
            s.type = ITALIC;
        } else if( string[i] == '*' && string[i+1] == '*' ) {
            s.loc = i;
            s.type = BOLD;
        } else if( string[i] == '~' && string[i+1] == '~' ) {
            s.loc = i;
            s.type = STRIKETHROUGH;
        } else if( string[i] == '+' || string[i] == '-' ) {
            s.loc = i;
            s.type = ITEMIZE;
        } else if(string[i] == '\t' || (string[i] == ' ' && string[i+1] == ' ' && string[i+2] == ' ' && string[i+3] == ' ')) {
            s.loc = i;
            s.type = TAB;
        } else if(string[i] == '_' && string[i+1] != '_') {
            s.loc = i;
            s.type = UNDERLINE;
        } else if(string[i] == '`' && string[i+1] == '`' && string[i+2] == '`') {
            s.loc = i;
            s.type = CODE;
        } else if(string[i] == '&') {
            s.loc = i;
            s.type = AMP;
        } else if(isdigit(string[i])) {
            //Get the next non number digit
            int j=i;
            while(isdigit(string[j])) j++;
            //j++;
            if(string[j] == '.' || string[j] == ')') {
                s.loc = i;
                s.type = ENUMERATE; 
            } 
        } else if(string[i] == '|') {
            s.loc = i;
            s.type = TABLE_COL_SEP;
        } else if(string[i] == '!' && string[i+1]=='[') {
            s.loc = i;
            s.type = IMAGE;
        } else if(string[i] == '[') {
            s.loc = i;
            s.type = LINK;
        } else if (string[i+1] == '"') { 
            if(string[i] == ' ') {
                s.loc = i+1;
                s.type = QUOTE_LEFT;
            } else {
                s.loc = i+1;
                s.type = QUOTE_RIGHT;
            }
        } else if (string[i+1] == '\'') { 
            if(string[i] == ' ') {
                s.loc = i+1;
                s.type = APOSTROPHE_LEFT;
            } else {
                s.loc = i+1;
                s.type = APOSTROPHE_RIGHT;
            }
        } else if(string[i] == '$' && string[i+1] == '$' && string[i+2] == '$') {
            s.loc = i;
            s.type = MATH;
        }
        i++;
    }
    
    return s;
}
