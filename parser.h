#ifndef TOPDOWN_PARSER
#define TOPDOWN_PARSER 1
#include "lexer.h"

struct Stmt {
};

struct Expr {
};

struct Parser {
	struct LexicalScanner *lexer;
	struct token *look;
	// hash_tree_stack* top;
};

void parser_init(struct Parser *self, struct LexicalScanner *lexer);
void parser_match(struct Parser *self, enum Tag tag);
void parser_move(struct Parser *self);
struct Stmt* parser_stmt(struct Parser *self);
void parser_free(struct Parser *self);

#endif
