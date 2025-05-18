#ifndef LOGIC_H
#define LOGIC_H

#define MAX_CHESTS 10
#define MIN_CHESTS 2
#define SIMULATIONS 100

typedef enum {EASY = 1, NEUTRAL = 2, HARD = 3} PlaceType;

void simulate_games();
int choose_chest(int num_chests, double *probabilities);
int place_type_index(PlaceType p);
PlaceType random_place_type();

#endif // LOGIC_H