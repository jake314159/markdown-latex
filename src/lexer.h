/*
   --------------------------------
      MarkdownLatex  
      lexer.h
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

typedef enum {NONE, BOLD, ITALIC, UNDERLINE, STRIKETHROUGH, H1, H2, H3, ITEMIZE, ENUMERATE, PAGE_BREAK, LINE_BREAK, 
                TABLE_COL_SEP, TABLE_ROW_SEP_L, TABLE_ROW_SEP_C, TABLE_ROW_SEP_R, AMP,
                CODE, HORIZONTAL_RULE, ESCAPE, COMMENT_OPEN, COMMENT_CLOSE, QUOTE_LEFT, QUOTE_RIGHT, LINK, QUOTE_BLOCK, 
                TAB, IMAGE, H1_LINE, H2_LINE, APOSTROPHE_LEFT, APOSTROPHE_RIGHT} FormatType;

typedef struct Symbol {
   int loc;
   FormatType type;
} Symbol;


//Takes a string and returns a FormatStruct which specifies the next symbol and how far in it is
Symbol lex(char* string);
