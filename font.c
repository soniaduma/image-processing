// DUMA SONIA NICOLA 315CA
#include "main.h"

#define LINE_NMAX 17000

int hex_to_decimal(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return 10 + c - 'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c - 'A';
	return -1;
}

// because the lines from BITMAP are written in hexadecimal
// and every digit represents 4 bits in binary (ex :A->1010)
// we convert the character into a decimal value and then
// add the values (0 or 1) in the matrix which corresponds to the letter
// if is 1 there will be color and 0 otherwise
void complete_line(int *pixels_line, char *hex_line, int width)
{
	int bit = 0;
	int i = 0;

	while (hex_line[i] && hex_line[i] != '\n' && bit < width) {
		int v = hex_to_decimal(hex_line[i]);
		int k = 3;

		while (k >= 0 && bit < width) {
			pixels_line[bit++] = (v >> k) & 1;
			k--;
		}
		i++;
	}
}

void free_font(state_t *state)
{
	if (!state || !state->font)
		return;

	font_bdf *current = state->font;

	for (int c = 0; c < current->chars_size; c++) {
		character *ch = current->chr[c];

		if (!ch)
			continue;

		if (ch->pixels) {
			for (int i = 0; i < ch->height; i++)
				free(ch->pixels[i]);
			free(ch->pixels);
			ch->pixels = NULL;
		}
		free(current->chr[c]);
	}

	if (current->font_name) {
		free(current->font_name);
		current->font_name = NULL;
	}

	if (current->chr)
		free(current->chr);

	free(current);
	state->font = NULL;
}

//in this function, each line from the BDG file
//received as an argument is read
//the data from the font is stored in the current state
void font(all_states *states, char *file)
{
	FILE *fptr = fopen(file, "r");

	if (fptr) {
		printf("Loaded %s ", file);
	} else {
		printf("Failed to load %s\n", file);
		return;
	}

	state_t *state = current_state(states);

	state->font = malloc(sizeof(font_bdf));
	font_bdf *current = state->font;
	char *line = malloc(LINE_NMAX * sizeof(char));

	while (fgets(line, LINE_NMAX, fptr)) {
		if (strncmp(line, "FONT ", 5) == 0)
			break;
	}

	char *name = strtok(line, " ");

	name = strtok(NULL, "\n");
	current->font_name = malloc((strlen(name) + 1) * sizeof(char));
	current->chr = NULL;
	strcpy(current->font_name, name);

	printf("(bitmap font %s)\n", name);

	int chr_index = -1;

	while (fgets(line, LINE_NMAX, fptr)) {
		if (strncmp(line, "CHARS ", 6) == 0) {
			char *s = strtok(line, " ");

			s = strtok(NULL, "\n ");
			current->chars_size = atoi(s);
			current->chr = malloc((current->chars_size + 1) *
					      sizeof(character *));
			for (int i = 0; i < current->chars_size; i++)
				current->chr[i] = NULL;
		} else if (strncmp(line, "BITMAP", 6) == 0) {
			character *chr = current->chr[chr_index];

			chr->pixels = malloc(chr->height * sizeof(int *));
			for (int i = 0; i < chr->height; i++)
				chr->pixels[i] = malloc(chr->width *
							sizeof(int));
			for (int i = 0; i < chr->height; i++) {
				complete_line(chr->pixels[i],
							  fgets(line, LINE_NMAX, fptr),
							  chr->width);
			}
		} else if (strncmp(line, "ENCODING ", 9) == 0) {
			char *s = strtok(line, " ");

			s = strtok(NULL, "\n ");
			current->chr[++chr_index] = malloc(sizeof(character));
			current->chr[chr_index]->pixels = NULL;
			current->chr[chr_index]->ascii_code = atoi(s);
		} else if (strncmp(line, "DWIDTH ", 7) == 0) {
			strtok(line, " ");
			current->chr[chr_index]->x = atoi(strtok(NULL, "\n "));
			current->chr[chr_index]->y = atoi(strtok(NULL, "\n "));
		} else if (strncmp(line, "BBX ", 4) == 0) {
			strtok(line, " ");
			current->chr[chr_index]->width =
				atoi(strtok(NULL, "\n "));
			current->chr[chr_index]->height =
				atoi(strtok(NULL, "\n "));
			current->chr[chr_index]->next_x =
				atoi(strtok(NULL, "\n "));
			current->chr[chr_index]->next_y =
				atoi(strtok(NULL, "\n "));
		}
	}
	free(line);
	fclose(fptr);
}
