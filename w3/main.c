/**********************************************************************
    When completed, this program should read a Java program
    (assumed to be syntactically correct) and produce HTML output,
    with
        keywords in bold face black
        literals in bold face green
        identifier declarations in red
        comments in italics
        pretty-printed indentation and spacing

    At the moment it tokenizes correctly, but
        (1) it only finds some of the declarations
        (2) it doesn't do spacing right
        (3) it hardly does indentation at all

    Your task is to fix these problems.

 **********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "html.h"

int main () {

    initialize_reader();
    fputs(file_header, stdout);

    parse();

    fputs(file_footer, stdout);
    finalize_reader();
    exit(0);
}
