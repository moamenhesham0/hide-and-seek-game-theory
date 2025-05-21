#include"gui/game_flow/map_setup.h"
#include "gui/game/textures_manager.h"
#include "gui/game/game_engine.h"
#include "macros.h"
#include "game_logic/ai_characters/hider.h"
#include "game_logic/ai_characters/seeker.h"
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

    return (rand() % DIFFICULTY_COUNT);
}

SDL_Texture* get_chest_texure(Difficulty diff)
{
    switch (diff)
    {
    case _HARD:
        return game_engine_get_hard_chest_texture();

    case _NEUTRAL:
        return game_engine_get_nutural_chest_texture();

    default:
        return game_engine_get_easy_chest_texture();
    }
}

void set_box_rect(SDL_Rect* rect , int index)
{
    static bool initialized = false;
    if (!initialized)
    {
        srand(time(NULL));
        initialized = true;
    }

    int dim = game_engine_get_dimension();
    bool is_2d = game_engine_get_is_2d();

    rect->x = is_2d ? (rand() % (MAP_FRAME_WIDTH)) : (index * MAP_FRAME_WIDTH / dim + 50);
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

    if(!game_engine_get_is_2d())
        return false;

    for(int i = 0 ; i < index  ; ++i)
    {
        if(SDL_HasIntersection(&rect , &(chests[i].rect)) == SDL_TRUE)
            return true;
    }

    return false;
}
void check_valid_probablities()
{
    int dim = game_engine_get_dimension();
    double max = 0;
    do
    {
        double* prob = game_engine_get_is_hider() ?
        game_engine_get_seeker()->probabilities : game_engine_get_hider()->probabilities;

        max = 0;
        for(int i = 0 ; i < dim ; ++i)
        {
            max = MAX(max , prob[i]);
        }

        if(max >= 1)
            reinit_game_mat();


    }
    while(max >= 1);

}


void reinit_seeker_hider()
{
    struct seeker* seeker = initialize_hider(game_engine_get_dimension() , game_engine_get_game_matrix());
    struct hider* hider = initialize_seeker(game_engine_get_dimension() , game_engine_get_game_matrix());

    find_hider_strategy(hider , game_engine_get_dimension());
    find_seeker_strategy(seeker , game_engine_get_dimension());

    game_engine_set_hider(hider);
    game_engine_set_seeker(seeker);
}
void reinit_game_mat()
{
    free_vars();
    init_game_matrix();
    reinit_seeker_hider();
}

void free_vars()
{
    struct seeker* seeker = game_engine_get_seeker();
    struct hider* hider = game_engine_get_hider();
    FREE_MATRIX(seeker->constraints , game_engine_get_dimension());
    FREE_MATRIX(hider->constraints , game_engine_get_dimension());
    free(seeker->probabilities);
    free(hider->probabilities);
    free(seeker);
    free(hider);

    FREE_MATRIX(game_engine_get_game_matrix() , game_engine_get_dimension());
    free(game_engine_get_chests());

    game_engine_set_game_matrix(NULL);
    game_engine_set_chests(NULL);
}


void init_chests()
{
    if(game_engine_get_chests() != NULL)
        return;

    int dim = game_engine_get_dimension();
    Chest* chests = ARRAY(Chest , dim);

    for(int i = 0 ; i < dim ; ++i)
    {
        chests[i].difficulty = generate_difficulty();
        chests[i].is_hovered = false;
        chests[i].texture = get_chest_texure(chests[i].difficulty);
        do
        {
            set_box_rect(&(chests[i].rect ), i);
        }
        while(invalid_pos(chests , i));
    }

    game_engine_set_chests(chests);
}

void init_game_matrix()
{
    if(game_engine_get_chests() == NULL)
        init_chests();

    int dim = game_engine_get_dimension();
    Chest* chests = game_engine_get_chests();

    int** game_matrix = MATRIX(int , dim , dim);

    for(int i = 0 ; i < dim ; ++i)
    {
        int difficulty = chests[i].difficulty;

        for(int j = 0 ; j < dim ; ++j)
        {

            if(j == i)
                game_matrix[i][j] = SEEKER_GAIN[difficulty];

            else
                game_matrix[i][j] = HIDER_GAIN[difficulty];

        }
    }



    game_engine_set_game_matrix(game_matrix);
}





