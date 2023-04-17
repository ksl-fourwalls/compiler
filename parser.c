#include <stdio.h>
#include "lexer.h"
#include "parser.h"

static void (*error)(const char *) = perror;

void parser_init(struct Parser *self, struct LexicalScanner *lexer)
{
	//init_hash_tree(&self->top);
	self->lexer = lexer;
	parser_move(self);
}

void parser_match(struct Parser* self, enum Tag t)
{
	if (self->look->tag == t)
	{
		// ...
	}
	else error("syntax error");
}



void parser_move(struct Parser *self)
{
	self->look = generateToken(self->lexer);
}

void parser_free(struct Parser *self)
{
//	destroy_hash_tree(&self->top);
}

struct Stmt* parser_stmt(struct Parser *self)
{
	struct Expr *x;
	struct Stmt *s, *s1, *s2;

	switch (self->look->tag)
	{
		case Tag_Unknown:
			if (self->look->value.integer == (long long int)';')
			{
				printf("Token: ;\n");
			}
			break;
		case Tag_While:
			printf("While Stmt\n");
			break;

		case Tag_If:
			parser_move(self); 
			parser_match(self, '(');
		//	x = parser_bool(self);
			parser_match(self, ')');
			s1 = parser_stmt(self);

			if (self->look->tag != Tag_Else)
			{
				// TODO: if stmt to be returned
				printf("IF stmt\n");
				return NULL;
			}
			s1 = parser_stmt(self);
			printf("ELSE stmt\n");
				// TODO: else stmt to be returned 
			return NULL;
		case Tag_Do:
			
			break;
	}
	parser_move(self);
}
/*
void programQ (){
	struct Stmt* s = block_get();
	int begin = newlabel(s);


	int after = newlabel(s);

	emitlabel(begin);
	gen(s, begin, after);
}
*/