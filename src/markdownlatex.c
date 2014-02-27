#include <stdio.h>
#include "lexer.h"
#include "parserstringops.h"
#include "stdvals.h"

#define BUF_SIZE 2000
#define END_OF_LINE_BUFFER_SIZE 50

int parseLine(char* string, int stringLength, char* buf, int bufSize);
int getStringLength(char* string);

//Are we are currently in a list?
char inList = FALSE;
char inBold = FALSE;
char inItalic = FALSE;
char inUnderline = FALSE;
char isCode = FALSE;

int parseLine(char* string, int stringLength, char* buf, int bufSize)
{
    char endOfLineBuff[END_OF_LINE_BUFFER_SIZE]; //TODO move out of here
    int endOfLineIndex = 0;
    int j = 0;
    int i = 0;


    //Do the things which are only valid if they apear at the start of a line
    Symbol lineStart = lex(string);
    
    if(inList) {
        if(lineStart.type == ITEMIZE && lineStart.loc < 2) {
            writeStringToBuffer("\\item ", buf, 0);
            j += 5;
            i++;
            //return TRUE;
        } else {
            //We just found the end of the list
            writeStringToBuffer("\\end{itemize}\n", buf, 0);
            inList = FALSE;
            j += 14;
        }
    } else {
        //We aren't in a list but we just found a new one
        if(lineStart.type == ITEMIZE && lineStart.loc < 2) {
            inList = TRUE;
            writeStringToBuffer("\\begin{itemize}\n\\item", buf, 0);
            j += 21;
            i++;
            //writeStringToBuffer(string, buf, 5);
            //return;
        }
    }

    if(lineStart.type == HORIZONTAL_RULE && lineStart.loc < 2) {
        writeStringToBuffer("\\hrulefill", buf, j);
        j+= 10;
        buf[j] = '\0';
        return 0;
    }

    //Go deeper into the string looking for symbols
    for(; i<stringLength; i++, j++) {

        //Lex from the current character
        Symbol s = lex(string + i);
        s.loc += i; //change loc to be in terms of string

        //Move up to next symbol the lexer 
        while( i < s.loc ) {
            buf[j] = string[i];
            i++;
            j++;
        }

        /////////////////////////////////////////////////////////////////
        // One very long else-if statement handling each possible type //
        /////////////////////////////////////////////////////////////////        
        if(isCode || s.type == CODE) {
            if(isCode && s.type == CODE) {
                writeStringToBuffer("\\end{lstlisting}", buf, j);
                isCode = FALSE;
                i += 2;
                j += 15;
            } else if(!isCode){
                writeStringToBuffer("\\begin{lstlisting}[frame=single]", buf, j);
                isCode = TRUE;
                i += 2;
                j += 31;
            } else {  
                //Note skip if not code formatting BUT is within a code block
                buf[j] = string[i];
            }
        } else if(s.type == ESCAPE) {
            buf[j] = string[i+1];
            i += 1;
        } else if(s.type == H1) {
              //Section
            writeStringToBuffer("\\section*{", buf, j);
            j += 9; //length of the added string
            
            //endOfLineIndex += 8; //length of the added string
            writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
            endOfLineIndex += 1; //length added to end of line
           
            
        } else if(s.type == H2) {
             //Subsection
            writeStringToBuffer("\\subsection*{", buf, j);
            j += 12; //length of the added string
            
            //endOfLineIndex += 8; //length of the added string
            writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
            endOfLineIndex += 1; //length added to end of line

            i += 1; //Go past the extra character we looked fowards too
            
        } else if(s.type == H3) {
             //Subsubsection
            writeStringToBuffer("\\subsubsection*{", buf, j);
            j += 15; //length of the added string
            
            //endOfLineIndex += 8; //length of the added string
            writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
            endOfLineIndex += 1; //length added to end of line

            i += 2; //Go past the extra characters we looked fowards too
            
        } else if(s.type == BOLD) {
            //Bold text
            if(inBold) {
                writeStringToBuffer("}", buf, j);
                inBold = FALSE;
                i += 1;
                j += 0;
            } else {
                writeStringToBuffer("\\textbf{", buf, j);
                inBold = TRUE;
                i += 1;
                j += 7;
            }   
        } else if(s.type == ITALIC) {
            //Bold text
            if(inItalic) {
                writeStringToBuffer("}", buf, j);
                inItalic = FALSE;
                i += 0;
                j += 0;
            } else {
                writeStringToBuffer("\\textit{", buf, j);
                inItalic = TRUE;
                i += 0;
                j += 7;
            }   
        } else if(s.type == UNDERLINE) {
            //Bold text
            if(inUnderline) {
                writeStringToBuffer("}", buf, j);
                inUnderline = FALSE;
                i += 0;
                j += 0;
            } else {
                writeStringToBuffer("\\underline{", buf, j);
                inUnderline = TRUE;
                i += 0;
                j += 10;
            }   
        } else if(s.type == PAGE_BREAK) { 
            writeStringToBuffer("\\newpage ", buf, j);
            i += 3;
            j += 8;
        } else {
            buf[j] = string[i];
        }
    }
    
    //Write anything which has been delayed till the end of line has been reached (eg. close any headings)
    if(endOfLineIndex > 0) {
        endOfLineBuff[endOfLineIndex] = '\0';
        writeStringToBuffer(endOfLineBuff, buf, j);
        j += endOfLineIndex;
    }
    buf[j] = '\0';
    return 0;
}

int main ( int argc, char *argv[] )
{
    char* outFile = NULL;
    for(int i=0; i<argc-1; i++) {
        if(argv[i][0] == '-' && argv[i][1] == 'o') {
            outFile = argv[++i];
        }
    }
    /*if(outFile == NULL) {
        fprintf(stderr, "ERROR NO OUTPUT FILE SPECIFIED\n");
        return 0;
    }*/

    //Use std in as a default
    FILE *fp = stdin;
	FILE *fout = stdout;
    if(argc > 1) {
        fp = fopen(argv[argc-1], "r"); // error check this!
        if(outFile != NULL) {
            fout = fopen(outFile, "w");
        } //else leave as stout
    }

    if(fp == NULL)   fp = stdin;
    if(fout == NULL) fout = stdout;
	    
    fprintf(fout, "\\documentclass[11pt,a4paper,oneside]{report}\n\\usepackage{listings}\n\\begin{document}\n\n");
    char buf[BUF_SIZE];
    char buf2[BUF_SIZE];
    //printf("Hello world\n");
    
    while( getLineFile(buf, BUF_SIZE, fp) > 0 ) {
        parseLine(buf, getStringLength(buf), buf2, 200);
        fprintf(fout, "%s\n", buf2);
    }
    fprintf(fout, "\\end{document}\n");
    return 0;
}
