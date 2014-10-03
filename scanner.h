/**********************************************************************
    Tokenizer for Java source.
 **********************************************************************/

#ifndef SCANNER_H
#define SCANNER_H

#include "reader.h"

typedef enum {
    T_EOF,
    T_SPACE,
    T_NL_SPACE,     /* inter-line space (includes >= 1 newlines) */
    T_OLD_COMMENT,  /* shash-star delimited */
    T_NEW_COMMENT,  /* double-slash to end of line */
    T_ID_DEC,
    T_IDENTIFIER,
    T_OPERATOR,
    T_KWOPERATOR,
    T_LITERAL,
    T_LBRACE,
    T_RBRACE,
    T_LBRAC,
    T_RBRAC,
    T_LPAREN,
    T_RPAREN,
    T_EQUALS,
    T_COLON,
    T_QMARK,
    T_SEMIC,
    T_COMMA,
    T_DOT,
    T_STAR,
    T_MODIFIER,
    T_TYPE,
    T_ATOMWORD,
    T_CLASSWORD,
    T_BREAK,
    T_CASE,
    T_CATCH,
    T_CONTINUE,
    T_DEFAULT,
    T_DO,
    T_ELSE,
    T_EXTENDS,
    T_FINALLY,
    T_FOR,
    T_GOTO,
    T_IF,
    T_IMPLEMENTS,
    T_IMPORT,
    T_PACKAGE,
    T_RETURN,
    T_SWITCH,
    T_SYNCHRONIZED,
    T_THROW,
    T_THROWS,
    T_TRY,
    T_WHILE,
    T_SLASH,
    T_PCT,
    T_PLUSPLUS,
    T_MINUSMINUS,
    T_PLUS,
    T_MINUS,
    T_NUM
} token_class;

typedef struct {
  token_class tc;
  location_t location;
  int length;     /* length of token in characters (may span lines) */
  char * data; //stores data as string. interpreted depending on token class
  double num
} token_t;

void scan(location_t * loc, token_t * tok);
    /* Modify tok to describe next token of input.
        Update loc to refer to location immediately after tok. */

#endif
