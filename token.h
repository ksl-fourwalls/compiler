#ifndef TOKEN_GEN_H
#define TOKEN_GEN_H

struct ProgramScanner {
	int peek;
	int line;
	FILE* filp;

	struct {
		struct token* INC;
		struct token* DEC;
		struct token* OR;
		struct token* AND;
		struct token* EQ;
		struct token* NE;
		struct token* LE;
		struct token* GE;
	} Word;
};

enum Tag {
	Tag_NUMBER,
	Tag_IDENTIFIER,
	Tag_UNKNOWN,
	Tag_STRING,

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

void ReadChr(struct ProgramScanner* self)
{
	self->peek = getc(self->filp);
}


struct token {
	union { 
		long long int integer; 
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

struct token* Integer(long long int value)
{
	struct token* m_Token = malloc(sizeof(struct token));
	assert(m_Token != NULL);

	m_Token->tag = Tag_NUMBER;
	m_Token->value.integer = value;
}

struct token* Token(char peek)
{
	struct token* m_Token = malloc(sizeof(struct token));
	assert(m_Token != NULL);

	m_Token->tag = Tag_UNKNOWN;
	m_Token->value.integer = (long long int)peek;
}

 
struct token* generateToken(struct ProgramScanner *self)
{
	for (;; ReadChr(self))
	{
		if (self->peek == ' ' || self->peek == '\t');
		else if (self->peek == '\n') ++self->line;
		else if (self->peek == EOF) return NULL;
		else break;
	}

	switch (self->peek)
	{
		case '&':
		{
			ReadChr(self);
			if (self->peek == '&') 
			{
				self->peek = ' ';
				return self->Word.AND;
			}
			return Token('&');
		}

		case '|':
		{
			ReadChr(self);
			if (self->peek == '|') 
			{
				self->peek = ' ';
				return self->Word.OR;
			}
			return Token('|');
		}
		case '+':
		{
			ReadChr(self);
			if (self->peek == '+') 
			{
				self->peek = ' ';
				return self->Word.INC;
			}
			return Token('-');
		}

		case '-':
		{
			ReadChr(self);
			if (self->peek == '-') 
			{
				self->peek = ' ';
				return self->Word.DEC;
			}
			return Token('-');
		}

		case '!':
		{
			ReadChr(self);
			if (self->peek == '=') 
			{
				self->peek = ' ';
				return self->Word.NE;
			}
			return Token('!');
		}

		case '>':
		{
			ReadChr(self);
			if (self->peek == '=') 
			{
				self->peek = ' ';
				return self->Word.GE;
			}
			return Token('>');
		}

		case '<':
		{
			ReadChr(self);
			if (self->peek == '=') 
			{
				self->peek = ' ';
				return self->Word.LE;
			}
			return Token('<');
		}
		case '=':
		{
			struct token *m_Token;
			ReadChr(self);
			m_Token = self->peek == '=' ? self->Word.EQ : Token('=');
			self->peek = ' ';
			return m_Token;
		}

		case '"': 
		{
			struct string m_String = { 0 };
			while (1) 
			{
				string_push(&m_String, self->peek);
				ReadChr(self);
				if (self->peek == EOF) 
					return Word(string_cstr(&m_String), Tag_STRING);

				if ( self->peek == '"' && m_String.bufptr[m_String.buflen-1] != '\\')
				{
					string_push(&m_String, self->peek);
					self->peek = ' ';
					return Word(string_cstr(&m_String), Tag_STRING);
				}
			}
		}
	}

	if (isdigit(self->peek))
	{
		int value = 0;
		do {
			value = 10 * value + (self->peek - 0x30);
			ReadChr(self);
		} while (isdigit(self->peek));
		return Integer(value);
	}

	if (isalpha(self->peek) || self->peek == '_')
	{
		struct string m_String = { 0 };
		do {
			string_push(&m_String, self->peek);
			ReadChr(self);
		} while (isalnum(self->peek) || self->peek == '_');
		return Word(string_cstr(&m_String), Tag_IDENTIFIER);
	}

	struct token *m_Token = Token(self->peek);
	self->peek = ' ';
	return m_Token;
}
#endif
