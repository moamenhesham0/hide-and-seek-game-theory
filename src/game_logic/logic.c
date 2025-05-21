#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_logic/ai_characters/hider.h"
#include "game_logic/ai_characters/seeker.h"
#include "game_logic/logic.h"


// Rewards for seeker guesses and hider hides when guess is correct, by place type
const int seeker_rewards[] = {1, 2, 3};  // EASY=1, NEUTRAL=2, HARD=3
const int hider_rewards[]  = {3, 2, 1};  // EASY=3, NEUTRAL=2, HARD=1

const int penalties[] = {-1, -2, -3};   // EASY, NEUTRAL, HARD

PlaceType random_place_type() {
    int r = rand() % 3;
    if (r == 0) return EASY;
    else if (r == 1) return NEUTRAL;
    else return HARD;
}

// Map place type to index 0..2 for arrays
int place_type_index(PlaceType p) {
    return (int)p - 1;
}

/*
 * Choose a chest based on the probabilities array.
 * If all probabilities are zero, choose randomly.
 */
int choose_chest(int num_chests, double *probabilities) {
    double r = (double)rand() / RAND_MAX;  // random number between 0 and 1
    double cum = 0.0;

    for (int i = 0; i < num_chests; i++) {
        cum += probabilities[i];

        if (r < cum) return i;
    }

    return num_chests - 1;  // fallback if rounding errors occur
}

/*
 * Simulate a number of games with random chest placements and strategies.
 * This function initializes the game, runs the simulations, and prints results.
 */
void simulate_games() {
    srand(time(NULL));

    int num_chests = 4;

    int fixed_payoff[4][4] = {
        { 3, -1, -1, -1},
        {-2,  1, -2, -2},
        {-2, -2,  1, -2},
        {-1, -1, -1,  3}
    };

    int **payoff_matrix = malloc(num_chests * sizeof(int *));
    for (int i = 0; i < num_chests; i++) {
        payoff_matrix[i] = malloc(num_chests * sizeof(int));
        for (int j = 0; j < num_chests; j++) {
            payoff_matrix[i][j] = fixed_payoff[i][j];
        }
    }

    struct hider* best_hider = initialize_hider(num_chests, payoff_matrix);
    struct seeker* best_seeker = initialize_seeker(num_chests, payoff_matrix);

    find_hider_strategy(best_hider, num_chests);
    find_seeker_strategy(best_seeker, num_chests);

    printf("Computed Hider Strategy:\n");
    for (int i = 0; i < num_chests; i++) {
        printf("Chest %d: %.4f\n", i + 1, best_hider->probabilities[i]);
    }

    printf("Computed Seeker Strategy:\n");
    for (int i = 0; i < num_chests; i++) {
        printf("Chest %d: %.4f\n", i + 1, best_seeker->probabilities[i]);
    }

    int seeker_score = 0;
    int hider_score = 0;

    // Counters for how many times each chest is picked
    int seeker_picks[4] = {0, 0, 0, 0};
    int hider_picks[4] = {0, 0, 0, 0};

    for (int round = 0; round < 100; round++) {
        int chosen_hide = choose_chest(num_chests, best_hider->probabilities);
        int chosen_guess = choose_chest(num_chests, best_seeker->probabilities);

        seeker_picks[chosen_guess]++;
        hider_picks[chosen_hide]++;

        int reward = payoff_matrix[chosen_guess][chosen_hide];
        seeker_score += reward;
        hider_score += -reward;

        printf("Round %3d: Hide Chest %d | Guess Chest %d | Reward (Seeker): %d\n",
               round + 1, chosen_hide + 1, chosen_guess + 1, reward);
    }

    printf("\n=== Final Score after 100 rounds ===\n");
    printf("Seeker: %d\n", seeker_score);
    printf("Hider : %d\n", hider_score);

    printf("\nChest Pick Counts:\n");
    printf("Seeker picks:\n");
    for (int i = 0; i < num_chests; i++) {
        printf("Chest %d: %d times\n", i + 1, seeker_picks[i]);
    }

    printf("Hider picks:\n");
    for (int i = 0; i < num_chests; i++) {
        printf("Chest %d: %d times\n", i + 1, hider_picks[i]);
    }

    for (int i = 0; i < num_chests; i++) free(payoff_matrix[i]);
    free(payoff_matrix);
}
