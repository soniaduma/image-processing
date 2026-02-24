//DUMA SONIA NICOLA 315CA
#include "main.h"
#include <math.h>
#include "turtle.h"

#define PI 3.14159265358979323846f

void add_state(all_states_turtle *s, int orientation, float x, float y)
{
	turtle_state *current = &s->turtle_state[s->size];

	current->x = x;
	current->y = y;
	current->orientation = orientation;
	s->size++;
}

void remove_state(all_states_turtle *s)
{
	if (s->size > 0)
		s->size--;
}

void get_state(all_states_turtle *s, float *x, float *y, int *orientation)
{
	turtle_state *current;

	if (s->size == 0)
		return;

	current = &s->turtle_state[s->size - 1];
	*x = current->x;
	*y = current->y;
	*orientation = current->orientation;
}

//here the new x and y are calculated using trigonometry
void get_new_position(float x, float y, float orientation, float distance,
					  float *new_x, float *new_y)
{
	//degrees are converted to radians because C math functions need radians
	float rad = orientation * PI / 180.0f;

	*new_x = x + distance * cosf(rad);
	*new_y = y + distance * sinf(rad);
}

void draw_pixel(image *img, int x, int y, pixel color)
{
	if (!img)
		return;
	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return;
	img->pixels[y][x] = color;
}

// Bresenham's algorithm to draw the line
void draw_line(image *img, int x0, int y0, int x1, int y1, pixel color)
{
	int dx = abs(x1 - x0);
	int sx = (x0 < x1) ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx + dy;

	while (1) {
		draw_pixel(img, x0, y0, color);

		if (x0 == x1 && y0 == y1)
			break;

		int e2 = 2 * err;

		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		}

		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void decision(image *img, all_states_turtle *s, char c, float *x, float *y,
			  int distance, int *orientation, float angle, pixel color)
{
	if (c == 'F') {
		float new_x;
		float new_y;

		get_new_position(*x, *y, (float)*orientation, (float)distance,
						 &new_x, &new_y);
		int x_pos = (int)lroundf(*x);
		int y_pos = (int)lroundf(*y);
		int x_next = (int)lroundf(new_x);
		int y_next = (int)lroundf(new_y);

		draw_line(img, x_pos, y_pos, x_next, y_next, color);
		*x = new_x;
		*y = new_y;
	} else if (c == '+') {
		*orientation += (int)angle;
	} else if (c == '-') {
		*orientation -= (int)angle;
	} else if (c == '[') {
		add_state(s, *orientation, *x, *y);
	} else if (c == ']') {
		if (s->size > 0) {
			get_state(s, x, y, orientation);
			remove_state(s);
		}
	}
}

// the arguments are parsed from the initial command
void read_turtle(char *args, float *x, float *y, int *distance,
				 int *orientation, float *angle, char *n, int *r,
				 int *g, int *b)
{
	char *p;

	*x = (float)atof(strtok(args, "\n "));
	*y = (float)atof(strtok(NULL, "\n "));
	*distance = atoi(strtok(NULL, "\n "));
	*orientation = atoi(strtok(NULL, "\n "));
	*angle = (float)atof(strtok(NULL, "\n "));
	p = strtok(NULL, "\n ");
	if (p)
		strcpy(n, p);

	*r = atoi(strtok(NULL, "\n "));
	*g = atoi(strtok(NULL, "\n "));
	*b = atoi(strtok(NULL, "\n "));
}

void print_turtle(float *x, float *y, int *distance, int *orientation,
				  float *angle, int *n, int *r, int *g, int *b)
{
	printf("%f ", *x);
	printf("%f ", *y);
	printf("%d ", *distance);
	printf("%d ", *orientation);
	printf("%f ", *angle);
	printf("%d ", *n);
	printf("%d ", *r);
	printf("%d ", *g);
	printf("%d ", *b);
}

void turtle(all_states *states, char *args)
{
	state_t *state = current_state(states);
	float x, y, angle;
	int distance, orientation, r, g, b;
	char n[20];
	char *s;
	int number_derive;
	all_states_turtle *S;
	pixel color;

	if (!state->lsys) {
		printf("No L-system loaded\n");
		return;
	}

	if (!state->img) {
		printf("No image loaded\n");
		return;
	}

	// the arguments are parsed
	read_turtle(args, &x, &y, &distance, &orientation, &angle, n,
				&r, &g, &b);

	// s is the derivated lsystem
	s = derive(states, n, 0);

	if (!s)
		return;

	number_derive = strlen(s);
	S = malloc(sizeof(all_states_turtle));
	S->size = 0;
	S->turtle_state = malloc(number_derive * sizeof(turtle_state));

	color.r = r;
	color.g = g;
	color.b = b;

	// after the arguments are parsed
	// for each character of the derived lsystem
	// is called the function 'decision' where is decided
	// the position of the turtle
	// and if something will be drawn
	for (int i = 0; i < number_derive; i++) {
		decision(state->img, S, s[i], &x, &y, distance,
				 &orientation, angle, color);
	}

	free(s);
	if (S->turtle_state)
		free(S->turtle_state);
	free(S);
	printf("Drawing done\n");
}
