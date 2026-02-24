//DUMA SONIA NICOLA 315CA
#include "main.h"
void turtle_case(int delete_redos, all_states *states,
				 char *original_sir, char *rest)
{
	if (delete_redos) {
		state_t *st = current_state(states);

		if (states->history_size && st->lsys &&
			st->img && states->redos_size) {
			clear_redos(states);
			states->redos_size = 0;
		}
	}
	create_state(states, original_sir);
	deepcopy_previous_lsystem(states);
	deepcopy_previous_image(states);
	deepcopy_previous_font(states);
	turtle(states, rest);
}

// in this function we check which arguments we receive from the CLI
// and call the function for the given command
// parameter delete_redos is for the logic from undo and redo,
// if it is 0 then it is called from "redo" and the redos queue will
// not be deleted, otherwise it is called from "main" and is
// deleted if we have an undoable command that was executed correctly
//every time an undoable command is called,
//a new state is created, where we copy the past state
//and modify what is new, this helps when undo is given
int cli(all_states *states, char *sir, int delete_redos)
{
	char *original_sir = malloc(sizeof(char) * strlen(sir) + 1);
	if (!original_sir)
		return 0;
	strcpy(original_sir, sir);
	char *cmd = strtok(sir, " \r\n");
	if (!cmd) {
		printf("sir = %s", original_sir);
		free(original_sir);
		return 0;
	}
	if (strcmp(cmd, "UNDO") == 0) {
		undo(states);
	} else if (strcmp(cmd, "REDO") == 0) {
		redo(states);
	} else if (strcmp(cmd, "EXIT") == 0) {
		// exit
	} else if (strcmp(cmd, "BITCHECK") == 0) {
		bitcheck(states);
	} else {
		char *rest = strtok(NULL, "\n");
		if (strcmp(cmd, "DERIVE") == 0) {
			derive(states, rest, 1);
		} else if (strcmp(cmd, "LSYSTEM") == 0) {
			create_state(states, original_sir);
			deepcopy_previous_image(states);
			deepcopy_previous_font(states);
			lsystem(states, rest);
			if (delete_redos && states->redos_size) {
				clear_redos(states);
				states->redos_size = 0;
			}
		} else if (strcmp(cmd, "LOAD") == 0) {
			create_state(states, original_sir);
			deepcopy_previous_lsystem(states);
			deepcopy_previous_font(states);
			load(states, rest);
			if (delete_redos && states->redos_size) {
				clear_redos(states);
				states->redos_size = 0;
			}
		} else if (strcmp(cmd, "SAVE") == 0) {
			save(states, rest);
		} else if (strcmp(cmd, "TURTLE") == 0) {
			turtle_case(delete_redos, states, original_sir, rest);
		} else if (strcmp(cmd, "FONT") == 0) {
			create_state(states, original_sir);
			deepcopy_previous_lsystem(states);
			deepcopy_previous_image(states);
			font(states, rest);
			if (delete_redos && states->redos_size) {
				clear_redos(states);
				states->redos_size = 0;
			}
		} else if (strcmp(cmd, "TYPE") == 0) {
			if (delete_redos) {
				state_t *st = current_state(states);

				if (states->history_size && st->font &&
				    st->img && states->redos_size) {
					clear_redos(states);
					states->redos_size = 0;
				}
			}
			create_state(states, original_sir);
			deepcopy_previous_lsystem(states);
			deepcopy_previous_image(states);
			deepcopy_previous_font(states);
			type(states, rest);
		} else {
			printf("comanda invalida\n");
		}
	}
	free(original_sir);
	return 1;
}
