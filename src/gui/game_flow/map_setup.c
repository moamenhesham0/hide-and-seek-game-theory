#include"gui/game_flow/map_setup.h"
#include "gui/game/textures_manager.h"
#include "gui/game/game_engine.h"
#include "macros.h"
#include "gui/game_flow/boundries.h"
#include <stdlib.h>
#include <time.h>


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

SDL_Texture* get_chest_texure(Difficulty diff)
{
    switch (diff)
    {
    case EASY:
        return game_engine_get_easy_chest_texture();

    case HARD:
        return game_engine_get_hard_chest_texture();

    default:
        return game_engine_get_nutural_chest_texture();
    }
}

void set_box_rect(SDL_Rect* rect)
{
    static bool initialized = false;
    if (!initialized)
    {
        srand(time(NULL));
        initialized = true;
    }

    int dim = game_engine_get_dimension();
    bool is_2d = game_engine_get_is_2d();

    rect->x = rand() % (MAP_FRAME_WIDTH);
    rect->y = rand() % (MAP_FRAME_HEIGHT);
    rect->w = CHEST_FRAME_WIDTH/16;
    rect->h = CHEST_FRAME_HEIGHT/16;


}

bool invalid_pos(Chest* chests , int index)
{
    SDL_Rect rect = chests[index].rect;
    if(rect.x < 50 || rect.x > MAP_FRAME_WIDTH - 50 ||
       rect.y < 30 || rect.y > MAP_FRAME_HEIGHT - 30)
        return true;
    for(int i = 0 ; i< BOUNDRIES_RECTS_SIZE ; ++i)
    {
        if(SDL_HasIntersection(&rect , &(BOUNDRIES[i])) == SDL_TRUE)
            return true;
    }

    for(int i = 0 ; i < index ; ++i)
    {
        if(SDL_HasIntersection(&rect , &(chests[i].rect)) == SDL_TRUE)
            return true;
    }

    return false;
}


void init_chests()
{
    int dim = game_engine_get_dimension();
    bool is_2d = game_engine_get_is_2d();
    Chest* chests = ARRAY(Chest , dim);

    for(int i = 0 ; i < dim ; ++i)
    {
        chests[i].difficulty = generate_difficulty();
        chests[i].is_hovered = false;
        chests[i].texture = get_chest_texure(chests[i].difficulty);
        do
        {
            set_box_rect(&(chests[i].rect));
        }
        while(invalid_pos(chests , i));
    }

    game_engine_set_chests(chests);
}

void init_game_matrix()
{
    int dim = game_engine_get_dimension();
    Chest* chests = game_engine_get_chests();

    if(chests == NULL)
        init_chests();

    int** game_matrix = MATRIX(int , dim , dim);

    for(int i = 0 ; i < dim ; ++i)
    {
        int difficulty = chests[i].difficulty;

        for(int j = 0 ; j < dim ; ++j)
        {

            if(j == i)
                game_matrix[HIDER_LOSS[difficulty]];

            else
                game_matrix[HIDER_GAIN[difficulty]];

        }
    }

    game_engine_set_game_matrix(game_matrix);
}





