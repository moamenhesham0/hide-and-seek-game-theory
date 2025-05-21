#ifndef SCORE_H
#define SCORE_H
struct score
{
    float seeker_score;
    float hider_score;
};

struct score *score;
void reset_score(struct score *score);
void init_score();
void update_score(float gain_penalty);

#endif  // SCORE_H