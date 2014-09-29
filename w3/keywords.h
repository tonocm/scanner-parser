/**********************************************************************
    Keyword table and lookup routine.
 **********************************************************************/

#ifndef KEYWORDS_H
#define KEYWORDS_H

#include "scanner.h"

void filter_keywords(token_t * tok);
    /* Look up token (assumed to be an identifier) in a table of keywords
        and literals.  If found, change token class accordingly. */

#endif
