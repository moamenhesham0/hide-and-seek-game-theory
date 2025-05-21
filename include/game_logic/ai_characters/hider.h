#ifndef HIDER_H
#define HIDER_H

struct hider
{
    double *probabilities; // Probabilities of hiding in each cell
    double **constraints;  // Constraints for each cell
    double value; // Value of the strategy
};


struct hider* initialize_hider(int num_chests, int **payoff_matrix);
void find_hider_strategy(struct hider *hider, int num_chests);

#endif // HIDER_H