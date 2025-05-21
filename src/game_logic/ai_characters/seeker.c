#include "lp_lib.h"
#include "game_logic/ai_characters/seeker.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Seeker AI character implementation.
 * This file contains the logic for the seeker character in the game.
 * The seeker uses linear programming to determine the optimal strategy.
 */
struct seeker* initialize_seeker(int num_chests, int **payoff_matrix) {
    struct seeker* s = malloc(sizeof(struct seeker));
    s->probabilities = (double *)malloc(num_chests * sizeof(double));
    s->constraints = (double **)malloc(num_chests * sizeof(double *));
    for (int i = 0; i < num_chests; ++i) {
        s->constraints[i] = (double *)malloc((num_chests + 1 + 1) * sizeof(double));
        // num_chests variables + 1 for 'v' + 1 for 1-based indexing
        s->probabilities[i] = 0.0;
    }

    // Prepare constraints matrix for LP:
    // Each constraint corresponds to one hider choice (column)
    // Row: p_i coefficients + coefficient for v = -1 (to do sum p_i * payoff[i][j] - v >= 0)
    for (int hide = 0; hide < num_chests; ++hide) {
        s->constraints[hide][0] = 0.0; // unused 0 index (lp_solve 1-based indexing)
        for (int guess = 0; guess < num_chests; ++guess) {
            s->constraints[hide][guess + 1] = payoff_matrix[guess][hide] * -1;
        }
        s->constraints[hide][num_chests + 1] = -1.0; // coefficient of -v
    }


    return s;
}

/*
 * Find the optimal strategy for the seeker using linear programming.
 * This function sets up the LP model, solves it, and updates the seeker's probabilities.
 */
void find_seeker_strategy(struct seeker *seeker, int num_chests) {
    lprec *lp = make_lp(0, num_chests + 1); // variables: p1..pn + v
    if (lp == NULL) {
        printf("Failed to create LP model.\n");
        return *seeker;
    }

    set_col_name(lp, num_chests + 1, "v");

    // Add constraints for each hider choice (columns):
    // sum_i p_i * payoff[i][j] - v >= 0
    for (int hide = 0; hide < num_chests; ++hide) {
        add_constraint(lp, seeker->constraints[hide], LE, 0.0);
    }

    // Sum of probabilities = 1
    double sum_row[num_chests + 2]; // 1-based + v
    sum_row[0] = 0.0;
    for (int i = 0; i < num_chests; ++i) {
        sum_row[i + 1] = 1.0;
    }
    sum_row[num_chests + 1] = 0.0; // v coefficient in this constraint
    add_constraint(lp, sum_row, EQ, 1.0);

    // Set lower bounds: p_i >= 0
    for (int i = 1; i <= num_chests; ++i) {
        set_lowbo(lp, i, 0.0);
    }
    // v can be unbounded (default)

    // Objective: maximize v
    double obj[num_chests + 2];
    obj[0] = 0.0;
    for (int i = 1; i <= num_chests; ++i) obj[i] = 0.0;
    obj[num_chests + 1] = 1.0; // coefficient for v
    set_obj_fn(lp, obj);
    set_minim(lp);

    int ret = solve(lp);
    if (ret != OPTIMAL) {
        printf("No optimal solution found. Status code: %.4f\n", ret);
        delete_lp(lp);
        return *seeker;
    }

    double vars[num_chests + 1];
    get_variables(lp, vars);

    for (int i = 0; i < num_chests; ++i) {
        seeker->probabilities[i] = vars[i];
    }
    printf("V: %.4f\n", vars[num_chests]);
    seeker->value = vars[num_chests];
    delete_lp(lp);
}
