#ifndef DTYPES_H
#define DTYPES_H 1


#define CLASS       "class"
#define STRUCT      "struct"

#define SDWORD      "int"
#define SWORD       "short"
#define SBYTE       "char"

#define DWORD       "uint"
#define WORD        "ushort"
#define BYTE        "uchar"

enum {
    BYTE, WORD, DWORD, QWORD,
    SBYTE, SWORD, SDWORD, SQWORD,
    STRUCT, CLASS, TOTAL_DTYPES
} dtypes;

void *jtable[TOTAL_DTYPES] = {
    h_byte, h_word, h_dword, h_qword,
    h_sbyte, h_sword, h_sdword, h_sqword,
    h_struct, h_class 
};
#endif
