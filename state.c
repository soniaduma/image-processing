//DUMA SONIA NICOLA 315CA
#include "main.h"

// returns the current state
state_t *current_state(all_states *states)
{
	if (states->history_size == 0)
		return NULL;
	return states->history[states->history_size - 1];
}

// here is created the new state and its components are initialized with NULL
void create_state(all_states *states, char *cli_command)
{
	states->history[states->history_size] = malloc(sizeof(state_t));
	state_t *current = states->history[states->history_size];

	current->lsys = NULL;
	current->img = NULL;
	current->font = NULL;
	current->cli_command = malloc((strlen(cli_command) + 1) * sizeof(char));

	strcpy(current->cli_command, cli_command);
	states->history_size++;
}

//debug todo sa l sterg
void print_l_system_current(all_states *states)
{
	state_t *state = current_state(states);
	l_system *current = state->lsys;
	int i;

	for (i = 0; i < current->n_rules; i++) {
		printf("%c %s\n", current->p_rules[i], current->q_rules[i]);
	}
}

// when a new state is created the previuos elements are copied
// this function copies the last lsystem readed
void deepcopy_previous_lsystem(all_states *states)
{
	if (states->history_size >= 2) {
		l_system *prev = states->history[states->history_size - 2]->lsys;

		if (!prev)
			return;

		state_t *state = current_state(states);

		state->lsys = malloc(sizeof(l_system));
		l_system *current = state->lsys;

		current->axiom = NULL;
		if (prev->axiom) {
			current->axiom = malloc(100 * sizeof(char));
			strcpy(current->axiom, prev->axiom);
		}

		current->n_rules = prev->n_rules;
		current->p_rules = NULL;
		current->q_rules = NULL;

		if (current->n_rules == 0)
			return;

		current->p_rules = malloc((current->n_rules + 1) * sizeof(char));
		current->q_rules = malloc((current->n_rules + 1) * sizeof(char *));

		for (int i = 0; i < prev->n_rules; i++) {
			current->p_rules[i] = prev->p_rules[i];
			current->q_rules[i] = malloc((strlen(prev->q_rules[i]) + 1) *
						      sizeof(char));
			strcpy(current->q_rules[i], prev->q_rules[i]);
		}
	}
}

// this function copies the last image readed
void deepcopy_previous_image(all_states *states)
{
	if (states->history_size >= 2) {
		if (!states->history[states->history_size - 2]->img)
			return;

		image *prev = states->history[states->history_size - 2]->img;
		state_t *state = current_state(states);

		state->img = malloc(sizeof(image));
		image *current = state->img;

		current->pixels = malloc(prev->height * sizeof(pixel *));

		current->width = prev->width;
		current->height = prev->height;

		for (int i = 0; i < current->height; i++) {
			current->pixels[i] = malloc(prev->width * sizeof(pixel));
			for (int j = 0; j < current->width; j++) {
				current->pixels[i][j].r = prev->pixels[i][j].r;
				current->pixels[i][j].g = prev->pixels[i][j].g;
				current->pixels[i][j].b = prev->pixels[i][j].b;
			}
		}
	}
}

// this function copies the last font readed
void deepcopy_previous_font(all_states *states)
{
	if (states->history_size < 2)
		return;

	state_t *prev_state = states->history[states->history_size - 2];

	if (!prev_state || !prev_state->font)
		return;

	font_bdf *prev = prev_state->font;
	state_t *state = current_state(states);

	if (!state)
		return;

	state->font = malloc(sizeof(font_bdf));
	font_bdf *current = state->font;

	current->font_name = malloc((strlen(prev->font_name) + 1) * sizeof(char));
	strcpy(current->font_name, prev->font_name);

	current->chars_size = prev->chars_size;
	current->chr = malloc((prev->chars_size + 1) * sizeof(character *));
	for (int c = 0; c < current->chars_size; c++)
		current->chr[c] = NULL;

	for (int c = 0; c < current->chars_size; c++) {
		current->chr[c] = malloc(sizeof(character));

		current->chr[c]->ascii_code = prev->chr[c]->ascii_code;
		current->chr[c]->x = prev->chr[c]->x;
		current->chr[c]->y = prev->chr[c]->y;
		current->chr[c]->width = prev->chr[c]->width;
		current->chr[c]->height = prev->chr[c]->height;
		current->chr[c]->next_x = prev->chr[c]->next_x;
		current->chr[c]->next_y = prev->chr[c]->next_y;

		current->chr[c]->pixels = NULL;
		if (prev->chr[c]->pixels && prev->chr[c]->height > 0 &&
		    prev->chr[c]->width > 0) {
			current->chr[c]->pixels = malloc(prev->chr[c]->height *
							 sizeof(int *));
			for (int i = 0; i < prev->chr[c]->height; i++) {
				current->chr[c]->pixels[i] =
					malloc(prev->chr[c]->width * sizeof(int));
				for (int j = 0; j < prev->chr[c]->width; j++) {
					current->chr[c]->pixels[i][j] =
						prev->chr[c]->pixels[i][j];
				}
			}
		}
	}
}

void free_current_state(all_states *states)
{
	state_t *current = current_state(states);

	if (!current)
		return;

	free_lsystem(current);
	free_image(current);
	free_font(current);

	if (current->cli_command) {
		free(current->cli_command);
		current->cli_command = NULL;
	}

	free(states->history[states->history_size - 1]);
	states->history[states->history_size - 1] = NULL;
	states->history_size--;
}
