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
int choose_chest(int num_chests, int *probabilities) {
    int sum = 0;
    for (int i = 0; i < num_chests; i++) sum += probabilities[i];
    if (sum == 0) return rand() % num_chests;

    int r = rand() % sum;
    int cum = 0;
    for (int i = 0; i < num_chests; i++) {
        cum += probabilities[i];
        if (r < cum) return i;
    }
    return num_chests - 1;
}

/*
 * Simulate a number of games with random chest placements and strategies.
 * This function initializes the game, runs the simulations, and prints results.
 */
void simulate_games() {
    srand(time(NULL));

    int seeker_score = 0;
    int hider_score = 0;

    for (int sim = 0; sim < SIMULATIONS; sim++) {
        int num_chests = MIN_CHESTS + rand() % (MAX_CHESTS - MIN_CHESTS + 1);

        PlaceType *places = malloc(num_chests * sizeof(PlaceType));
        for (int i = 0; i < num_chests; i++) {
            places[i] = random_place_type();
        }

        int **payoff_matrix = malloc(num_chests * sizeof(int *));
        for (int i = 0; i < num_chests; i++) {
            payoff_matrix[i] = malloc(num_chests * sizeof(int));
        }

        for (int guess = 0; guess < num_chests; guess++) {
            for (int hide = 0; hide < num_chests; hide++) {
                PlaceType place = places[hide];
                int idx = place_type_index(place);

                if (guess == hide) {
                    payoff_matrix[guess][hide] = seeker_rewards[idx];
                } else {
                    payoff_matrix[guess][hide] = penalties[idx];
                }
            }
        }

        printf("\n=== Round %d (Chests: %d) ===\nPlace Types (1=EASY, 2=NEUTRAL, 3=HARD):\n", sim + 1, num_chests);
        for (int i = 0; i < num_chests; i++) printf("%d ", places[i]);
        printf("\nPayoff Matrix (Seeker Rows, Hider Cols):\n");

        for (int i = 0; i < num_chests; i++) {
            for (int j = 0; j < num_chests; j++) {
                printf("%3d ", payoff_matrix[i][j]);
            }
            printf("\n");
        }

        struct hider hider = initialize_hider(num_chests, payoff_matrix);
        struct seeker seeker = initialize_seeker(num_chests, payoff_matrix);

        struct hider best_hider = find_hider_strategy(&hider, num_chests);
        struct seeker best_seeker = find_seeker_strategy(&seeker, num_chests);

        printf("Hider Strategy:\n");
        for (int i = 0; i < num_chests; i++) printf("Chest %d: %.4f\n", i + 1, best_hider.probabilities[i]);

        printf("Seeker Strategy:\n");
        for (int i = 0; i < num_chests; i++) printf("Chest %d: %.4f\n", i + 1, best_seeker.probabilities[i]);

        int chosen_hide = choose_chest(num_chests, best_hider.probabilities);
        int chosen_guess = choose_chest(num_chests, best_seeker.probabilities);

        printf("Chosen Hide: Chest %d\n", chosen_hide + 1);
        printf("Chosen Guess: Chest %d\n", chosen_guess + 1);

        int reward = payoff_matrix[chosen_guess][chosen_hide];
        printf("Reward (Seeker): %d\n", reward);

        seeker_score += reward;
        hider_score += -reward;  // Opponent payoff is negative of seeker's payoff

        printf("Current Score => Seeker: %d | Hider: %d\n", seeker_score, hider_score);

        for (int i = 0; i < num_chests; i++) free(payoff_matrix[i]);
        free(payoff_matrix);
        free(places);
        free(hider.probabilities);
        free(seeker.probabilities);
    }

    printf("\n=== Final Score ===\n");
    printf("Seeker: %d\n", seeker_score);
    printf("Hider : %d\n", hider_score);
}
