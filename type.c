// DUMA SONIA NICOLA 315CA
#include "main.h"
#define LINE_NMAX 17000

// this fucntion draws the character
void draw_chr(image *img, font_bdf *font, char c, int *x, int *y, pixel color)
{
	character *chr = NULL;

	// here the character is located in the array of the font character
	for (int i = 0; i < font->chars_size; i++) {
		if (font->chr[i] && font->chr[i]->ascii_code == c) {
			chr = font->chr[i];
			break;
		}
	}

	if (!chr)
		return;

	// the positions to start the drawing are offseted because
	//some letter like g extend below the baseline
	int start_x = *x + chr->next_x;
	int start_y = *y + chr->next_y;

	for (int i = 0; i < chr->height; i++) {
		for (int j = 0; j < chr->width; j++) {
			int p_y = start_y + (chr->height - 1 - i);
			int p_x = start_x + j;

			if (chr->pixels[i][j] == 1 &&
			    (p_x >= 0 && p_x < img->width &&
			     p_y >= 0 && p_y < img->height)) {
				img->pixels[p_y][p_x].r = color.r;
				img->pixels[p_y][p_x].g = color.g;
				img->pixels[p_y][p_x].b = color.b;
			}
		}
	}

	//the cursor is advanced for the next letter to be drawn
	*x += chr->x;
	*y += chr->y;
}

// every character of the text is drawn
void draw(image *img, font_bdf *font, pixel color, char *string, int x, int y)
{
	int i = 0;

	while (string[i] != '\0') {
		draw_chr(img, font, string[i], &x, &y, color);
		i++;
	}
}

void type(all_states *states, char *args)
{
	state_t *state = current_state(states);

	if (!state->img) {
		printf("No image loaded\n");
		return;
	}

	if (!state->font) {
		printf("No font loaded\n");
		return;
	}

	int i = 0;

	while (args[i] != '"')
		i++;
	i++;

	char string[4096];
	int k = 0;

	while (args[i] != '"')
		string[k++] = args[i++];
	string[k] = '\0';

	i += 2;
	char *rest = args + i;
	int start_x = atoi(strtok(rest, "\n "));
	int start_y = atoi(strtok(NULL, "\n "));

	pixel color;

	color.r = atoi(strtok(NULL, " "));
	color.g = atoi(strtok(NULL, " "));
	color.b = atoi(strtok(NULL, "\n "));

	draw(state->img, state->font, color, string, start_x, start_y);
	printf("Text written\n");
}
