


typedef enum {NONE, BOLD, ITALIC, UNDERLINE, STRIKETHROUGH, H1, H2, H3, ITEMIZE, ENUMERATE, PAGE_BREAK, LINE_BREAK, 
                TABLE_COL_SEP, TABLE_ROW_SEP_L, TABLE_ROW_SEP_C, TABLE_ROW_SEP_R,
                CODE, HORIZONTAL_RULE, ESCAPE, COMMENT_OPEN, COMMENT_CLOSE} FormatType;

typedef struct Symbol {
   int loc;
   FormatType type;
} Symbol;


//Takes a string and returns a FormatStruct which specifies the next symbol and how far in it is
Symbol lex(char* string);
