#include <stdio.h>
#include "lexer.h"

#define TRUE 1
#define FALSE 0

#define BUF_SIZE 600
#define END_OF_LINE_BUFFER_SIZE 50

int getLine(char* buf, int length);
int findMatch(char* string, int stringLength, char* buf, int bufSize);
int getStringLength(char* string);

//typedef enum {NONE, H1, H2, BOLD} FormatType;

typedef char bool;

//Are we are currently in a list?
bool inList = FALSE;
bool inBold = FALSE;
bool inItalic = FALSE;
bool inUnderline = FALSE;
bool isCode = FALSE;

int getLine(char* buf, int length)
{
    int i = 0;
    char c = 0;
    
    do {
        c = getchar();
        if(c == EOF) {
            return FALSE;
        } else if(c == '\n') {
            buf[i] = '\0';
            return TRUE;
        } else {
            buf[i] = c;
        }
        i++;
    } while(c != '\n' && i < length);
    return TRUE;
}

int writeStringToBuffer(char* string, char* buf, int bufIndex) {
    int i=0;
    bufIndex--;
    while(string[i] != '\0') {
        buf[++bufIndex] = string[i];
        ++i;
    }
    return TRUE;
}

int findMatch(char* string, int stringLength, char* buf, int bufSize)
{
    char endOfLineBuff[END_OF_LINE_BUFFER_SIZE]; //TODO move out of here
    int endOfLineIndex = 0;
    int j = 0;
    int i = 0;

    Symbol lineStart = lex(string);
    //printf("Symbol %d at %d\n", lineStart.type, lineStart.loc);
    
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

    for(; i<stringLength; i++, j++) {

        //Lex from the current character
        Symbol s = lex(string + i);
        s.loc += i;

        //Move up to next symbol the lexer 
        while( i < s.loc ) {
            buf[j] = string[i];
            i++;
            j++;
        }

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

            i += 2; //Go past the extra character we looked fowards too
            
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
        } else if(s.type == HORIZONTAL_RULE) {
            writeStringToBuffer("\\hrulefill", buf, j);
            j+= 10;
            buf[j] = '\0';
            return TRUE;
        } else {
            buf[j] = string[i];
        }
    }
    
    if(endOfLineIndex > 0) {
        endOfLineBuff[endOfLineIndex] = '\0';
        //printf("End of line buffer:%s :: %d :: %d\n", endOfLineBuff, endOfLineIndex, j);   
        writeStringToBuffer(endOfLineBuff, buf, j);
        j += endOfLineIndex;
    }
    buf[j] = '\0';
    return 1;
}

int getStringLength(char* string)
{
    int i=0;
    while(string[i] != '\0') {
        i++;
    }
    return i;
}

int main()
{
    printf("\\documentclass[11pt,a4paper,oneside]{report}\n\\usepackage{listings}\n\\begin{document}\n\n");
    char buf[BUF_SIZE];
    char buf2[BUF_SIZE];
    //printf("Hello world\n");
    while( getLine(buf, BUF_SIZE) > 0 ) {
        //getLine(buf, BUF_SIZE);
        findMatch(buf, getStringLength(buf), buf2, 200);
        printf("%s\n", buf2);
    }
    printf("\\end{document}\n");
}
