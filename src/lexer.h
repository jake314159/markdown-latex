


typedef enum {NONE, BOLD, ITALIC, UNDERLINE, H1, H2, H3, ITEMIZE, PAGE_BREAK, LINE_BREAK, 
                CODE, HORIZONTAL_RULE, ESCAPE} FormatType;

typedef struct Symbol {
   int loc;
   FormatType type;
} Symbol;


//Takes a string and returns a FormatStruct which specifies the next symbol and how far in it is
Symbol lex(char* string);
