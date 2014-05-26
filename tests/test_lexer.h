/*
   --------------------------------
      MarkdownLatex  
      test_lexer.h
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
#include "minunit.h"
#include "../src/lexer.h"

static char* test_NONE() {
    Symbol s = lex("Here is just some text which has no markdown tokens.");
   mu_assert("Lexer no token", s.type==NONE);
   return 0;
}

static char* test_BOLD() {
    Symbol s = lex(" **BOLD.");
   mu_assert("Lexer BOLD token", s.type==BOLD && s.loc==1);
   return 0;
}

static char* test_ITALIC() {
    Symbol s = lex("It *Italic.");
   mu_assert("Lexer ITALIC token", s.type==ITALIC && s.loc==3);
   return 0;
}

static char* test_UNDERLINE() {
    Symbol s = lex(" _UNDERLINE");
   mu_assert("Lexer UNDERLINE token", s.type==UNDERLINE && s.loc==1);
   return 0;
}

static char* test_STRIKETHROUGH() {
    Symbol s = lex(" ~~STRIKETHROUGH");
   mu_assert("Lexer STRIKETHROUGH token", s.type==STRIKETHROUGH && s.loc==1);
   return 0;
}

static char* test_H1() {
    Symbol s = lex("#H1");
   mu_assert("Lexer H1 token", s.type==H1);
   return 0;
}

static char* test_H2() {
    Symbol s = lex("##H2");
   mu_assert("Lexer H2 token", s.type==H2);
   return 0;
}

static char* test_H3() {
    Symbol s = lex("###H3");
   mu_assert("Lexer H3 token", s.type==H3);
   return 0;
}

static char* test_ITEMIZE_plus() {
    Symbol s = lex("+Hello");
   mu_assert("Lexer itemize plus token", s.type==ITEMIZE);
   return 0;
}

static char* test_ITEMIZE_minus() {
    Symbol s = lex("-Hello");
   mu_assert("Lexer itemize minus token", s.type==ITEMIZE);
   return 0;
}

static char* test_ENUMERATE_dot() {
    Symbol s = lex(" 100.Hello");
   mu_assert("Lexer enumerate dot token", s.type==ENUMERATE && s.loc==1);
   return 0;
}

static char* test_ENUMERATE_bracket() {
    Symbol s = lex(" 100)Hello");
   mu_assert("Lexer enumerate bracket token", s.type==ENUMERATE && s.loc==1);
   return 0;
}

static char* test_LINE_BREAK() {
    Symbol s = lex("Some text  ");
   mu_assert("Lexer line break token", s.type==LINE_BREAK);
   return 0;
}

static char* test_TABLE_COL_SEP() {
    Symbol s = lex("| here is a table | ok |");
   mu_assert("Lexer start of table token", s.type==TABLE_COL_SEP && s.loc==0);
   return 0;
}

static char* test_CODE() {
    Symbol s = lex("  ```");
   mu_assert("Lexer code token", s.type==CODE && s.loc==2);
   return 0;
}

static char* test_RULE_MINUS() {
    Symbol s = lex("---");
   mu_assert("Lexer minus rule token", s.type==HORIZONTAL_RULE);
   return 0;
}

static char* test_RULE_STAR() {
    Symbol s = lex("***");
   mu_assert("Lexer star rule token", s.type==HORIZONTAL_RULE);
   return 0;
}

static char* test_ESCAPE() {
    Symbol s = lex("So here \\is an escape");
   mu_assert("Lexer Escape character token", s.type==ESCAPE && s.loc==8);
   return 0;
}

static char* test_COMMENT_OPEN() {
    Symbol s = lex(" <!--");
   mu_assert("Lexer comment open token", s.type==COMMENT_OPEN && s.loc==1);
   return 0;
}

static char* test_COMMENT_CLOSE() {
    Symbol s = lex(" -->");
   mu_assert("Lexer comment close token", s.type==COMMENT_CLOSE && s.loc==1);
   return 0;
}

static char* test_QUOTE_LEFT() {
    Symbol s = lex(" \"Some text");
   mu_assert("Lexer quote left token", s.type==QUOTE_LEFT && s.loc==1);
   return 0;
}

static char* test_QUOTE_RIGHT() {
    Symbol s = lex(" Some\" text");
   mu_assert("Lexer quote right token", s.type==QUOTE_RIGHT && s.loc==5);
   return 0;
}

static char* test_APOSTROPHE_LEFT() {
    Symbol s = lex(" 'Some text");
   mu_assert("Lexer apostrophe left token", s.type==APOSTROPHE_LEFT && s.loc==1);
   return 0;
}

static char* test_APOSTROPHE_RIGHT() {
    Symbol s = lex(" Some' text");
   mu_assert("Lexer apostrophe right token", s.type==APOSTROPHE_RIGHT && s.loc==5);
   return 0;
}

static char* test_LINK() {
    Symbol s = lex(" [link](http://www.link.com)");
   mu_assert("Lexer comment close token", s.type==LINK && s.loc==1);
   return 0;
}

static char* test_QUOTE_BLOCK() {
    Symbol s = lex(" > Some text!");
    mu_assert("Lexer comment close token", s.type==QUOTE_BLOCK && s.loc==1);
    return 0;
}

static char* test_IMAGE() {
    Symbol s = lex(" ![alt](http://image.jpg)");
    mu_assert("Lexer comment close token", s.type==IMAGE && s.loc==1);
    return 0;
}

static char* test_tab_tab() {
    Symbol s = lex(" \t");
    mu_assert("Lexer tab token (with \\t)", s.type==TAB && s.loc==1);
    return 0;
}

static char* test_tab_space() {
    Symbol s = lex("    ");
    mu_assert("Lexer tab token (with spaces)", s.type==TAB && s.loc==0);
    return 0;
}

static char* all_lexer_tests() {
    mu_run_test(test_NONE);
    mu_run_test(test_BOLD);
    mu_run_test(test_ITALIC);
    mu_run_test(test_UNDERLINE);
    mu_run_test(test_STRIKETHROUGH);
    mu_run_test(test_ITEMIZE_plus);
    mu_run_test(test_ITEMIZE_minus);
    mu_run_test(test_ENUMERATE_dot);
    mu_run_test(test_ENUMERATE_bracket);
    mu_run_test(test_H1);
    mu_run_test(test_H2);
    mu_run_test(test_H3);
    mu_run_test(test_LINE_BREAK);
    mu_run_test(test_TABLE_COL_SEP);
    mu_run_test(test_RULE_MINUS);
    mu_run_test(test_RULE_STAR);
    mu_run_test(test_ESCAPE);
    mu_run_test(test_COMMENT_OPEN);
    mu_run_test(test_COMMENT_CLOSE);
    mu_run_test(test_QUOTE_LEFT);
    mu_run_test(test_QUOTE_RIGHT);
    mu_run_test(test_LINK);
    mu_run_test(test_QUOTE_BLOCK);
    mu_run_test(test_IMAGE);
    mu_run_test(test_tab_tab);
    mu_run_test(test_tab_space);
    mu_run_test(test_APOSTROPHE_LEFT);
    mu_run_test(test_APOSTROPHE_RIGHT);

    return 0;
}
