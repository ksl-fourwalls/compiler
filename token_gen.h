#ifndef TOKEN_GEN_H
#define TOKEN_GEN_H

/*
 * type_struct is used
 * to find type of 
 * value.
 */
struct type_struct
{
    // type of variable
    unsigned type;
    size_t value;
};

struct token
{
    int tag;
    struct type_struct mtype;
};


struct _tag
{

};



#endif
