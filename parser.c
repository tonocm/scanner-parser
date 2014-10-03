#include <stdlib.h>
#include <stdio.h>
#include "reader.h"
#include "scanner.h"
#include "html.h"
#include "parser.h"

static token_t     tok;
static location_t  loc;
static double * tot; //this is a test
static double  temp; //test
static int indent_level = 0;
static int init =0;
#define    INDENT_WIDTH   4

/* forward declarations: */
static void Ttail();
static void Etail();
static void F();
static void T();
static void E();
static void parse_compilation_unit();

static void get_token()
{
  int i;
  //int init;

  token_class prev_class = T_SPACE;
  init = init + tok.length;
  do {

    scan(&loc, &tok);
    prev_class = tok.tc;
    
    //if(tok.tc == T_SEMIC)
      //printf("we have a semicolon... do something!\n");
      //init = 0; //reset the scanning of a line;
      
    if(tok.tc == T_NL_SPACE)
      init = 0; //resets the scanning of a line;
    
    if(tok.tc == T_SPACE)
      init = init + tok.length; //gets the length of the furthermost space
    
  } while (tok.tc == T_SPACE || tok.tc == T_NL_SPACE);

  /* Interpreting scanned token */   
  
  tok.data = (char *)malloc((tok.length)*sizeof(char)); //allocating space for the string
  
  
  /* Stores token data in token */
  for(i=0;/*i=init;*/ i < tok.length; i++){
      tok.data[i] = tok.location.line->data[init+i];
    }
  
  if(tok.tc == T_SEMIC)
    init = 0;
  
  printf("%s\n",tok.data);
  
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
  printf("Woot!\n");
  //printf("%s\n", tok.data);
    get_token();
}

void parse(){
  set_to_beginning(&loc);
  
  do{
    start_parse();
  } while(tok.tc != T_EOF); //there's more stuff to parse!
}

void start_parse(){

  get_token();
  
  /* Calls the first non-termial */
  E();
}


/* E -> T Etail */
static void E(){
  T();
  Etail();
  //parse_Etail(parse_T()); 
}

/* ETail -> W T Etail | epsilon */
static void Etail(){
  
  switch (tok.tc) {
    
  case T_MINUS:
    match(T_MINUS);
    T();
    Etail();
    //parse_Etail(parse_T());
    break;
    
  case T_PLUS: // <- Error
    match(T_PLUS);
    T();
    Etail();
    //parse_Etail(parse_T());
    break;
    
  case T_PLUSPLUS: // <- CHECK HOW I DEFINED THEM
    match(T_PLUSPLUS);
    T();
    Etail();
    //parse_Etail(parse_T());
    break;
    
  case T_MINUSMINUS: // <- CHECK HOW I DEFINED THEM
    match(T_MINUSMINUS);
    T();
    Etail();
    //parse_Etail(parse_T());
    break;
    
  default:
    //epsilon
    break;
  }
}

/* T -> F Ttail */
static void T(){
  F();
  Ttail();
  //parse_Ttail(parse_F());
}

/* Ttail -> * F Ttail | / F Ttail | % F Ttail | epsilon */
static void Ttail(){
  
  switch (tok.tc) {
  
  case T_STAR:
    match(T_STAR);
    F();
    Ttail();
    //parse_Ttail(parse_F());
    break;
  
  case T_SLASH:
    match(T_SLASH);
    F();
    Ttail();
    //parse_Ttail(parse_F());
    break;
  
  case T_PCT:
    match(T_PCT);
    F();
    Ttail();
    //parse_Ttail(parse_F());
    break;
  
  default:
    //epsilon
    break;
  }
}

/* F -> ( E ) | num | epsilon */
static void F(){
  
  switch (tok.tc) {
  
  case T_LPAREN:
    match(T_LPAREN);
    E();
    match(T_RPAREN);
    break;

  case T_NUM:
    tok.num = atof(tok.data);
    match(T_NUM);
    break;
  
  default:
    //semicolon or epsilon
    break;
  }
}
