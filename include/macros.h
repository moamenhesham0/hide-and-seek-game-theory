#pragma once
#include <stdlib.h>



#define MAX(X,Y) (X>Y ? X : Y)

#define MIN(X,Y) (X<Y ? X : Y)

#define SWAP(X,Y) (                             \
    {                                           \
        if(__typeof__(X) == __typeof__(Y))      \
        {                                       \
            __typeof__(X) TEMP = X;             \
            X = Y;                              \
            Y = TEMP;                           \
        }                                       \
    }                                           \
)

/* Creates a new Matrix */
/* TYPE : data type */
/* ROW : number of rows */
/* COL : number of columns */
#define MATRIX(TYPE, ROW , COL) (                       \
    {                                                   \
    TYPE** mat;                                         \
    mat = malloc(sizeof(TYPE*)*ROW);                    \
    for(int i = 0 ; i<ROW ; ++i)                        \
        mat[i] = malloc(sizeof(TYPE) * COL);            \
    mat;                                                \
    }                                                   \
)

#define FREE_MATRIX(MAT, ROW) (                         \
    {                                                   \
        for(int i = 0 ; i<ROW ; ++i)                    \
        {                                               \
            free(MAT[i]);                               \
        }                                               \
        free(MAT);                                      \
    }                                                   \
)

/* Creates a new Array */
/* TYPE : data type */
#define ARRAY(TYPE, SIZE) (                             \
    {                                                   \
        TYPE* arr = malloc(sizeof(TYPE) * SIZE);        \
        arr;                                            \
    }                                                   \
)

#define RESIZE_ARRAY(TYPE, ARR, SIZE) (                 \
    {                                                   \
        ARR = realloc(ARR, sizeof(TYPE) * SIZE);        \
    }                                                   \
)
