#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PARTITIONS 4000
#define START_TIME 0
#define END_TIME 60
#define DELTA_TIME (double)(END_TIME - START_TIME) / PARTITIONS
#define OUTFILE "double-pendulum-pts.txt"

struct system_state {
  double phi;
  double psi;
  double z1;
  double z2;
};

const struct system_state INITIAL_CONDITIONS = {M_PI_4, 0, 0, 0};
const double l = 1;
const double m = 1;

size_t itter_ctr = 0;
struct system_state states[PARTITIONS];

int set_state(struct system_state *state, struct system_state *new_state);
// double diff_phi_func(struct system_state *state);
// double diff_psi_func(struct system_state *state);
double diff_z1_func(struct system_state *state);
double diff_z2_func(struct system_state *state);
void compute_itter(void);

int main(void) {
  fprintf(stderr, "STARTING COMPUTING WITH FOLLOWING CONDITIONS:\n");
  fprintf(stderr, "%12s\t%d s\n", "START TIME", START_TIME);
  fprintf(stderr, "%12s\t%d s\n", "END TIME", END_TIME);
  fprintf(stderr, "%12s\t%d\n", "PARTITIONS", PARTITIONS);
  fprintf(stderr, "%12s\t%lf s\n", "DELTA TIME", DELTA_TIME);
  fprintf(stderr, "%12s\t%lf\n", "PHI_0", INITIAL_CONDITIONS.phi);
  fprintf(stderr, "%12s\t%lf\n", "PSI_0", INITIAL_CONDITIONS.psi);
  fprintf(stderr, "%12s\t%lf\n", "Z1_0", INITIAL_CONDITIONS.z1);
  fprintf(stderr, "%12s\t%lf\n", "Z2_0", INITIAL_CONDITIONS.z2);

  fprintf(stderr, "SETTING INITIAL CONDITIONS.\n");
  set_state(&states[itter_ctr++], &INITIAL_CONDITIONS);

  while (itter_ctr < PARTITIONS) {
    compute_itter();
  }

  FILE *outfile = fopen(OUTFILE, "w");
  if (!outfile) {
    perror("main: failed to open file");
    return 1;
  }

  for (int i = 0; i < PARTITIONS; i++) {
    fprintf(outfile, "\t%lf\t%lf\t%lf\t%lf\n", states[i].phi, states[i].z1,
            states[i].psi, states[i].z2);
  }

  fclose(outfile);

  fprintf(stderr, "COMPUTING DONE\n");

  return 0;
}

int set_state(struct system_state *state, struct system_state *new_state) {
  if (!state || !new_state) {
    fprintf(stderr, "set_state: recieved NULL args");
    return -1;
  }

  state->phi = new_state->phi;
  state->psi = new_state->psi;
  state->z1 = new_state->z1;
  state->z2 = new_state->z2;

  return 0;
}

double main_det(double phi, double psi) {
  return l * l * (2 - cos(phi - psi) * cos(phi - psi));
}

double diff_z1_func(struct system_state *state) {
  return (-l * l * sin(state->phi - state->psi) * state->z2 * state->z2 -
          2 * l * sin(state->phi) -
          0.5 * l * l * sin(2 * (state->phi - state->psi)) * state->z1 *
              state->z1 +
          l * cos(state->psi) * cos(state->phi - state->psi)) /
         main_det(state->phi, state->psi);
}

double diff_z2_func(struct system_state *state) {
  return (2 * l * l * sin(state->phi - state->psi) * state->z1 * state->z1 -
          2 * l * sin(state->psi) -
          0.5 * l * l * sin(2 * (state->phi - state->psi)) * state->z2 *
              state->z2 -
          2 * l * cos(state->phi) * cos(state->phi - state->psi)) /
         main_det(state->phi, state->psi);
}

void compute_itter(void) {
  states[itter_ctr].phi =
      states[itter_ctr - 1].phi + DELTA_TIME * states[itter_ctr - 1].z1;
  states[itter_ctr].psi =
      states[itter_ctr - 1].psi + DELTA_TIME * states[itter_ctr - 1].z2;
  states[itter_ctr].z1 = states[itter_ctr - 1].z1 +
                         DELTA_TIME * diff_z1_func(&states[itter_ctr - 1]);
  states[itter_ctr].z2 = states[itter_ctr - 1].z2 +
                         DELTA_TIME * diff_z2_func(&states[itter_ctr - 1]);
  itter_ctr++;
}
