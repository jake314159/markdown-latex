 /* file minunit_example.c */
 
#include <stdio.h>
#include "minunit.h"
#include "../src/parserstringops.h"
 
int foo = 7;
int bar = 4;

static char* compare_equal() {
   mu_assert("Compare equal strings", compare("Hello", "Hello")==0);
   return 0;
}

static char* compare_equal_emptyString() {
   mu_assert("Compare empty equal string", compare("", "")==0);
   return 0;
}

static char* compare_lessThan() {
   mu_assert("Compare less than string", compare("hibb", "hiaa")<0);
   return 0;
}

static char* compare_greaterThan() {
   mu_assert("Compare greater than string", compare("hiaa", "hibb")>0);
   return 0;
}

static char* compare_equalSub() {
   mu_assert("Compare equal sub strings", compareSub("Hello1", "Hello2", 5)==0);
   return 0;
}

static char* compare_equalSub_notEq() {
   mu_assert("Compare not equal sub strings", compareSub("Hello12345", "Hello23456", 6)!=0);
   return 0;
}

static char* get_stringLength() {
   int value = getStringLength("1234567890");
   mu_assert("Get length of a string", value==10);
   return 0;
}

static char* test_writeToBuffer() {
   char* string = "12345";
   char buffer[40];
   writeStringToBuffer(string, buffer, 4);
   char correct = 1;
   for(int i=0; i<5; i++) {
        if(string[i] != buffer[i+4]) correct = 0;
   }
   mu_assert("Write to buffer (mid way through)", correct == 1);
   return 0;
}

static char* all_stringops_tests() {
    mu_run_test(compare_equal);
    mu_run_test(compare_equal_emptyString);
    mu_run_test(compare_lessThan);
    mu_run_test(compare_greaterThan);
    mu_run_test(compare_equalSub);
    mu_run_test(compare_equalSub_notEq);
    mu_run_test(get_stringLength);
    mu_run_test(test_writeToBuffer);
    return 0;
}
