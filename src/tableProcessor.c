/*
   --------------------------------
      MarkdownLatex  
      tableProcessor.c
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

#include <stdio.h>
#include "lexer.h"
#include "parserstringops.h"
#include "stdvals.h"
#include <stdlib.h>

typedef enum {DEFAULT, LEFT, RIGHT, CENTER} alignment;

typedef struct Col {
    char l;
    char r;
   alignment align;
} Col;

Col* cols = NULL;
int numberOfCols = 0;

char toChar(alignment a) 
{
    if(a == CENTER) return 'c';
    if(a == RIGHT)  return 'r';
    if(a == LEFT)   return 'l';
    return 'X'; //default
}

int processTable(char* line1, FILE* in, FILE* out) 
{
    numberOfCols = 0;
    cols = NULL;
    int i=0;
    while(line1[i] != '\0' && line1[i] != '\n') {
        if(line1[i] == '|') {
            numberOfCols++;
        }
        i++;
    } 
    numberOfCols -= 1; //Reduce for the first and last '|' which means we get 1 more than we should

    //fprintf(stdout, "We have %d cols\n", numberOfCols);

    cols = malloc(numberOfCols * sizeof(Col));
    if(cols == NULL) {
        fprintf(stderr, "Out of memory\nUnable to process table.");
        return 1;
    }
    for(i=0; i<numberOfCols; i++) {
        Col c = {FALSE, FALSE, DEFAULT};
        cols[i] = c;
    }
    

    //Read the next line
    char c = 0;
    i = -1; //the col we are in
    do {
        c = getc(in);
        if(c == '|') {
            i++;
            c = getc(in);
            if(c == ':') {
                cols[i].l = TRUE;
            }
        } else if(c == ':') {
            c = getc(in);
            if(c == '|') {
                cols[i].r = TRUE;
                i++;
                c = getc(in);
                if(c == ':') {
                    
                    cols[i].l = TRUE;
                }
            }
        }
    } while(c != '\n' && i < numberOfCols);

    for(i=0; i<numberOfCols; i++) {
        if(cols[i].r && cols[i].l) {
            cols[i].align = CENTER;
        } else if(cols[i].r) {
            cols[i].align = RIGHT;
        } else if(cols[i].l) {
            cols[i].align = LEFT;
        }
    }

    //Do we need table x? (If there are any defaults then yes)
    char tablex = false;
    for(i=0; i<numberOfCols; i++) {
        if(cols[i].align == DEFAULT) {
            tablex = true;
            break;
        }
    }

    fprintf(out, "\\begin{center}\n\\rowcolors{3}{tableShade}{white}\n");
    if(tablex) {
        fprintf(out, "\\begin{tabularx}{\\linewidth}");
    } else {
        fprintf(out, "\\begin{tabular}");
    }
    fprintf(out, "{ ");

    for(i=0; i<numberOfCols; i++) {
        putc(toChar(cols[i].align), out);
        putc(' ', out);
        if(i < numberOfCols-1) {
            putc('|', out); putc(' ', out);
        }
    }
    fprintf(out, "}\n\\hiderowcolors\n");

    int pos = 0;
    i = 0; //number of cols we have written
    c = line1[pos];
    pos++;
    while(c != '\n' && c != '\0') {
        c = line1[pos];
        pos++;
        if(c == '\0') break;
        if(c == '|') {
            if(i != numberOfCols-1) {
                putc(' ', out);
                putc('&', out);
                putc(' ', out);
            }
            i++;
        } else if (c != '\n'){
            putc(c, out);
        }
    }
    
    fprintf(out, "\\\\\n\\showrowcolors \n\\hline\n");

    //while the lines start with a '|'
    while((c=getc(in)) == '|') {
        i=0;
        do {
            c = getc(in);
            if(c == '|') {
                if(i != -1 && i != numberOfCols-1) putc('&', out);
                i++;
            } else if (c != '\n'){
                putc(c, out);
            }
        } while(c != '\n');
        fprintf(out, "\\\\\n");
    }

    if(tablex) {
        fprintf(out, "\\end{tabularx}");
    } else {
        fprintf(out, "\\end{tabular}");
    }
    fprintf(out, "\n\\\n\\end{center}\n\\vspace{5mm}\n");

    free(cols);
    return 0;
}

/**

\begin{tabular}{ l c r }
  1 & 2 & 3 \\
  4 & 5 & 6 \\
  7 & 8 & 9 \\
\end{tabular}

**/

