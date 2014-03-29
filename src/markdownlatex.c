/*
   --------------------------------
      MarkdownLatex  
      markdownlatex.c
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
#include <stdlib.h> 
#include "lexer.h"
#include "parserstringops.h"
#include "tableProcessor.h"
#include "stdvals.h"
#include "versionData.h"

#define BUF_SIZE 128
#define END_OF_LINE_BUFFER_SIZE 50

int parseLine(char* string, int stringLength, FILE* in, FILE* out);
int getStringLength(char* string);

const char* colorData =
#include "colorData.txt"
    ;

int bufferSize = BUF_SIZE;

// Note that endOfLineBuff overflows are handled by ignoring any future additions to the buffer
// This will cause errors but to keep the code a bit easier to read we are just going to give it a
// Large enough size so under normal operation there will be enough space
// The only way to cause this to happen is to have over 50 headers within headers which isn't even
// Valid in markdown and even if they do it will simply generate a bad tex file and not break anything
char endOfLineBuff[END_OF_LINE_BUFFER_SIZE];

//Are we are currently in a list?
char inList = FALSE;
char inBold = FALSE;
char inItalic = FALSE;
char inUnderline = FALSE;
char inStrikethrough = FALSE;
char isCode = FALSE;
char inNumberList = FALSE;
char pageBreakPlaced = FALSE;

int quoteBlockDepth = 0;

//Have we found some markdown (rather than a comment at the start of the page)
char markdownStarted = FALSE;
char inInitialCommentBlock = FALSE;
char displayTitle = FALSE;

//How many empty new lines have we seen in a row?
int groupedNewLineCount = 0;

void outOfMemoryError()
{
    fprintf(stderr, "Out of memory\nUnable to continue\n");
    exit(100);
}

int parseLine(char* string, int stringLength, FILE* in, FILE* out)
{
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

    if(lineStart.type == COMMENT_OPEN && !markdownStarted) {
        inInitialCommentBlock = TRUE;
        return 0;
    } 
    if(lineStart.type == COMMENT_CLOSE && inInitialCommentBlock) {
        inInitialCommentBlock = FALSE;
        if(displayTitle) {
            fprintf(out, "\\maketitle\\pagebreak");
        }
        return 0;
    } else if(!markdownStarted) {
        markdownStarted = TRUE;
    }
    
    if(inInitialCommentBlock) {
        //Processes the initial comment block which can contain information about things (eg. the cover page)
        if(compareSub("title:", string, 6) == 0) {
            fprintf(out, "\\title{");
            int i=6; //start after the "title:"
            while(string[i] != '\n' && string[i] != '\0') {
                putc(string[i], out);
                i++;
            }
            putc('}', out);
            displayTitle = TRUE;
        } else if(compareSub("author:", string, 7) == 0) {
            fprintf(out, "\\author{");
            int i=7; //start after the "title:"
            while(string[i] != '\n' && string[i] != '\0') {
                putc(string[i], out);
                i++;
            }
            putc('}', out);
        } else if(compareSub("date:", string, 5) == 0) {
            fprintf(out, "\\date{");
            int i=5; //start after the "title:"
            while(string[i] != '\n' && string[i] != '\0') {
                putc(string[i], out);
                i++;
            }
            putc('}', out);
        }
        
        return 0;
    }

    if(lineStart.type == NONE) {
        // Here we can just write the entire line with no more checks because there are no symbols on this row
    }


    if(lineStart.type == QUOTE_BLOCK && !isCode) {
        int qC = 1; 
        int j = lineStart.loc+1;
        Symbol s = lex(string+1);
        
        while(s.type == QUOTE_BLOCK) {
            //fprintf(out, "%% qc= %d, quoteBlockDepth=%d, j=%d\n", qC, quoteBlockDepth, j);
            qC++;
            j += s.loc+1;
            s = lex(string+j);
        }
        //fprintf(out, "%% qc= %d, quoteBlockDepth=%d\n", qC, quoteBlockDepth);
        i = j;
        if(qC > quoteBlockDepth) {
            //we have just gone into a block
            for(int k=qC; k>quoteBlockDepth; k--) {
                fprintf(out, "\n\\begin{quote}\n");
            }
            quoteBlockDepth = qC;
        } else if(qC < quoteBlockDepth) {
            //we have just gone into a block
            for(int k=qC; k<quoteBlockDepth; k++) {
                fprintf(out, "\n\\end{quote}\n");
            }
            quoteBlockDepth = qC;
        }
        //return 0;
    } else if(quoteBlockDepth > 0 && string[0] != '\0') {
        for(int k=0; k<quoteBlockDepth; k++) {
            fprintf(out, "\n\\end{quote}\n");
        }
        quoteBlockDepth = 0;
    }
    
    if(inList) {
        if((lineStart.type == ITEMIZE && lineStart.loc < 2) || (lineStart.type == ITALIC && lineStart.loc == 0)) {
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
        if((lineStart.type == ITEMIZE && lineStart.loc < 2) || (lineStart.type == ITALIC && lineStart.loc == 0)) {
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
        // And btw if there is a * (Italic) at the start then it is probubly a list
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
                char bufCode[10];
                int iC = 3;
                while(string[iC] != '\0' && string[iC] != '\n' && string[iC] != ' ' && iC<10) {
                    bufCode[iC-3] = string[iC];
                    iC++;
                }
                bufCode[iC-3] = '\0';
                fprintf(out, "\\begin{lstlisting}[frame=single,language=%s]", bufCode);
                isCode = TRUE;
                i += 3;
                i += iC;
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

            if(endOfLineIndex < END_OF_LINE_BUFFER_SIZE) {
                writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
                endOfLineIndex += 1; //length added to end of line
            }
           
        } else if(s.type == H2) {
             //Subsection
            fprintf(out, "\\subsection*{");
            
            if(endOfLineIndex < END_OF_LINE_BUFFER_SIZE) {
                writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
                endOfLineIndex += 1; //length added to end of line
            }

            i += 1; //Go past the extra character we looked fowards too
            
        } else if(s.type == H3) {
             //Subsubsection
            fprintf(out, "\\subsubsection*{");
            
            if(endOfLineIndex < END_OF_LINE_BUFFER_SIZE) {
                writeStringToBuffer("}", endOfLineBuff, endOfLineIndex);
                endOfLineIndex += 1; //length added to end of line
            }

            i += 2; //Go past the extra characters we looked fowards too
            
        } else if(s.type == LINE_BREAK) {
            fprintf(out, "\n\\vspace{2mm}");
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
        } else if(s.type == STRIKETHROUGH) {
            //Bold text
            if(inStrikethrough) {
                putc('}', out);
                inStrikethrough = FALSE;
                i += 1;
            } else {
                fprintf(out, "\\sout{");
                inStrikethrough = TRUE;
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
        } else if(s.type == QUOTE_LEFT) {
            //add the character before but replace the " with ``
            fprintf(out, "``");
            //i++;
        } else if(s.type == QUOTE_RIGHT) {
            //add the character before but replace the " with ''
            fprintf(out, "''");
            //i++;
        } else if(s.type == IMAGE) {
            //NOTE only works with local images!
            //\includegraphics[width=10cm, height=10cm, keepaspectratio]{chick}
            fprintf(out, "\\includegraphics[width=10cm, height=10cm, keepaspectratio]{");

            //Move up to the url ignoring the alt text
            while(string[i] != '(') {
                i++;
            }
            i++;
            
            while(string[i+1] != '"' && string[i] != ')') {
                putc(string[i], out);   

                i++;
            }
            while(string[i] != ')') {
                i++;
            }
            i++;

            fprintf(out, "}\n\n");

        } else if(s.type == LINK) {
            //[This link](http://example.net/)
            //\href{http://www.wikibooks.org}{Wikibooks home}
            int j = 1;
            while(string[i+j] != ']') {
                j++;
            }
            char* namebuf = (char*) malloc(j * sizeof(char));

            j = 1;
            while(string[i+j] != ']') {
                namebuf[j-1] = string[i+j];
                j++;
            }
            namebuf[j-1] = '\0';

            fprintf(out, "\\href{");
            
            j+=2;
            while(string[i+j] != ')') {
                putc(string[i+j], out);
                j++;
            }

            fprintf(out, "}{%s} ", namebuf);

            free(namebuf);
            i += j;
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

    if (argv[argc-1][0] == '-' && argv[argc-1][1] == 'v') {
        printf("------------------------------\n  markdownlatex\n  Version: %s\n------------------------------\n", VERSION);
        return 0;
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
    
    fprintf(fout, "\\documentclass[%s,a4paper,oneside]{%s}\n\\usepackage{listings}\n\\usepackage{tabularx}\n\\usepackage[table]{xcolor}\n\\definecolor{tableShade}{gray}{0.9}\n\\usepackage{hyperref}\n\\usepackage[margin=%s]{geometry}\n\\usepackage{graphicx}\n\\usepackage{ulem}\n",
                        fontSize, doucmentType, marginSize);


    fprintf(fout, "%s\n", colorData);

    fprintf(fout, "\\begin{document}\n\n\n");
    char* buf = malloc(bufferSize * sizeof(char));
    if(buf == NULL) {
        outOfMemoryError();
    }
    
    int getLineReturnCode = getLineFile(buf, bufferSize, fp);
    while( getLineReturnCode > 0 ) {
        //If we have a buffer overflow
        if(getLineReturnCode == 2) {
            //Double buffer size in event of buffer overflow
            buf = (char*)realloc(buf, (bufferSize * sizeof(char) * 2));
            if(buf == NULL) {
                outOfMemoryError();
            }
            int oldSize = bufferSize;
            bufferSize *= 2;
            //Try to read off the rest of the line
            //Note the -1 is to ignore the \0 added by getLineFile();
            getLineReturnCode = getLineFile(buf+oldSize-1, oldSize+1, fp);
        } else {
            parseLine(buf, getStringLength(buf), fp, fout);
            putc('\n', fout);
            getLineReturnCode = getLineFile(buf, bufferSize, fp);
        }
    }
    fprintf(fout, "\\end{document}\n\n\n");
    fprintf(fout, "%%\n");
    fprintf(fout, "%%     COMPILED WITH MARKDOWN LATEX\n");
    fprintf(fout, "%%     %s\n", VERSION);
    fprintf(fout, "%%\n");
    fprintf(fout, "%%     https://github.com/jake314159/markdown-latex\n");
    fprintf(fout, "%%\n");
    fprintf(fout, "%%\n");
    fprintf(fout, "%%     Compile notes: (v=%d.%d.%d,buf=%dBto%dB,margin=%s,doc=%s)\n", V_MAJOR, V_MINOR, V_PATCH, BUF_SIZE, bufferSize, marginSize,doucmentType);
    fprintf(fout, "%%\n\n\n");
    free(buf);
    return 0;
}
