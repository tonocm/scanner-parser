/**********************************************************************
    Keyword table and lookup routine.

 **********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "keywords.h"

typedef struct {
    char       * name;
    int          len;
    token_class  tc;
} keyword_t;

#define NUM_KEYWORDS   50

static keyword_t keyword_table[NUM_KEYWORDS] = {
    { "abstract",        8, T_MODIFIER },
    { "boolean",         7, T_TYPE },
    { "break",           5, T_BREAK },
    { "byte",            4, T_TYPE },
    { "case",            4, T_CASE },
    { "catch",           5, T_CATCH },
    { "char",            4, T_TYPE },
    { "class",           5, T_CLASSWORD },
    { "const",           5, T_MODIFIER },
    { "continue",        8, T_CONTINUE },
    { "default",         7, T_DEFAULT },
    { "do",              2, T_DO },
    { "double",          6, T_TYPE },
    { "else",            4, T_ELSE },
    { "extends",         7, T_EXTENDS },
    { "false",           5, T_LITERAL },
    { "final",           5, T_MODIFIER },
    { "finally",         7, T_FINALLY },
    { "float",           5, T_TYPE },
    { "for",             3, T_FOR },
    { "goto",            4, T_GOTO },
    { "if",              2, T_IF },
    { "implements",     10, T_IMPLEMENTS },
    { "import",          6, T_IMPORT },
    { "instanceof",     10, T_KWOPERATOR },
    { "int",             3, T_TYPE },
    { "interface",       9, T_CLASSWORD },
    { "long",            4, T_TYPE },
    { "native",          6, T_MODIFIER },
    { "new",             3, T_KWOPERATOR },
    { "null",            4, T_LITERAL },
    { "package",         7, T_PACKAGE },
    { "private",         7, T_MODIFIER },
    { "protected",       9, T_MODIFIER },
    { "public",          6, T_MODIFIER },
    { "return",          6, T_RETURN },
    { "short",           5, T_TYPE },
    { "static",          6, T_MODIFIER },
    { "super",           5, T_ATOMWORD },
    { "switch",          6, T_SWITCH },
    { "synchronized",   12, T_SYNCHRONIZED },
    { "this",            4, T_ATOMWORD },
    { "throw",           5, T_THROW },
    { "throws",          6, T_THROWS },
    { "transient",       9, T_MODIFIER },
    { "true",            4, T_LITERAL },
    { "try",             3, T_TRY },
    { "void",            4, T_TYPE },
    { "volatile",        8, T_MODIFIER },
    { "while",           5, T_WHILE }
};

int compar (const void * key, const void * elem)
{
    keyword_t * kw = (keyword_t *) elem;
    token_t *tok = (token_t *) key;
    
    int cmp = strncmp(&tok->location.line->data[tok->location.column],
        kw->name, kw->len);
    if (!cmp && tok->length > kw->len) {
        return 1;
    } else {
        return cmp;
    }
}

/********
    Look up token (assumed to be an identifier) in a table of keywords
    and literals.  If found, change token class accordingly.
 ********/
void filter_keywords(token_t * tok)
{
    keyword_t * p = (keyword_t *)
        bsearch((void *) tok, (void *) keyword_table,
            NUM_KEYWORDS, sizeof(keyword_t), compar);

    /* p is now either 0, indicating that tok is really an identifier,
        or a pointer into keyword_table. */

    if (p) tok->tc = p->tc;
}
