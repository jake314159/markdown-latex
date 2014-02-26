#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define BUF_SIZE 200
#define END_OF_LINE_BUFFER_SIZE 50

int getLine(char* buf, int length);
int parseLine(char* line, int lineLength, char* buf, int bufSize);
int findMatch(char* string, int stringLength, char* buf, int bufSize);
int getStringLength(char* string);

//typedef enum {NONE, H1, H2, BOLD} FormatType;

typedef char bool;

//Are we are currently in a list?
bool inList = FALSE;
bool inBold = FALSE;
bool inItalic = FALSE;

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
    
    if(inList) {
        if(string[0] == '+') {
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
        if(string[0] == '+') {
            inList = TRUE;
            writeStringToBuffer("\\begin{itemize}\n\\item", buf, 0);
            j += 21;
            i++;
            //writeStringToBuffer(string, buf, 5);
            //return;
        }
    }

    for(; i<stringLength; i++, j++) {

        if(string[i] == '#' && string[i+1] != '#') {
              //Section
            writeStringToBuffer("\\section*{", buf, j);
            j += 9; //length of the added string
            
            //endOfLineIndex += 8; //length of the added string
            writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
            endOfLineIndex += 1; //length added to end of line
           
            
        } else if(string[i] == '#' && string[i+1] == '#' && string[i+2] != '#') {
             //Subsection
            writeStringToBuffer("\\subsection*{", buf, j);
            j += 12; //length of the added string
            
            //endOfLineIndex += 8; //length of the added string
            writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
            endOfLineIndex += 1; //length added to end of line

            i += 1; //Go past the extra character we looked fowards too
            
        } else if(string[i] == '#' && string[i+1] == '#' && string[i+2] == '#' && string[i+3] != '#') {
             //Subsubsection
            writeStringToBuffer("\\subsubsection*{", buf, j);
            j += 15; //length of the added string
            
            //endOfLineIndex += 8; //length of the added string
            writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
            endOfLineIndex += 1; //length added to end of line

            i += 2; //Go past the extra character we looked fowards too
            
        } else if(string[i] == '*' && string[i+1] == '*') {
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
        } else if(string[i] == '*' && string[i+1] != '*') {
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

int parseLine(char* line, int lineLength, char* buf, int bufSize)
{
    //FormatType format = findMatch(line, lineLength);
    /*while(format != NONE) {
        
        format = findMatch(line, lineLength);
    }*/
    return 0;
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
    printf("\\documentclass[11pt,a4paper,oneside]{report}\n\\begin{document}\n\n");
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
