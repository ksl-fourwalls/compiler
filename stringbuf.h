#ifndef _STRINGBUF_H_
#define _STRINGBUF_H_ 1

struct string {
	char* bufptr;
	size_t buflen;
	size_t size;
};

char* string_cstr(struct string* self);
void string_push(struct string* self, char pchar);
void string_cat(struct string* self, char* catstr);
bool string_getdelim(struct string* self, int delim, FILE* filp);
bool string_getuntil(struct string* self, char* peekstr, FILE* filp);
bool string_cmp(struct string* self, char *source);

#endif
