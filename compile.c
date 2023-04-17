#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "stringbuf.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[])
{
	struct LexicalScanner pscanner;
	struct Parser parser;
	FILE* filp;

	assert(argc >= 2);

	filp = fopen(argv[1], "r");
	assert(filp != NULL);

	initLexicalScanner(&pscanner, filp);
	printTokens(&pscanner);
	printf("\n\n");
	parser_init(&parser, &pscanner);

	while (parser.look != NULL)
	{
		parser_stmt(&parser);
	}

	parser_free(&parser);

	freeLexicalScanner(&pscanner);

	fclose(filp);
	return 0;
}
