#include <stdio.h>
#include "parserstringops.h"
#include "stdvals.h"

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

// 1 == all good
// 2 == buffer full
// 0 == end of file
int getLineFile(char* buf, int length, FILE* file)
{
    int i = 0;
    char c = 0;
    
    do {

        if(i >= length-1) {
            //Buffer too small!
            //Add a null character just incase the user doesn't check and tries to print it
            buf[i] = '\0';
            return 2;
        }

        c = getc(file);
        if(c == EOF) {
            return FALSE;
        } else if(c == '\n') {
            buf[i] = '\0';
            return TRUE;
        } else {
            buf[i] = c;
        }
        i++;
    } while(c != '\n');
    return i;
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

int getStringLength(char* string)
{
    int i=0;
    while(string[i] != '\0') {
        i++;
    }
    return i;
}

int compare(char* string1, char* string2) 
{
    int i = 0;
    while(string1[i] != '\0' && string2[i] != '\0') {
        if(string1[i] != string2[i]) {
            if(string1[i] < string2[i]) {
                return 1;
            } else {
                return -1;
            }
        }
        i++;
    }
    return 0; //0 for equal
}

int compareSub(char* string1, char* string2, int length) 
{
    int i = 0;
    while(string1[i] != '\0' && string2[i] != '\0') {
        if(string1[i] != string2[i]) {
            if(string1[i] < string2[i]) {
                return 1;
            } else {
                return -1;
            }
            if( i >= length) {
                return 0;
            }
        }
        i++;
    }
    return 0; //0 for equal
}
