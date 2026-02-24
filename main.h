//DUMA SONIA NICOLA 315CA
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "memory.h"
#include "state.h"
#include <stdlib.h>
#define NMAX 100

int cli(all_states *states, char *sir, int delete_redos);
char *derive(all_states *states, char *args, int to_print);

void lsystem(all_states *states, char *sir);
void free_lsystem(state_t *state);

void load(all_states *states, char *image);
void save(all_states *states, char *image);
void free_image(state_t *state);

void turtle(all_states *states, char *args);

void type(all_states *states, char *args);
