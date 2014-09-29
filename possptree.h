
#ifndef _PTREE_h
#define  _PTREE_h


/* types of Parse Tree nodes...one type per nonterminal in the grammar,
   more or less */
			
typedef enum
    {
	EXPR, TERM, TERM_TAIL, FACTOR, FACTOR_TAIL, IDENTIFIER, NUM
    } node_type;


/*  Factortails and termtails are funny; they consist of an operation
and another factortail, which must be evaluated first, then the
operation is performed between that value and the value of the factor
that is one level up the tree.  Anyway, for factortail ptnodes I need
to keep an both the operation and the next factortail.  This type
refers to the operation, and its arithmetic values are only meaningful
for factor tail and term tail PT nodes. Other nodetypes get the value
NONE */

typedef enum
    {
	TIMES, DIVIDE, PLUS, MINUS, NONE
    } op_type;


/***********************************/

/* Disclaimer: should be adequate but I can't guarantee.  Read farther
   (or copy) at your own risk. */

typedef struct ptnode{
    char * ptname;   /*strictly for humans, debugging and understanding
                     printed version of PT */
    node_type pttype; 

    char * ID_name;  /*string containing Identifier name */
    double ptvalue;  /*double containing value of a numerical constant*/
    op_type optype;  /* for term, factor tails */
    struct ptnode * first_subtree;  /* up to two sub-trees */
    struct ptnode * second_subtree;

} * ptnode_t;


/***********************************/

/* Basic utilities.
Besides the scanner and parser-ptree-builder,   
you'll   create  routines that might be called
(double) evaluate_ptree(pt_node_t ptnode) 
 and
(double) evaluate_pt_node(pt_node_t ptnode) 
for the assignment */

ptnode_t  make_ptnode (char * aname, node_type atype, char *ID_name, double avalue,
		       op_type otype, struct ptnode * one, struct ptnode * two);


void free_ptnode(ptnode_t ptnode);

void print_ptnode(ptnode_t ptnode, int lev);
void print_ptree(ptnode_t ptnode, int lev);

#endif
