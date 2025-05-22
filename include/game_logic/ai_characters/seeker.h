#ifndef SEEKER_H
#define SEEKER_H

struct seeker
{
    double *probabilities; // Probabilities of hiding in each cell
    double **constraints;  // Constraints for each cell
    double value; // Value of the strategy
};

struct seeker* initialize_seeker(int num_chests, double **payoff_matrix);
void find_seeker_strategy(struct seeker *seeker, int num_chests);

#endif // HIDER_H