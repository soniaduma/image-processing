//DUMA SONIA NICOLA 315CA
#include "main.h"

char *derive(all_states *states, char *args, int to_print)
{
	state_t *state = current_state(states);
	if (!state || !state->lsys) {
		printf("No L-system loaded\n");
		return NULL;
	}

	l_system * current = state->lsys;
	char *res = strdup(current->axiom);
	int derivation_nr = atoi(args);

	// at each iteration, the number of elements that the string
	// which will be added is calculated, then a new string
	// is created (next_res) and is allocated mewmory for both
	// the old string and concatenated one
	for (int i = 0; i < derivation_nr; i++) {
		int n = strlen(res);
		int new_memory = 0;

		// here  is calculated the length
		for (int j = 0; j < n; j++) {
			char c = res[j];
			int index = -1;
			for (int k = 0; k < current->n_rules; k++) {
				if (current->p_rules[k] == c) {
					index = k;
					break;
				}
			}
			new_memory += (index != -1) ?
				      strlen(current->q_rules[index]) : 1;
		}

		char *next_res = malloc((new_memory + 1) * sizeof(char));

		if (!next_res) {
			free(res);
			return NULL;
		}

		// here the string from the iteration D(n) is created
		int pos = 0;
		for (int j = 0; j < n; j++) {
			char c = res[j];
			int index = -1;

			for (int k = 0; k < current->n_rules; k++) {
				if (current->p_rules[k] == c) {
					index = k;
					break;
				}
			}

			if (index != -1) {
				int q_pos = 0;
				// is added the rule at the end of the strig
				// resulting string (D(n-1))
				while (current->q_rules[index][q_pos] != '\0') {
					next_res[pos++] =
						current->q_rules[index][q_pos++];
				}
			} else {
				next_res[pos++] = c;
			}
		}
		//"res" is now the strng at the derivation D(n)
		next_res[pos] = '\0';
		free(res);
		res = next_res;
	}

	if (to_print) {
		printf("%s\n", res);
		free(res);
		return NULL;
	}
	return res;
}
