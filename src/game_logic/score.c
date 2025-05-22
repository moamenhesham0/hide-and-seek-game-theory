#include <stdio.h>
#include <stdlib.h>
#include "game_logic/score.h"


struct score *score = NULL;
/* Initializes the score structure and returns struct score*/

struct score* get_score()
{
    return score;
}

void init_score()
{
    score = malloc(sizeof(struct score));
    score->seeker_score = 0;
    score->hider_score = 0;
}

/* Reset the score structure to initial values */
void reset_score(struct score *score)
{
    score->seeker_score = 0;
    score->hider_score = 0;
}

void update_score(float gain_penalty)
{
    score->seeker_score += gain_penalty;
    score->hider_score -= gain_penalty;
    printf("Updated Score: Seeker: %.2f, Hider: %.2f\n", score->seeker_score, score->hider_score);
    printf("Gain/Penalty: %.2f\n", gain_penalty);
}