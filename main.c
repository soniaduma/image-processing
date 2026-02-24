//DUMA SONIA NICOLA 315CA
#include "main.h"

#define MAX_STATES 10000
#define MAX_LINE 4100
int main(void)
{
	all_states *states = malloc(sizeof(all_states));

	states->history_size = 0;
	states->redos_size = 0;
	states->history = malloc(sizeof(state_t *) * MAX_STATES);
	states->redos = malloc(10000 * sizeof(char *));

	char s[MAX_LINE];

	// for each line read from the CLI is callede the 'cli' command
	// which decides what command to execute next
	while (fgets(s, MAX_LINE, stdin)) {
		if (!cli(states, s, 1))
			break;
	}

	while (current_state(states)) {
		free_current_state(states);
	}

	free(states->history);
	clear_redos(states);
	free(states->redos);
	free(states);

	return 0;
}
