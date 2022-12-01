#ifndef TOKEN_GEN_H
#define TOKEN_GEN_H

struct ProgramScanner {
	int peek;
	int line;
	FILE* filp;

	struct {
		struct token *INC, *DEC, *OR, 
			*AND, *EQ, *NE, *LE, *GE;
	} Word;
};

enum Tag {
	Tag_Number,
	Tag_Unknown,
	Tag_String,
	Tag_Float,
	Tag_Identifier,
	Tag_Char,
	Tag_Comment,

	Tag_INC,
	Tag_DEC,
	Tag_OR,
	Tag_AND,
	Tag_EQ,
	Tag_NE,
	Tag_LE,
	Tag_GE,
};

void initProgramScanner(struct ProgramScanner* self, FILE *filp)
{
	struct token* Word(char* string, enum Tag tag);

	self->peek = ' ';
	self->line = 1;
	self->filp = filp;

	self->Word.INC = Word("++", Tag_INC);
	self->Word.DEC = Word("--", Tag_DEC);

	self->Word.OR = Word("||", Tag_OR);
	self->Word.AND = Word("&&", Tag_AND);
	self->Word.EQ = Word("==", Tag_EQ);
	self->Word.NE = Word("!=", Tag_NE);
	self->Word.LE = Word(">=", Tag_LE);
	self->Word.GE = Word("<=", Tag_GE);
}



struct token {
	union { 
		long long int integer; 
		double floatval;
		char* word; 
	} value;

	int tag;
	int attribute;

};

struct token* Word(char* string, enum Tag tag)
{
	struct token* m_Token = malloc(sizeof(struct token));
	assert(m_Token != NULL);

	m_Token->tag = tag;
	m_Token->value.word = string;
}

struct token* Float(double value)
{
	struct token* m_Token = malloc(sizeof(struct token));
	assert(m_Token != NULL);
	m_Token->tag = Tag_Float;
	m_Token->value.floatval = value;
}

struct token* Integer(long long int value)
{
	struct token* m_Token = malloc(sizeof(struct token));
	assert(m_Token != NULL);

	m_Token->tag = Tag_Number;
	m_Token->value.integer = value;
}

struct token* Token(char peek, enum Tag tag)
{
	struct token* m_Token = malloc(sizeof(struct token));
	assert(m_Token != NULL);

	m_Token->tag = tag;
	m_Token->value.integer = (long long int)peek;
}

 
struct token* generateToken(struct ProgramScanner *self)
{
	for (;; self->peek = getc(self->filp))
	{
		if (self->peek == ' ' || self->peek == '\t');
		else if (self->peek == '\n') ++self->line;
		else if (self->peek == EOF) return NULL;
		else break;
	}

	switch (self->peek)
	{
		case '/':
		{
			self->peek = getc(self->filp);
			if (self->peek == '/')
			{
				struct string m_String = { 0 };
				// start of comment
				string_cat(&m_String, "//");
				self->peek = string_getdelim(&m_String, '\n', self->filp) ? EOF : ' ';
				return Word(string_cstr(&m_String), Tag_Comment);
			}
			else if (self->peek == '*')
			{
				struct string m_String = { 0 };
				string_cat(&m_String, "/*");
				self->peek = string_getuntil(&m_String, "*/", self->filp) ? EOF : ' ';
				return Word(string_cstr(&m_String), Tag_Comment);
			}
			return Token('/', Tag_Unknown);
		}

		case '&':
		{
			self->peek = getc(self->filp);
			if (self->peek == '&') 
			{
				self->peek = ' ';
				return self->Word.AND;
			}
			return Token('&', Tag_Unknown);
		}

		case '|':
		{
			self->peek = getc(self->filp);
			if (self->peek == '|') 
			{
				self->peek = ' ';
				return self->Word.OR;
			}
			return Token('|', Tag_Unknown);
		}
		case '+':
		{
			self->peek = getc(self->filp);
			if (self->peek == '+') 
			{
				self->peek = ' ';
				return self->Word.INC;
			}
			return Token('-', Tag_Unknown);
		}

		case '-':
		{
			self->peek = getc(self->filp);
			if (self->peek == '-') 
			{
				self->peek = ' ';
				return self->Word.DEC;
			}
			return Token('-', Tag_Unknown);
		}

		case '!':
		{
			self->peek = getc(self->filp);
			if (self->peek == '=') 
			{
				self->peek = ' ';
				return self->Word.NE;
			}
			return Token('!', Tag_Unknown);
		}

		case '>':
		{
			self->peek = getc(self->filp);
			if (self->peek == '=') 
			{
				self->peek = ' ';
				return self->Word.GE;
			}
			return Token('>', Tag_Unknown);
		}

		case '<':
		{
			self->peek = getc(self->filp);
			if (self->peek == '=') 
			{
				self->peek = ' ';
				return self->Word.LE;
			}
			return Token('<',Tag_Unknown );
		}
		case '=':
		{
			struct token *m_Token;
			self->peek = getc(self->filp);
			m_Token = self->peek == '=' ? self->Word.EQ : Token('=', Tag_Unknown);
			self->peek = ' ';
			return m_Token;
		}


		case '"': 
		{
			struct string m_String = { 0 };
			while (1) 
			{
				string_push(&m_String, self->peek);
				self->peek = getc(self->filp);
				if (self->peek == EOF) 
					return Word(string_cstr(&m_String), Tag_String);

				if ( self->peek == '"' && m_String.bufptr[m_String.buflen-1] != '\\')
				{
					string_push(&m_String, self->peek);
					self->peek = ' ';
					return Word(string_cstr(&m_String), Tag_String);
				}
			}
		}
	}

	if (isdigit(self->peek))
	{
		int value = 0;
		do {
			value = 10 * value + (self->peek - 0x30);
			self->peek = getc(self->filp);
		} while (isdigit(self->peek));
		if (self->peek != '.') return Integer(value);
		float x = value;
		float d = 10.0;

		for (;;) {
			self->peek = getc(self->filp);
			if (!isdigit(self->peek)) break;
			x = x + (float)(self->peek-0x30) / d;
			d = d * 10;
		}
		return Float(x);
	}

	if (isalpha(self->peek) || self->peek == '_')
	{
		struct string m_String = { 0 };
		do {
			string_push(&m_String, self->peek);
			self->peek = getc(self->filp);
		} while (isalnum(self->peek) || self->peek == '_');
		return Word(string_cstr(&m_String), Tag_Identifier);
	}

	struct token *m_Token = Token(self->peek, Tag_Unknown);
	self->peek = ' ';
	return m_Token;
}
#endif
