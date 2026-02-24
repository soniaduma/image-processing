//DUMA SONIA NICOLA 315CA
#include "main.h"

//here we check for each received byte if it has the
//structure of the form 0010 or 1101 and display
//the pixel and the color with which it can be confused
void bit(int r, int g, int b, int x, int y, unsigned char rgb, char c)
{
	for (int i = 7; i >= 3; i--) {
		if (((((rgb >> i) & 1) == 1 && ((rgb >> (i - 1)) & 1) == 1 &&
		      ((rgb >> (i - 2)) & 1) == 0 && ((rgb >> (i - 3)) & 1) == 1) ||
		     (((rgb >> i) & 1) == 0 && ((rgb >> (i - 1)) & 1) == 0 &&
		      ((rgb >> (i - 2)) & 1) == 1 && ((rgb >> (i - 3)) & 1) == 0))) {
			unsigned char modified_rgb = rgb;

			modified_rgb ^= (1 << (i - 2));

			if (c == 'r') {
				printf("Warning: pixel at (%d, %d) ", x, y);
				printf("may be read as ");
				printf("(%d, %d, %d)\n", (int)modified_rgb, g, b);
			} else if (c == 'g') {
				printf("Warning: pixel at (%d, %d) ", x, y);
				printf("may be read as ");
				printf("(%d, %d, %d)\n", r, (int)modified_rgb, b);
			} else if (c == 'b') {
				printf("Warning: pixel at (%d, %d) ", x, y);
				printf("may be read as ");
				printf("(%d, %d, %d)\n", r, g, (int)modified_rgb);
			}
		}
	}
}

void message_to_print(char c, int x, int y, int mod_rgb, int r, int g, int b)
{
		if (c == 'r') {
			printf("Warning: pixel at (%d, %d) may be read as (%d, %d, %d)\n",
			       x, y, mod_rgb, g, b);
		} else if (c == 'g') {
			printf("Warning: pixel at (%d, %d) may be read as (%d, %d, %d)\n",
			       x, y, r, mod_rgb, b);
		} else {
			printf("Warning: pixel at (%d, %d) may be read as (%d, %d, %d)\n",
			       x, y, r, g, mod_rgb);
		}
}

//here we check for each two received bytes
//if at their intersection we have the structure 1101 or 0010
//there can be 3 cases
void bit_two(int r, int g, int b,
			 int next_r, int next_g, int next_b,
			 int x, int y, int next_x, int next_y,
			 unsigned char rgb, unsigned char next_rgb, char c, char next_c)
{
	int x2 = x, y2 = y;

	if (c == 'b') {
		x2 = next_x;
		y2 = next_y;
	}

	//(1) 3 bits from the first byte + 1  bit from the other
	// rgb[2],rgb[1],rgb[0], next[7]
	int byt_0 = (rgb >> 2) & 1;
	int byt_1 = (rgb >> 1) & 1;
	int byt_2 = (rgb >> 0) & 1;
	int byt_3 = (next_rgb >> 7) & 1;

	if (byt_0 == 1 && byt_1 == 1 && byt_2 == 0 && byt_3 == 1) {
		unsigned char mod_rgb = rgb;
		// is created a copy to rgb
		// to change the byt 0 from 1101 and print the color
		mod_rgb |= (1 << 0);
		message_to_print(c, x, y, (int)mod_rgb, r, g, b);
	} else if (byt_0 == 0 && byt_1 == 0 && byt_2 == 1 && byt_3 == 0) {
		unsigned char mod_rgb = rgb;
		// is created a copy to rgb
		// to change the bit 1 from 0010 and print the color
		mod_rgb &= ~(1 << 0);
		message_to_print(c, x, y, (int)mod_rgb, r, g, b);
	}
	//(2) 2 bits from the first byte + 2 bits from the other
	// rgb[1], rgb[0], next[7], next[6]
	int b0 = (rgb >> 1) & 1;
	int b1 = (rgb >> 0) & 1;
	int b2 = (next_rgb >> 7) & 1;
	int b3 = (next_rgb >> 6) & 1;

	if (b0 == 1 && b1 == 1 && b2 == 0 && b3 == 1) {
		unsigned char mod_n_rgb = next_rgb;

		mod_n_rgb |= (1 << 7);
		message_to_print(next_c, x2, y2, (int)mod_n_rgb,
						 next_r, next_g, next_b);
	} else if (b0 == 0 && b1 == 0 && b2 == 1 && b3 == 0) {
		unsigned char mod_n_rgb = next_rgb;

		mod_n_rgb &= ~(1 << 7);
		message_to_print(next_c, x2, y2, (int)mod_n_rgb,
						 next_r, next_g, next_b);
	}
	//(3) 1 bit from the first byte + 3 bits from the other
	// rgb[0], next[7], next[6], next[5]
	int bt0 = (rgb >> 0) & 1;
	int bt1 = (next_rgb >> 7) & 1;
	int bt2 = (next_rgb >> 6) & 1;
	int bt3 = (next_rgb >> 5) & 1;

	if (bt0 == 1 && bt1 == 1 && bt2 == 0 && bt3 == 1) {
		unsigned char mod_n_rgb = next_rgb;

		mod_n_rgb |= (1 << 6);
		message_to_print(next_c, x2, y2, (int)mod_n_rgb,
						 next_r, next_g, next_b);
	} else if (bt0 == 0 && bt1 == 0 && bt2 == 1 && bt3 == 0) {
		unsigned char mod_n_rgb = next_rgb;

		mod_n_rgb &= ~(1 << 6);
		message_to_print(next_c, x2, y2, (int)mod_n_rgb,
						 next_r, next_g, next_b);
	}
}

//here we take each pixel from the image
//to check the existence of the given sequences
void bitcheck(all_states *states)
{
	state_t *state = current_state(states);
	image *current = state->img;

	if (!current) {
		printf("No image loaded\n");
		return;
	}

	for (int y = current->height - 1; y >= 0; y--) {
		for (int x = 0; x < current->width; x++) {
			int r = current->pixels[y][x].r;
			int g = current->pixels[y][x].g;
			int b = current->pixels[y][x].b;

			//the byte for "R"
			bit(r, g, b, x, y, (unsigned char)r, 'r');

			//the bites from the intersection of "R" and "G"
			bit_two(r, g, b, r, g, b, x, y, x, y,
					(unsigned char)r, (unsigned char)g, 'r', 'g');

			//the byte for "G"
			bit(r, g, b, x, y, (unsigned char)g, 'g');

			//the bites from the intersection of "G" and "B"
			bit_two(r, g, b, r, g, b, x, y, x, y,
					(unsigned char)g, (unsigned char)b, 'g', 'b');

			//the byte for "B"
			bit(r, g, b, x, y, (unsigned char)b, 'b');

			//here we check if the next pixel is still on the line, otherwise
			//we take the value from the R byte of the pixel on the next line
			int next_r = 0, next_g = 0, next_b = 0, next_x = 0, next_y = 0;
			if (x + 1 < current->width) {
				next_x = x + 1;
				next_y = y;
			} else if (y - 1 >= 0) {
				next_x = 0;
				next_y = y - 1;
			}

			if (y - 1 >= 0 || x + 1 < current->width) {
				next_r = current->pixels[next_y][next_x].r;
				next_g = current->pixels[next_y][next_x].g;
				next_b = current->pixels[next_y][next_x].b;
				//the bytes from the intersection of "B" (current pixel)
				//and "R" (next pixel)
				bit_two(r, g, b, next_r, next_g, next_b, x, y, next_x, next_y,
						(unsigned char)b, (unsigned char)next_r,
						'b', 'r');
			}
		}
	}
}
