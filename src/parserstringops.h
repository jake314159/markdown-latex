/*
   --------------------------------
      MarkdownLatex  
      parserstringops.h
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

//Gets the next line from standard output
int getLine(char* buf, int length);
int getLineFile(char* buf, int length, FILE* stream);

//writes a string to the buffer at the index position without the \0 character
int writeStringToBuffer(char* string, char* buf, int bufIndex);

//Returns the length of a string
int getStringLength(char* string);

int compare(char* string1, char* string2);
int compareSub(char* string1, char* string2, int length);

void copyFileContents(FILE* src, FILE* dest);



