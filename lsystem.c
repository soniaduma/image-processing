//DUMA SONIA NICOLA 315CA
#include "main.h"

void free_lsystem(state_t *state)
{
	if (!state)
		return;

	l_system *current = state->lsys;

	if (!current)
		return;

	for (int i = 0; i < current->n_rules; i++) {
		if (current->q_rules[i])
			free(current->q_rules[i]);
	}

	if (current->p_rules)
		free(current->p_rules);

	if (current->q_rules)
		free(current->q_rules);

	if (current->axiom)
		free(current->axiom);

	if (state->lsys)
		free(state->lsys);

	state->lsys = NULL;
}

void lsystem(all_states *states, char *sir)
{
	state_t *state = current_state(states);
	FILE *fptr = fopen(sir, "r");

	if (fptr) {
		printf("Loaded %s ", sir);
	} else {
		printf("Failed to load %s\n", sir);
		return;
	}

	state->lsys = malloc(sizeof(l_system));
	l_system *current = state->lsys;

	current->axiom = malloc(100 * sizeof(char));

	if (fgets(current->axiom, 100, fptr))
		current->axiom[strcspn(current->axiom, "\n")] = '\0';

	char number[100];

	if (fgets(number, 100, fptr))
		number[strcspn(number, "\n")] = '\0';

	current->n_rules = atoi(number);

	printf("(L-system with %d rules)\n", current->n_rules);

	current->p_rules = NULL;
	current->q_rules = NULL;

	if (current->n_rules == 0) {
		if (fclose(fptr) == EOF)
			printf("Error closing file");
		return;
	}

	current->p_rules = malloc((current->n_rules + 1) * sizeof(char));
	current->q_rules = malloc((current->n_rules + 1) * sizeof(char *));

	for (int i = 0; i < current->n_rules; i++) {
		char string[100];

		if (fgets(string, 100, fptr))
			string[strcspn(string, "\n")] = '\0';

		current->p_rules[i] = string[0];

		int len = strlen(string) - 2;

		current->q_rules[i] = malloc(len + 1);

		int pos = 0;
		int k = 2;

		while (string[k] != '\0') {
			current->q_rules[i][pos++] = string[k];
			k++;
		}
		current->q_rules[i][pos] = '\0';
	}

	if (fclose(fptr) == EOF)
		printf("Error closing file");
}
