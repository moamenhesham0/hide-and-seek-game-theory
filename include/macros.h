#pragma once
#include <stdlib.h>


#define UNUSED __attribute__((unused))

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
        for(int i = 0 ; i<ROW&& MAT !=NULL ; ++i)       \
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

#define IS_MOUSE_INSIDE(x, y, rect) (                   \
    {                                                   \
    (x >= rect.x && x <= rect.x + rect.w) &&            \
     (y >= rect.y && y <= rect.y + rect.h);             \
    }                                                   \
)


#define IS_OVERLAPPING(OBJ_RECT, BOUND_RECT) (          \
    (OBJ_RECT.x < BOUND_RECT.x + BOUND_RECT.w) &&       \
    (OBJ_RECT.x + OBJ_RECT.w > BOUND_RECT.x) &&         \
    (OBJ_RECT.y < BOUND_RECT.y + BOUND_RECT.h) &&       \
    (OBJ_RECT.y + OBJ_RECT.h > BOUND_RECT.y)            \
)
