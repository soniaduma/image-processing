// DUMA SONIA NICOLA 315CA
typedef struct lsystem_struct {
	char *axiom;
	char *p_rules; // p implies q, A->AB (here are stored 'A's)
	char **q_rules; // (here are stored the rule for p,'AB's)
	int n_rules;
} l_system;

typedef struct {
	unsigned char r, g, b;
} pixel;

typedef struct {
	int width;
	int height;
	int max_val;
	pixel **pixels;
} image;

typedef struct {
	int ascii_code;
	// how much the cursor moves after drawing the character:
	int x; //horizontal
	int y; // vertical
	int width;
	int height;
	//how much to shift the bitmap:
	int next_x; // offset for x
	int next_y; // offset for y
	int **pixels;
} character;

typedef struct {
	char *font_name;
	character **chr;
	int chars_size;
} font_bdf;

typedef struct state {
	l_system *lsys;
	image *img;
	font_bdf *font;
	char *cli_command;
} state_t;

typedef struct all_states_struct {
	state_t **history;
	int history_size;
	char **redos;
	int redos_size;
} all_states;

void undo(all_states *states);
void redo(all_states *states);

void create_state(all_states *states, char *cli_command);
state_t *current_state(all_states *states);
void free_current_state(all_states *states);

void deepcopy_previous_lsystem(all_states *states);
void deepcopy_previous_font(all_states *states);
void deepcopy_previous_image(all_states *states);

void clear_redos(all_states *states);

void type(all_states *states, char *args);
void font(all_states *states, char *file);
void free_font(state_t *state);

void bitcheck(all_states *states);
