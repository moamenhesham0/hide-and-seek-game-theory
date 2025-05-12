#pragma once
#include <stdlib.h>

/* HELPER */
#define MAX(X,Y) X>Y ? X : Y
#define MIN(X,Y) X<Y ? X : Y

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


/* GUI CONSTS*/
#define MIN_HEIGHT_WINDOW 800
#define MIN_WIDTH_WINDOW 640


/* GAME CONSTS*/
#define DEFAULT_MATRIX_DIM 4
#define SIMULATION_RUNS 100
#define ONE_BOX_PROXIMITY 0.5
#define TWO_BOX_PROXIMITY 0.75
#define HIDER 0
#define SEEKER 1







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

#define NEW_ARRAY(TYPE, SIZE) (                         \
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
