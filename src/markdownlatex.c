#include <stdio.h>
#include "lexer.h"
#include "parserstringops.h"
#include "tableProcessor.h"
#include "stdvals.h"

#define BUF_SIZE 2000
#define END_OF_LINE_BUFFER_SIZE 50

int parseLine(char* string, int stringLength, FILE* in, FILE* out);
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

int parseLine(char* string, int stringLength, FILE* in, FILE* out)
{
    char endOfLineBuff[END_OF_LINE_BUFFER_SIZE]; //TODO move out of here
    int endOfLineIndex = 0;
    int i = 0;

    if(string[0] == '\n' || string[0] == '\0') {
        groupedNewLineCount++;
        if(!pageBreakPlaced && groupedNewLineCount >= 4) {
            fprintf(out, "\\newpage\n");
            pageBreakPlaced = TRUE;     
        }
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
            i++;
            //return TRUE;
        } else {
            //We just found the end of the list
            fprintf(out, "\\end{itemize}\n");
            inList = FALSE;
        }
    } else {
        //We aren't in a list but we just found a new one
        if(lineStart.type == ITEMIZE && lineStart.loc < 2) {
            inList = TRUE;
            fprintf(out, "\\begin{itemize}\n\\item");//, buf, 0);
            i++;
            //writeStringToBuffer(string, buf, 5);
            //return;
        }
    } 

    if(inNumberList) {
        if(lineStart.type == ENUMERATE && lineStart.loc < 2) {
            fprintf(out, "\\item ");
            while(string[i] != ' ') i++;
            //return TRUE;
        } else {
            //We just found the end of the list
            fprintf(out, "\\end{enumerate}\n");
            inNumberList = FALSE;
        }
    } else {
        //We aren't in a list but we just found a new one
        if(lineStart.type == ENUMERATE && lineStart.loc < 2) {
            inNumberList = TRUE;
            fprintf(out, "\\begin{enumerate}\n\\item");//, buf, 0);
            while(string[i] != ' ') i++;
        }
    }

    if(lineStart.type == HORIZONTAL_RULE && lineStart.loc < 2) {
        fprintf(out, "\\hrulefill");
        return 0;
    }

    if(lineStart.type == TABLE_COL_SEP) {
        processTable(string, in, out);
        return 0;
    }

    //Go deeper into the string looking for symbols
    for(; i<stringLength; i++) {

        //Lex from the current character
        Symbol s = lex(string + i);
        s.loc += i; //change loc to be in terms of string

        //Move up to next symbol the lexer 
        while( i < s.loc ) {
            putc(string[i], out);
            i++;
        }

        /////////////////////////////////////////////////////////////////
        // One very long else-if statement handling each possible type //
        /////////////////////////////////////////////////////////////////        
        if(isCode || s.type == CODE) {
            if(isCode && s.type == CODE) {
                fprintf(out, "\\end{lstlisting}");
                isCode = FALSE;
                i += 2;
            } else if(!isCode){
                fprintf(out, "\\begin{lstlisting}[frame=single]");
                isCode = TRUE;
                i += 2;
                break; //Skip the extra information about the syntax highlighting for now
            } else {  
                //Note skip if not code formatting BUT is within a code block
                putc(string[i], out);
            }
        } else if(s.type == ESCAPE) {
            putc(string[i+1], out);
            i += 1;
        } else if(s.type == H1) {
              //Section
            fprintf(out, "\\section*{");

            writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
            endOfLineIndex += 1; //length added to end of line
           
        } else if(s.type == H2) {
             //Subsection
            fprintf(out, "\\subsection*{");
            
            writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
            endOfLineIndex += 1; //length added to end of line

            i += 1; //Go past the extra character we looked fowards too
            
        } else if(s.type == H3) {
             //Subsubsection
            fprintf(out, "\\subsubsection*{");
            
            writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
            endOfLineIndex += 1; //length added to end of line

            i += 2; //Go past the extra characters we looked fowards too
            
        } else if(s.type == BOLD) {
            //Bold text
            if(inBold) {
                putc('}', out);
                inBold = FALSE;
                i += 1;
            } else {
                fprintf(out, "\\textbf{");
                inBold = TRUE;
                i += 1;
            }   
        } else if(s.type == ITALIC) {
            //Bold text
            if(inItalic) {
                putc('}', out);
                inItalic = FALSE;
            } else {
                fprintf(out, "\\textit{");
                inItalic = TRUE;
            }   
        } else if(s.type == UNDERLINE) {
            //Bold text
            if(inUnderline) {
                putc('}', out);
                inUnderline = FALSE;
            } else {
                fprintf(out, "\\underline{");
                inUnderline = TRUE;
            }   
        } else {
            putc(string[i], out);
        }
    }
    
    //Write anything which has been delayed till the end of line has been reached (eg. close any headings)
    if(endOfLineIndex > 0) {
        endOfLineBuff[endOfLineIndex] = '\0';
        fprintf(out, "%s", endOfLineBuff);
    }
    return 0;
}

int main ( int argc, char *argv[] )
{
    char* outFile = NULL;

    char* fontSize = NULL;
    char* doucmentType = NULL;
    char* marginSize = NULL;

    for(int i=0; i<argc-1; i++) {
        if(argv[i][0] == '-' && argv[i][1] == 'o') {
            outFile = argv[++i];
        } else if(argv[i][0] == '-' && argv[i][1] == 's') {
            //NOTE only '10pt' '11pt' and '12pt' are valid sizes
            fontSize = argv[++i];
        } else if(argv[i][0] == '-' && argv[i][1] == 'd') {
            doucmentType = argv[++i];
        } else if(argv[i][0] == '-' && argv[i][1] == 'm') {
            marginSize = argv[++i];
        }
    }

    FILE *fp = NULL;
    FILE *fout = NULL;
    if(argc > 1) {
        fp = fopen(argv[argc-1], "r"); // error check this!
        if(outFile != NULL) {
            fout = fopen(outFile, "w");
        } //else leave as stout
    }

    //Set default settings
    if(fp == NULL)              fp = stdin;
    if(fout == NULL)            fout = stdout;
    if(fontSize == NULL)        fontSize = "11pt";
    if(marginSize == NULL)      marginSize = "1.5in";
    if(doucmentType == NULL)    doucmentType = "report";
    
    fprintf(fout, "\\documentclass[%s,a4paper,oneside]{%s}\n\\usepackage{listings}\n\\usepackage[margin=%s]{geometry}\n\\begin{document}\n\n\n",
                        fontSize, doucmentType, marginSize);
    char buf[BUF_SIZE];
    //printf("Hello world\n");
    
    while( getLineFile(buf, BUF_SIZE, fp) > 0 ) {
        parseLine(buf, getStringLength(buf), fp, fout);
        putc('\n', fout);
    }
    fprintf(fout, "\\end{document}\n");
    return 0;
}
