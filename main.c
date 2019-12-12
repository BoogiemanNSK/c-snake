#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 10
#define WIDTH 10
#define WIN_SCORE 10
#define BASE_UPDATE 100000.0
#define BASE_SPEED 0.0005
#define ACCELERATION 0.0001

void reprint_map(int **map, int **snake, int score, int snake_size) {
	system("clear");
	int snake_cell = 0;

	printf("SCORE: %d\n", score);
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			for (int k = 0; k < snake_size; k++) {
				if (snake[k] == &(map[i][j])) { snake_cell = 1; }			
			}
			if (snake_cell) {
				printf("*");
			} else {
				switch(map[i][j]) {
					case -1:
						printf("#");
						break;
					case 0:
						printf(" ");
						break;
					case 1:
						printf("$");
						break;
					default:
						break;
				}
			}
			snake_cell = 0;
		}
		printf("\n");
	}
}

void intro() {
	char *anything = malloc(128);
	printf("Welcome to the game of C-Snake! Get score of %d to win.\nType your name to begin: ", WIN_SCORE);
	scanf("%s", anything);
	free(anything);
}

int crossed_itself(int **snake, int snake_size) {
	for (int i = 1; i < snake_size; i++) {
		if (snake[0] == snake[i]) {
			return 1;
		}
	}
	return 0;
}

void generate_new_fruit(int **map, int **snake, int snake_size) {
	// TODO Generate new fruit, that is not inside snake
}

int vertical_input() {
	// TODO Check user Input for y_dir
}

int horizontal_input() {
	// TODO Check user Input for x_dir
}

int game_cycle() {
	// Initialize game
	int ended = 0, won = 0, score = 0;
	double update = BASE_UPDATE, speed = BASE_SPEED, timestamp = 0.0;
	int x_dir = 1, y_dir = 0, last_axis = 0;
	int snake_size = 1;

	int **map = (int**)malloc((HEIGHT + 2) * sizeof(int*));
	for (int i = 0; i < HEIGHT; i++) { map[i] = (int*)malloc((WIDTH + 2) * sizeof(int)); }
	for (int i = 0; i < HEIGHT; i++) {
		map[i][0] = -1;
		map[i][WIDTH - 1] = -1;
	}
	for (int i = 0; i < WIDTH; i++) {
		map[0][i] = -1;
		map[HEIGHT - 1][i] = -1;
	}
	for (int i = 1; i < HEIGHT - 1; i++) {
		for (int j = 1; j < WIDTH - 1; j++) {
			map[i][j] = 0;
		}
	}

	int **snake_parts = (int**)malloc((WIN_SCORE + 1) * sizeof(int*));
	snake_parts[0] = &(map[1][1]);

	int fruit_eaten = 0;
	generate_new_fruit(map, snake_parts, snake_size);

	// Initial map print
	reprint_map(map, snake_parts, score, snake_size);
	
	// Main Game Loop
	while (!ended) {
		// Check user input
		if (last_axis) {
			x_dir = horizontal_input();
			if (x_dir) { y_dir = 0; }
		} else {
			y_dir = vertical_input();
			if (y_dir) { x_dir = 0; }
		}
		
		// Update map each [update] clicks
		timestamp += speed;
		if (timestamp >= update) {
			// Check it snake will eat fruit
			if (*(snake_parts[0] + x_dir + y_dir * WIDTH) == 1) { 
				fruit_eaten = 1; 
			}

			// Update snake position
			if (fruit_eaten) { snake_parts[snake_size] = snake_parts[snake_size - 1]; }
			for (int i = snake_size - 1; i >= 1; i--) {
				snake_parts[i] = snake_parts[i - 1];
			}
			snake_parts[0] = snake_parts[0] + x_dir + y_dir * WIDTH;
			if ( x_dir ) { last_axis = 0; }
			if ( y_dir ) { last_axis = 1; }
			if (fruit_eaten) { 
				snake_size++;
				score++;
				generate_new_fruit(map, snake_parts, snake_size);
			}

			// Update map and timestamp
			reprint_map(map, snake_parts, score, snake_size);
			timestamp = 0.0;

			// Check win/lose conditions
			if (score == WIN_SCORE) {
				won = 1;
				ended = 1;
			}
			if (*snake_parts[0] == -1 || crossed_itself(snake_parts, snake_size)) {
				ended = 1;
			}
		}
	}

	// Free memory
	free(map);
	free(snake_parts);
	return won;
}

void outro(int is_won) {
	char *anything = malloc(128);
	if (is_won) {
		printf("Congrats, you have won! You are truly the best C-Snake player (and the only one)!\nType anything to exit.");
	} else {
		printf("You have lost, but do not worry, you still have 77 years 127 days 13 hours and 34 minutes to win!\nType anything to exit: ");
	}
	scanf("%s", anything);
	free(anything);
}

// MAIN //
int main() {
	intro();
	int won = game_cycle();
	outro(won);	
	return 0;
}
