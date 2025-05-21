#include "lp_lib.h"
#include "game_logic/ai_characters/hider.h"
#include <stdio.h>
#include <stdlib.h>

struct hider* initialize_hider(int num_chests, int **payoff_matrix) {
    struct hider* s = malloc(sizeof(struct hider));
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
            s->constraints[hide][guess + 1] = payoff_matrix[hide][guess] * -1;
        }
        s->constraints[hide][num_chests + 1] = -1.0; // coefficient of -v
    }

    return s;
}

void find_hider_strategy(struct hider *hider, int num_chests) {
    lprec *lp = make_lp(0, num_chests + 1); // variables: p1..pn + v
    if (lp == NULL) {
        printf("Failed to create LP model.\n");
        return *hider;
    }

    set_col_name(lp, num_chests + 1, "w");

    // Add constraints for each hider choice (columns):
    // sum_i p_i * payoff[i][j] - v >= 0
    for (int hide = 0; hide < num_chests; ++hide) {
        add_constraint(lp, hider->constraints[hide], GE, 0.0);
    }

    // Sum of probabilities = 1
    double sum_row[num_chests + 2]; // 1-based + w
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

    // Objective: minimize v
    double obj[num_chests + 2];
    obj[0] = 0.0;
    for (int i = 1; i <= num_chests; ++i) obj[i] = 0.0;
    obj[num_chests + 1] = 1.0; // coefficient for w
    set_obj_fn(lp, obj);
    set_maxim(lp);

    int ret = solve(lp);

    double vars[num_chests + 1];
    get_variables(lp, vars);

    for (int i = 0; i < num_chests; ++i) {
        hider->probabilities[i] = vars[i];
    }
    printf("W: %.4f\n", vars[num_chests]);
    hider->value = vars[num_chests];
    delete_lp(lp);
}
