typedef struct ttt_player_state{
               
	char wins;
	char *initials;
	char location_x;
	char location_y;
	char symbol;
	char command;
 
} ttt_player_state;
 
typedef struct ttt_game_state{
 
	char state;
	char turns;

	struct ttt_player_state *p1;
	struct ttt_player_state *p2;

	char *score_board;
	char *game_board;
               
} ttt_game_state;
 
 
void initialize_game(struct ttt_game_state **game);
void initialize_player(struct ttt_player_state **p, char symbol);
void place_score(struct ttt_game_state **game);
void update_score(char *score_board, char p1, char p2);
void place_border(char *game_board);
void clear_ttt(char *game_board);
void print_game(struct ttt_game_state *game);
void get_initials(struct ttt_player_state *p);
void initialize_rematch(struct ttt_game_state *game, char winner);
int process_game(struct ttt_game_state *game);
void process_player(struct ttt_game_state *game, struct ttt_player_state *p);
int check_win(struct ttt_game_state *game, struct ttt_player_state *p);
int check_equal_step(char *arr, char start, char symbol, char step, char instances);