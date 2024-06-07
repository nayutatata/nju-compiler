#ifndef _TYPE_H_
#define _TYPE_H_
#define MAX_CH 10 // max children's len
#define NAME_LEN 20 // max id name's len
typedef enum
{
    GE,
    GT,
    LE,
    LT,
    EQ,
    NE
} reloptype;
typedef enum
{
    ttype_int,
    ttype_float
}
ttype;
#endif