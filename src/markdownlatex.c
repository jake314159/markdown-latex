#include <stdio.h>
#include "lexer.h"
#include "parserstringops.h"
#include "stdvals.h"

#define BUF_SIZE 2000
#define END_OF_LINE_BUFFER_SIZE 50

int parseLine(char* string, int stringLength, FILE* out);
int getStringLength(char* string);

//Are we are currently in a list?
char inList = FALSE;
char inBold = FALSE;
char inItalic = FALSE;
char inUnderline = FALSE;
char isCode = FALSE;
char inNumberList = FALSE;
char pageBreakPlaced = FALSE;

//How many empty new lines have we seen in a row?
int groupedNewLineCount = 0;

int parseLine(char* string, int stringLength, FILE* out)
{
    char endOfLineBuff[END_OF_LINE_BUFFER_SIZE]; //TODO move out of here
    int endOfLineIndex = 0;
    int j = 0;
    int i = 0;


    //printf("Sring0(%d)\n", string[0]);
    //printf("Sring1(%d)\n", string[1]);
    if(string[0] == '\n' || string[0] == '\0') {
        groupedNewLineCount++;
        //printf("NEW line count is %d\n", groupedNewLineCount);
        if(!pageBreakPlaced && groupedNewLineCount >= 4) {
            fprintf(out, "\\newpage\n");
            j += 8;
            //buf[8] = '\0';
            pageBreakPlaced = TRUE;     
        }
        //return 0;
    }else if(groupedNewLineCount > 0) {
        groupedNewLineCount = 0;
        pageBreakPlaced = FALSE;
    }


    //Do the things which are only valid if they apear at the start of a line
    Symbol lineStart = lex(string);

    if(lineStart.type == NONE) {
        // Here we can just write the entire line with no more checks because there are no symbols on this row
    }
    
    if(inList) {
        if(lineStart.type == ITEMIZE && lineStart.loc < 2) {
            fprintf(out, "\\item ");
            j += 5;
            i++;
            //return TRUE;
        } else {
            //We just found the end of the list
            fprintf(out, "\\end{itemize}\n");
            inList = FALSE;
            j += 14;
        }
    } else {
        //We aren't in a list but we just found a new one
        if(lineStart.type == ITEMIZE && lineStart.loc < 2) {
            inList = TRUE;
            fprintf(out, "\\begin{itemize}\n\\item");//, buf, 0);
            j += 21;
            i++;
            //writeStringToBuffer(string, buf, 5);
            //return;
        }
    } 

    if(inNumberList) {
        if(lineStart.type == ENUMERATE && lineStart.loc < 2) {
            fprintf(out, "\\item ");
            j += 5;
            while(string[i] != ' ') i++;
            //return TRUE;
        } else {
            //We just found the end of the list
            fprintf(out, "\\end{enumerate}\n");
            inNumberList = FALSE;
            j += 14;
        }
    } else {
        //We aren't in a list but we just found a new one
        if(lineStart.type == ENUMERATE && lineStart.loc < 2) {
            inNumberList = TRUE;
            fprintf(out, "\\begin{enumerate}\n\\item");//, buf, 0);
            while(string[i] != ' ') i++;
            j += 21;
            //i++;
            //writeStringToBuffer(string, buf, 5);
            //return;
        }
    }

    if(lineStart.type == HORIZONTAL_RULE && lineStart.loc < 2) {
        fprintf(out, "\\hrulefill");
        j+= 10;
        //buf[j] = '\0';
        return 0;
    }

    //Go deeper into the string looking for symbols
    for(; i<stringLength; i++, j++) {

        //Lex from the current character
        Symbol s = lex(string + i);
        s.loc += i; //change loc to be in terms of string

        //Move up to next symbol the lexer 
        while( i < s.loc ) {
            //buf[j] = string[i];
            putc(string[i], out);
            i++;
            j++;
        }

        /////////////////////////////////////////////////////////////////
        // One very long else-if statement handling each possible type //
        /////////////////////////////////////////////////////////////////        
        if(isCode || s.type == CODE) {
            if(isCode && s.type == CODE) {
                fprintf(out, "\\end{lstlisting}");
                isCode = FALSE;
                i += 2;
                j += 15;
            } else if(!isCode){
                fprintf(out, "\\begin{lstlisting}[frame=single]");
                isCode = TRUE;
                i += 2;
                j += 32;
                break; //Skip the extra information about the syntax highlighting for now
            } else {  
                //Note skip if not code formatting BUT is within a code block
                putc(string[i], out);
            }
        } else if(s.type == ESCAPE) {
            //buf[j] = string[i+1];
            putc(string[i+1], out);
            i += 1;
        } else if(s.type == H1) {
              //Section
            fprintf(out, "\\section*{");
            j += 9; //length of the added string
            
            //endOfLineIndex += 8; //length of the added string
            writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);   //TODO WTF
            endOfLineIndex += 1; //length added to end of line
           
            
        } else if(s.type == H2) {
             //Subsection
            fprintf(out, "\\subsection*{");
            j += 12; //length of the added string
            
            //endOfLineIndex += 8; //length of the added string
            writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
            endOfLineIndex += 1; //length added to end of line

            i += 1; //Go past the extra character we looked fowards too
            
        } else if(s.type == H3) {
             //Subsubsection
            fprintf(out, "\\subsubsection*{");
            j += 15; //length of the added string
            
            //endOfLineIndex += 8; //length of the added string
            writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
            endOfLineIndex += 1; //length added to end of line

            i += 2; //Go past the extra characters we looked fowards too
            
        } else if(s.type == BOLD) {
            //Bold text
            if(inBold) {
                putc('}', out);
                inBold = FALSE;
                i += 1;
                j += 0;
            } else {
                fprintf(out, "\\textbf{");
                inBold = TRUE;
                i += 1;
                j += 7;
            }   
        } else if(s.type == ITALIC) {
            //Bold text
            if(inItalic) {
                putc('}', out);
                inItalic = FALSE;
                i += 0;
                j += 0;
            } else {
                fprintf(out, "\\textit{");
                inItalic = TRUE;
                i += 0;
                j += 7;
            }   
        } else if(s.type == UNDERLINE) {
            //Bold text
            if(inUnderline) {
                putc('}', out);
                inUnderline = FALSE;
                i += 0;
                j += 0;
            } else {
                fprintf(out, "\\underline{");
                inUnderline = TRUE;
                i += 0;
                j += 10;
            }   
        } else if(s.type == PAGE_BREAK) { 
            fprintf(out, "\\newpage ");
            i += 3;
            j += 8;
        } else {
            putc(string[i], out);
        }
    }
    
    //Write anything which has been delayed till the end of line has been reached (eg. close any headings)
    if(endOfLineIndex > 0) {
        endOfLineBuff[endOfLineIndex] = '\0';
        fprintf(out, "%s", endOfLineBuff);
        j += endOfLineIndex;
    }
    //buf[j] = '\0';
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
    //printf("Hello world\n");
    
    while( getLineFile(buf, BUF_SIZE, fp) > 0 ) {
        parseLine(buf, getStringLength(buf), fout);
        putc('\n', fout);
    }
    fprintf(fout, "\\end{document}\n");
    return 0;
}
