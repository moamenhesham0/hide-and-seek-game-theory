#ifndef SEEKER_H
#define SEEKER_H

struct seeker
{
    double *probabilities; // Probabilities of hiding in each cell
    double **constraints;  // Constraints for each cell
};

struct seeker initialize_seeker(int num_chests, int **payoff_matrix);
struct seeker find_seeker_strategy(struct seeker *seeker, int num_chests);

#endif // HIDER_H