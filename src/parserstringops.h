
//Gets the next line from standard output
int getLine(char* buf, int length);
int getLineFile(char* buf, int length, FILE* stream);

//writes a string to the buffer at the index position without the \0 character
int writeStringToBuffer(char* string, char* buf, int bufIndex);

//Returns the length of a string
int getStringLength(char* string);

int compare(char* string1, char* string2);

