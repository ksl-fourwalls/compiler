#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "stringbuf.h"
#include "lexer.h"

int main(int argc, char *argv[])
{
	struct LexicalScanner pscanner;
	FILE* filp;

	assert(argc >= 2);

	filp = fopen(argv[1], "r");
	assert(filp != NULL);

	initLexicalScanner(&pscanner, filp);
	printTokens(&pscanner);
	freeLexicalScanner(&pscanner);

	fclose(filp);
	return 0;
}
