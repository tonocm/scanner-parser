/**********************************************************************
    Tokenizer for Java source.

    Allows unicode escapes only within strings and comments.  Otherwise,
    accepts all and only those tokens defined by the Java language
    specification.  (Note that this is significantly more than you were
    required to do for assignment 3; this solution would have received a
    lot of extra credit.)

    Tokens are classified as listed in scanner.h.

 **********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "reader.h"
#include "char_classes.h"
#include "keywords.h"
#include "scanner.h"

static void print_location (token_t *tok)
{
    fprintf(stderr, " at line %d, column %d\n",
        tok->location.line->line_num, tok->location.column);
}

/********
    Modify tok to describe next token of input.
    Update loc to refer to location immediately after tok.
 ********/
void scan(location_t * loc, token_t * tok)
{
    enum {
            start,
            in_identifier,
            got_space,
            got_nl_space,
            got_other,
            done,
        /* numbers: */
            got_dot,
            got_0,
            got_0_89,
            got_oct,
            got_dec,
            starting_hex,
            got_hex,
            got_fp_dot,
            starting_exp,
            got_exp_sign,
            got_fp_exp,
        /* character literals: */
            got_s_quote,
            got_sq_char,
            ch_esc,
            ch_oct_esc_1,
            ch_oct_esc_2,
            ch_uni_esc_1,
            ch_uni_esc_2,
            ch_uni_esc_3,
            ch_uni_esc_4,
        /* string literals: */
            in_string,
            str_esc,
            str_oct_esc_1,
            str_oct_esc_2,
            str_uni_esc_1,
            str_uni_esc_2,
            str_uni_esc_3,
            str_uni_esc_4,
        /* operators: */
            got_plus,
            got_minus,
            got_amp,
            got_bar,
            got_equals,
            got_l_than,
            got_l_than_2,
            got_g_than,
            got_g_than_2,
            got_g_than_3,
            got_star,
            got_op,
        /* comments: */
            got_slash,
            in_old_comment,
            ending_comment,
            in_new_comment
    } state = start;

/* Standard way to recognize a token: put back lookahead character that
    isn't part of current token: */
#define ACCEPT_REUSE(t) \
    *loc = loc_save;    \
    tok->length--;      \
    tok->tc = t;        \
    state = done;

/* Shortcut to eliminate final states with no out transitions: go
    ahead and accept token in previous state, but don't put back the
    lookahead: it's actually part of the token: */
#define ACCEPT(t) \
    tok->tc = t;  \
    state = done;


    /* The Program */

    tok->location = *loc;
    tok->length = 0;

    while (state != done) {
        location_t loc_save = *loc;
        int c = get_character(loc);
        tok->length++;
        switch (state) {
        
        /* Possible States */
        
        case start:
          switch (char_classes[c]) {
          case WHITE: //space, tab
            state = got_space;
            break;
          case EOLN: //newline (return)
            state = got_nl_space;
            break;
          case DOT: //.
            state = got_dot;
            break;
          case DIG_0: //0
            state = got_0;
            break;
          CASE_NZ_DIGIT: //Non Zero Digits [1 - 9]
            state = got_dec;
            break;
          case PLUS: //+
            state = got_plus;
            break;
          case MINUS: //-
            state = got_minus;
            break;
          case L_THAN: //<
            state = got_l_than;
            break;
          case G_THAN: //>
            state = got_g_than;
            break;
          case STAR: //*
            state = got_star;
            break;
          case BANG: //!
          case PCT: //%
          case CARET: //^
            state = got_op; // ops = [!, %, ^]
            break;
          case LPAREN: //(
            ACCEPT(T_LPAREN);
            break;
          case RPAREN: //)
            ACCEPT(T_RPAREN);
            break;
          case LBRAC: //[
            ACCEPT(T_LBRAC);
            break;
          case SEMIC: //;
            ACCEPT(T_SEMIC);
            break;
          case RBRAC: //]
            ACCEPT(T_RBRAC);
            break;
          case LBRACE: //{
            ACCEPT(T_LBRACE);
            break;
          case RBRACE: //}
            ACCEPT(T_RBRACE);
            break;
          case SLASH: //'/'
            state = got_slash;
            break;
          case END: //EOF
            ACCEPT_REUSE(T_EOF);
            break;
          case OTHER: //Every other possible input
            state = got_other;
            break;
          }
          break;
          
        case in_identifier:
          switch (char_classes[c]) {
          CASE_DEC_DIGIT:
            break;
          default:
            ACCEPT_REUSE(T_IDENTIFIER);
            filter_keywords(tok);
            break;
          }
          break;

        case got_space:
          switch (char_classes[c]) {
          case WHITE:
            break;  /* stay put */
          case EOLN:
            state = got_nl_space;
            break;
          default:
            ACCEPT_REUSE(T_SPACE);
            break;
          }
          break;
        
        case got_nl_space:
          switch (char_classes[c]) {
          case WHITE:
          case EOLN:
            break;  /* stay put */
          default:
            ACCEPT_REUSE(T_NL_SPACE);
            break;
          }
          break;
        
        case got_other: //stuff can get tricky
          switch (char_classes[c]) {
          case OTHER:
          case WHITE:
          case EOLN:
            break;  /* stay put */
          default:
            fprintf(stderr, "Invalid token");
            print_location(tok);
            ACCEPT_REUSE(T_SPACE);    /* most likely recovery? */
            break;
          }
          break;
        
        case done: //can it exit normally? (without errors)
          fprintf(stderr, "scan: unexpected done in switch\n");
          exit(-1);
          break;  
                    
          
        /************ operators: ************/
        case got_plus:
          switch (char_classes[c]) {
          default:
            ACCEPT_REUSE(T_OPERATOR); /* + */
            break;
          }
          break;
          
        case got_slash:               /* / */
          switch(char_classes[c]) {
          default ACCEPT_REUSE(T_OPERATOR);
          break;
          }
          break;
        
        case got_minus:
          switch (char_classes[c]) {
          default:
            ACCEPT_REUSE(T_OPERATOR); /* - */
            break;
          }
          break;
        
        case got_equals:
          switch (char_classes[c]) {
          default:
            ACCEPT_REUSE(T_EQUALS);   /* = */ /* NEED TO REMOVE THIS CASE */
            break;
          }
          break;
        
        case got_l_than:
          switch (char_classes[c]) {
          case EQUALS:
            ACCEPT(T_OPERATOR);       /* <= */ /* NEED TO REMOVE THIS CASE */
            break;
          case L_THAN:
            state = got_l_than_2;
            break;
          default:
            ACCEPT_REUSE(T_OPERATOR); /* < */
            break;
          }
          break;
        
        case got_g_than:
          switch (char_classes[c]) {
          case EQUALS:
            ACCEPT(T_OPERATOR);       /* >= */ /* NEED TO REMOVE THIS CASE */
            break;
          case G_THAN:
            state = got_g_than_2;
            break;
          default:
            ACCEPT_REUSE(T_OPERATOR); /* > */
            break;
          }
          break;
        
        case got_star:
          switch (char_classes[c]) {
          default:
            ACCEPT_REUSE(T_STAR);     /* bare '*' op */
            break;
          }
          break;
        
          /* NEED TO EXPAND THIS GOT OP, EXTRA CREDIT I GUESS */
        case got_op: /* Can be ^, %, ! */
          switch (char_classes[c]) {
          default:
            ACCEPT_REUSE(T_OPERATOR); /* bare '^' op */
            break;
          }
          break;

        /* numeric literals: */
        case got_dot:
          switch (char_classes[c]) {
          CASE_DEC_DIGIT:
            state = got_fp_dot;
            break;
          default:
            ACCEPT_REUSE(T_DOT);      /* dot */
            break;
          }
          break;
          
        case got_dec: //digits [0 - 9]
          switch (char_classes[c]) {
          CASE_DEC_DIGIT:
            break;  /* stay put */
          case DOT:
            state = got_fp_dot;
            break;
          case LET_E:
            state = starting_exp;
            break;
          default:
            ACCEPT_REUSE(T_LITERAL);  /* decimal integer */
            break;
          }
          break;
        
        case got_fp_dot: //Floats
          switch (char_classes[c]) {
          CASE_DEC_DIGIT:
            break;  /* stay put */
          CASE_LET_DF:
            ACCEPT(T_LITERAL);        /* fp w/ suffix */
            break;
          case LET_E:
            state = starting_exp;
            break;
          default:
            ACCEPT_REUSE(T_LITERAL);  /* fp */
            break;
          }
          break;
          
        case starting_exp:
          switch (char_classes[c]) {
          CASE_DEC_DIGIT:
            state = got_fp_exp;
            break;
          CASE_SIGN:
            state = got_exp_sign;
            break;
          default:
            fprintf(stderr, "Invalid floating point literal");
            print_location(tok);
            ACCEPT_REUSE(T_LITERAL);  /* punt */
            break;
          }
          break;
          
        case got_exp_sign:
          switch (char_classes[c]) {
          CASE_DEC_DIGIT:
            state = got_fp_exp;
            break;
          default:
            fprintf(stderr, "Invalid floating point literal");
            print_location(tok);
            ACCEPT_REUSE(T_LITERAL);  /* punt */
            break;
          }
          break;
          
        case got_fp_exp:
          switch (char_classes[c]) {
          CASE_DEC_DIGIT:
            break;  /* stay put */
          CASE_LET_DF:
            ACCEPT(T_LITERAL);        /* fp w/ suffix */
            break;
          default:
            ACCEPT_REUSE(T_LITERAL);  /* fp */
            break;
                }
          break;
        }
    }
}
