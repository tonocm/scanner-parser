/**********************************************************************
    Java parser and pretty-printer.

    Uses a recursive-descent parser that accepts a superset of the
    language.

    At the moment it's an unreasonably large superset: basically any
    string of tokens with balanced curvy braces is accepted inside a
    class definition.  You have to fix that by fleshing out the routines
    that parse a compound_stmt.  You also have to
        - find all declared identifiers, so you can print them red
        - indent things appropriately
        - fix inter-token spaces

 **********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "reader.h"
#include "scanner.h"
#include "html.h"
#include "parser.h"

static token_t     tok;
static location_t  loc;

static int indent_level = 0;
#define    INDENT_WIDTH   4

/* forward declarations: */

static void parse_compilation_unit();

static int at_bol = 1;
     /* We're about to start a new line of output. */
static int comment_nl = 0;
     /* We've just printed a newline because of a comment.
        Don't print *another* newline if the parser asks for one. */

/********
    Print a newline.  Remember we did so, so put_token can indent
    appropriately.  Skip newline if we've already done it in the process
    of printing a comment.
 ********/
static void newline()
{
    if (comment_nl) {
        comment_nl = 0;
    } else {
        putchar('\n');
        at_bol = 1;
    }
}

/********
    Print current token.  Precede with appropriate space.
 ********/
static void put_token()
{
    if (at_bol) {
        int i = (indent_level) * INDENT_WIDTH;
        while (i--) {
            putchar(' ');
        }
    } else {
        if (tok.tc == T_NEW_COMMENT || tok.tc == T_OLD_COMMENT) {
            int i = INDENT_WIDTH;
            while (i--) {
                putchar(' ');
            }
        } else {
            putchar(' ');       /* YOU NEED TO FIX THIS; insert space
                                between tokens only when appropriate */
        }
    }
    print_token(&tok);
    at_bol = 0;
    comment_nl = 0;     /* no longer care if comment printed a newline */
}

/********
    Get next token from the scanner.  Filter out white space, comments,
    and newlines.

    Print comments appropriately:
      - If comment is preceded by a newline in input, precede it with
        newline in output.  If an old-style comment is followed by a
        newline, add a newline to the output.  These are the ONLY places
        where the formatter pays attention to white space in the input.
      - if the comment is new style (// to eoln), follow with newline
        (scanner does not include newline in comment).
      - with both of the previous rules, if the parser also decides to
        generate a newline (e.g. for a semicolon), combine them so we
        don't generate an *extra* one.
 ********/
static void get_token()
{
    token_class prev_class = T_SPACE;
    do {
        scan(&loc, &tok);
        if (tok.tc == T_NL_SPACE && prev_class == T_OLD_COMMENT) {
            /* comment was followed by a newline */
            newline();
            comment_nl = 1;     /* remember this newline */
        }
        if (tok.tc == T_OLD_COMMENT || tok.tc == T_NEW_COMMENT) {
            if (prev_class == T_NL_SPACE) {
                /* comment was preceded by a newline */
                newline();
                comment_nl = 1;     /* remember this newline */
            }
            put_token();    /* print comment itself */
            if (tok.tc == T_NEW_COMMENT) {
                /* newline is not part of comment */
                comment_nl = 0;     /* we really do want this one */
                newline();
                comment_nl = 1;     /* remember this newline */
            }
        }
        prev_class = tok.tc;
    } while (tok.tc == T_SPACE       || tok.tc == T_NL_SPACE
          || tok.tc == T_OLD_COMMENT || tok.tc == T_NEW_COMMENT);
}

/********
    A parse error has occurred.  Print error message and halt.
 ********/
static void parse_error()
{
    fprintf(stderr, "Syntax error");
    fprintf(stderr, " at line %d, column %d\n",
        tok.location.line->line_num, tok.location.column);
    exit(1);
}

/********
    A token of class tc is expected from the scanner.  Verify and print.
 ********/
static void match(token_class tc)
{
    if (tc == T_ID_DEC && tok.tc == T_IDENTIFIER) {
        tok.tc = T_ID_DEC;
    }
    if (tc != tok.tc) {
        parse_error();
    }
    put_token();
    get_token();
}

/********
    Scan source, identify structure, and print appropriately.
 ********/
void parse(){
  set_to_b`eginning(&loc);
  get_token();
  
  /* Calls the first non-termial */
  parse_E();
}


/* E -> T Etail */
static void parse_E(){
  parse_Ttail(parse_T()); 
}

/* ETail -> W T Etail | epsilon */
static void parse_Etail(){
  
  switch (tok.tc) {
    
  case MINUS: // <- Error
    match(MINUS);
    parse_Etail(parse_T());
    break;
    
  case PLUS: // <- Error
    match(PLUS);
    parse_Etail(parse_T());
    break;
    
  case PLUSPLUS: // <- CHECK HOW I DEFINED THEM
    match(PLUSPLUS);
    parse_Etail(parse_T());
    break;
    
  case MINUSMINUS: // <- CHECK HOW I DEFINED THEM
    match(MINUSMINUS);
    parse_Etail(parse_T());
    break;
    
  default:
    //epsilon
    break;
  }
}

/* T -> F Ttail */
static void parse_T(){
  parse_F(parse_Ttail());
}

/* Ttail -> * F Ttail | / F Ttail | % F Ttail | epsilon */
static void parse_Ttail(){
  
  switch (tok.tc) {
  
  case STAR:
    match(STAR);
    parse_Ttail(parse_F());
    break;
  
  case SLASH:
    match(SLASH);
    parse_Ttail(parse_F());
    break;
  
  case PCT:
    match(PCT);
    parse_Ttail(parse_F());
    break;
  
  default:
    //epsilon
    break;
  }
}

/* F -> ( E ) | num | epsilon */
static void parse_F(){
  
  switch (tok.tc) {
  
  case LPAREN:
    match(LPAREN);
    parse_E();
    match(RPAREN);
    break;

  case NUM:
    match(NUM);
    break;

  default:
    break;
  }
}
