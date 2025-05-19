#include"gui/map_setup.h"
#include "gui/textures_manager.h"
#include "macros.h"
#include <stdlib.h>
#include <time.h>

static Difficulty* chests = NULL;

Difficulty generate_difficulty()
{
    static bool initialized = false;
    if (!initialized)
    {
        srand(time(NULL));
        initialized = true;
    }

    return rand() % DIFFICULTY_COUNT;
}

void init_chests(int chests_count)
{
    free(chests);
    chests = malloc(sizeof(Difficulty) * chests_count);
    for(int i = 0 ; i<chests_count ; ++i)
    {
        chests[i] = generate_difficulty();
    }
}

int** create_game_matrix(int dimension , bool is_2d)
{
    int** matrix = MATRIX(int, dimension, dimension);

    if(chests == NULL)
    {
        init_chests(dimension);
    }
    
    for(int i = 0 ; i<dimension ; ++i)
    {
        for(int j = 0 ; j<dimension ; ++j)
        {
            if(i == j)
            {
                matrix[i][j] = HIDER_LOSS[chests[i]];
            }
            else
            {
                matrix[i][j] = HIDER_GAIN[chests[i]];
            }
        }
    }

    return matrix;
}

void* generate_chests_locations(int chests_count , bool is_2d)
{
    if(is_2d && !IS_PRIME(chests_count))
    {
        return two_d_world_chests_generator(chests_count);
    }

    return linear_world_chests_generator(chests_count);
}


Button* linear_world_chests_generator(int chests_count)
{
    Button* chests = malloc(sizeof(Button) * chests_count);

    for(int i = 0 ; i<chests_count ; ++i)
    {
        chests[i].rect.x = i * (MAP_FRAME_WIDTH / chests_count);
        chests[i].rect.y = 0;
        chests[i].rect.w = MAP_FRAME_WIDTH / chests_count;
        chests[i].rect.h = MAP_FRAME_HEIGHT;
    }

    return chests;
}

Button** two_d_world_chests_generator(int chests_count)
{
    int rows = (int)sqrt(chests_count);
    int cols = (int)ceil((float)chests_count / rows);

    Button** chests = malloc(sizeof(Button*) * rows);
    for(int i = 0 ; i<rows ; ++i)
    {
        chests[i] = malloc(sizeof(Button) * cols);
    }

    for(int i = 0 ; i<rows ; ++i)
    {
        for(int j = 0 ; j<cols ; ++j)
        {
            chests[i][j].rect.x = j * (MAP_FRAME_WIDTH / cols);
            chests[i][j].rect.y = i * (MAP_FRAME_HEIGHT / rows);
            chests[i][j].rect.w = MAP_FRAME_WIDTH / cols;
            chests[i][j].rect.h = MAP_FRAME_HEIGHT / rows;
        }
    }

    return chests;
}





