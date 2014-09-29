/**********************************************************************
    HTML tags

 **********************************************************************/

#include <stdio.h>
#include "scanner.h"
#include "html.h"

char *file_header = "\
<html>\n\
<head>\n\
<title>Formatted Java output</title>\n\
</head>\n\
<body bgcolor=white>\n\
<pre>\n\
";

char *file_footer = "\
</pre>\n\
</body>\n\
</html>\n\
";

static struct {
    const char * pre;
    const char * post;
} token_brackets[] = {
    {/* eof             */  "",                      ""            },
    {/* space           */  "",                      ""            },
    {/* nl_space        */  "",                      ""            },
    {/* old_comment     */  "<i>",                   "</i>"        },
    {/* new_comment     */  "<i>",                   "</i>"        },
    {/* id_dec          */  "<font color=red><b>",   "</b></font>" },
    {/* ident           */  "",                      ""            },
    {/* operator        */  "",                      ""            },
    {/* kwoperator      */  "<b>",                   "</b>"        },
    {/* literal         */  "<font color=green><b>", "</b></font>" },
    {/* lbrace          */  "",                      ""            },
    {/* rbrace          */  "",                      ""            },
    {/* lbrac           */  "",                      ""            },
    {/* rbrac           */  "",                      ""            },
    {/* lparen          */  "",                      ""            },
    {/* rparen          */  "",                      ""            },
    {/* equals          */  "",                      ""            },
    {/* colon           */  "",                      ""            },
    {/* qmark           */  "",                      ""            },
    {/* semic           */  "",                      ""            },
    {/* comma           */  "",                      ""            },
    {/* dot             */  "",                      ""            },
    {/* star            */  "",                      ""            },
    {/* modifier        */  "<b>",                   "</b>"        },
    {/* type            */  "<b>",                   "</b>"        },
    {/* atomword        */  "<b>",                   "</b>"        },
    {/* classword       */  "<b>",                   "</b>"        },
    {/* kw_break        */  "<b>",                   "</b>"        },
    {/* kw_case         */  "<b>",                   "</b>"        },
    {/* kw_catch        */  "<b>",                   "</b>"        },
    {/* kw_continue     */  "<b>",                   "</b>"        },
    {/* kw_default      */  "<b>",                   "</b>"        },
    {/* kw_do           */  "<b>",                   "</b>"        },
    {/* kw_else         */  "<b>",                   "</b>"        },
    {/* kw_extends      */  "<b>",                   "</b>"        },
    {/* kw_finally      */  "<b>",                   "</b>"        },
    {/* kw_for          */  "<b>",                   "</b>"        },
    {/* kw_goto         */  "<b>",                   "</b>"        },
    {/* kw_if           */  "<b>",                   "</b>"        },
    {/* kw_implements   */  "<b>",                   "</b>"        },
    {/* kw_import       */  "<b>",                   "</b>"        },
    {/* kw_package      */  "<b>",                   "</b>"        },
    {/* kw_return       */  "<b>",                   "</b>"        },
    {/* kw_switch       */  "<b>",                   "</b>"        },
    {/* kw_synchronized */  "<b>",                   "</b>"        },
    {/* kw_throw        */  "<b>",                   "</b>"        },
    {/* kw_throws       */  "<b>",                   "</b>"        },
    {/* kw_try          */  "<b>",                   "</b>"        },
    {/* kw_while        */  "<b>",                   "</b>"        }
};

/********
    Print specified token, bracketed as appropriate.
    Assume that declaration identifiers have had their token class
    modified.

    Note that some characters, specifically <, >, &, and ",  have special
    meaning in HTML.  We need to print escape sequences to get these to
    turn out correctly in the output.
 ********/
void print_token(const token_t * tok)
{
    location_t loc = tok->location;
    int i;
    char c;
    fputs(token_brackets[tok->tc].pre, stdout);
    for (i = tok->length; i; i--) {
        switch (c = get_character(&loc)) {
            case '<':
                printf("&lt;");
                break;
            case '>':
                printf("&gt;");
                break;
            case '&':
                printf("&amp;");
                break;
            case '"':
                printf("&quot;");
                break;
            default:
                putchar(c);     /* not special to HTML */
                break;
        }
    }
    fputs(token_brackets[tok->tc].post, stdout);
}
