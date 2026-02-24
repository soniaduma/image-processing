#include "main.h"

// here the image is read from the PPM file
void load(all_states *states, char *image_file)
{
	FILE *fptr = fopen(image_file, "rb");

	if (fptr) {
		printf("Loaded %s ", image_file);
	} else {
		printf("Failed to load %s\n", image_file);
		free_current_state(states);
		return;
	}

	state_t *state = current_state(states);

	state->img = malloc(sizeof(image));
	image *current = state->img;

	char buff[100];

	fgets(buff, 100, fptr); //p6  is ignored
	fgets(buff, 100, fptr); // for width an height

	char *width = strtok(buff, " ");

	current->width = atoi(width);
	char *height = strtok(NULL, "\n ");

	current->height = atoi(height);

	printf("(PPM image %dx%d)\n", current->width, current->height);

	fgets(buff, 100, fptr); // 255 is ignored
	current->pixels = malloc((current->height) * sizeof(pixel *));

    //the image is stored in reverse because we want
    //the point (0,0) to be like in the xoy axis
	for (int i = current->height - 1; i >= 0; i--) {
		current->pixels[i] = malloc(current->width * sizeof(pixel));
		for (int j = 0; j < current->width; j++) {
			char buffer[3];

			fread(buffer, 1, 3, fptr);
			pixel *pixel_rgb = &current->pixels[i][j];

			pixel_rgb->r = buffer[0];
			pixel_rgb->g = buffer[1];
			pixel_rgb->b = buffer[2];
		}
	}
	fclose(fptr);
}

void free_image(state_t *state)
{
	image *current = state->img;

	if (!current)
		return;

	int n = current->height;

	for (int i = 0; i < n; i++) {
		free(current->pixels[i]);
	}

	free(current->pixels);

	free(state->img);
	state->img = NULL;
}

void save(all_states *states, char *image_file)
{
	state_t *state = current_state(states);

	if (!state->img) {
		printf("No image loaded\n");
		return;
	}
	image *current = state->img;

	FILE *fptr = fopen(image_file, "wb");

	if (!fptr)
		return;

	fprintf(fptr, "P6\n");
	fprintf(fptr, "%d %d\n", current->width, current->height);
	fprintf(fptr, "255\n");

	for (int i = current->height - 1; i >= 0; i--) {
		for (int j = 0; j < current->width; j++) {
			pixel *pixel = &current->pixels[i][j];
			char buffer[3];

			buffer[0] = pixel->r;
			buffer[1] = pixel->g;
			buffer[2] = pixel->b;
			fwrite(buffer, 1, 3, fptr);
		}
	}

	fclose(fptr);
	printf("Saved %s\n", image_file);
}
