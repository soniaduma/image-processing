//DUMA SONIA NICOLA 315CA
#include "main.h"

void undo(all_states *states)
{
	if (states->history_size == 0) {
		printf("Nothing to undo\n");
		return;
	}
	state_t *state = current_state(states);
	// when a command is undoed
	// the command is added in redo array
	states->redos[states->redos_size] =
		malloc((strlen(state->cli_command) + 1) * sizeof(char));

	strcpy(states->redos[states->redos_size], state->cli_command);
	states->redos_size++;

	// and then the current state is deleted
	free_current_state(states);
}

void clear_redos(all_states *states)
{
	for (int i = 0; i < states->redos_size; i++) {
		free(states->redos[i]);
	}
}

void redo(all_states *states)
{
	if (states->redos_size == 0) {
		printf("Nothing to redo\n");
		return;
	}

	char copy[100];

	strcpy(copy, states->redos[states->redos_size - 1]);

	// the last command in the redo array is passed as argument in 'cli'
	// where a new state is added with the modifications of that command
	cli(states, copy, 0);

	if (states->redos_size != 0) {
		free(states->redos[states->redos_size - 1]);

		states->redos[states->redos_size - 1] = NULL;
		states->redos_size--;
	}
}
