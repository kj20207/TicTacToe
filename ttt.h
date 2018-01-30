/// ///// Created by Kellen Johnson



// Length of the score board string
#define SCORE_BOARD_LENGTH 25
// Starting index for Player 1 within the game board
#define SCORE_BOARD_P1_POS 8
// Starting index for Player 2 within the game board
#define SCORE_BOARD_P2_POS 18
// Number of rows within game board
#define GAME_BOARD_ROWS 7
// Number of columns within game board
#define GAME_BOARD_COLS 7
// Optional Buffer (spaces printed prior to printing game board)
#define GAME_BOARD_BUFFER 9
 
// Symbol left by player 1
#define P1_SYMBOL 'X'
// Symbol denoting current player 1 position
#define P1_LABEL '1'
// Symbol left by player 2
#define P2_SYMBOL 'O'
// Symbol denoting current player 2 position
#define P2_LABEL '2'
// Border Symbols for TicTacToe game board
#define BORDER '#'
// Empty spaces within the game board
#define EMPTY ' '
 
/*
	Various input commands that players can make
*/
/*
	Movement
*/
#define UP 'w'
#define DOWN 's'
#define RIGHT 'd'
#define LEFT 'a'
/*
	Actions
*/
#define PLACE 'k'
#define QUIT 'q'
#define YES 'y'
#define NO 'n'


/*
	Player Struct
*/
typedef struct ttt_player_state{
               
	// number of wins the current player has
	char wins;
	// initials of the current player
	char *initials;
	// Current row of the player
	char location_x;
	// Current column of the player
	char location_y;
	// Symbol that player leaves on the board
	char symbol;
	// Symbol which represents current position of player on the board
	char label;
	// Current command of the player, correlates with defined values
	char command;
 
} ttt_player_state;
 
 /*
	Game Struct
 */
typedef struct ttt_game_state{
 
	// current state of the game
	char state;
	// previous state of the game, used for reverting back on "y/n" input
	char prev_state;
	// number of turns taken in the current game
	char turns;

	// pointer for player 1
	struct ttt_player_state *p1;
	// pointer for player 2
	struct ttt_player_state *p2;

	// score board string
	char *score_board;
	// game board array
	char *game_board;
               
} ttt_game_state;
 
// Initializes variables within a game struct
void initialize_game(struct ttt_game_state **game);

// Initializes variables within a player struct
void initialize_player(struct ttt_player_state **p, char symbol, char label);

// Creates the scoreboard within a game struct 
void place_score(struct ttt_game_state **game);

// Updates the score board to the input character scores
void update_score(char *score_board, char p1, char p2);

// Sets the BORDER spaces within the game board
void place_border(char *game_board);

// Sets all valid PLACE spaces to EMPTY
void clear_ttt(char *game_board);

// Prints all values within the game board
// Additionally, prints the players on top of the game board and prints the scoreboard to the screen
void print_game(struct ttt_game_state *game);

//
void get_initials(struct ttt_player_state *p);

// Clears the game board, and sets the current state to allow the previous winner to go first
void initialize_rematch(struct ttt_game_state *game, char winner);

// Reads input and processes a turn for the input player
void process_command(struct ttt_game_state *game, struct ttt_player_state *p);

// Main state machine of the game
// Handles various states of the game
int process_game(struct ttt_game_state *game);

// Process a turn for the current player
void process_player(struct ttt_game_state *game, struct ttt_player_state *p);

// Returns 1 if the current game board has a valid winning placement for the input player
int check_win(struct ttt_game_state *game, struct ttt_player_state *p);

// Helper function for checking valid combinations on the game board
int check_equal_step(char *arr, char start, char symbol, char step, char instances);