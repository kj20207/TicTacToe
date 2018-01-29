#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ttt.h"
 
#define SCORE_BOARD_LENGTH 25
#define SCORE_BOARD_P1_POS 8
#define SCORE_BOARD_P2_POS 18
#define GAME_BOARD_ROWS 7
#define GAME_BOARD_COLS 7
#define GAME_BOARD_BUFFER 9
 
#define P1 'X'
#define P1_SYMBOL '1'
#define P2 'O'
#define P2_SYMBOL '2'
#define BORDER '#'
#define EMPTY ' '
 
#define UP 'w'
#define DOWN 's'
#define RIGHT 'd'
#define LEFT 'a'
#define PLACE 'k'
#define QUIT 'q'
#define YES 'y'
#define NO 'n'
 
 
void main(){
	struct ttt_game_state *game;
	initialize_game(&game);
	while(process_game(game)){
		printf("State: %d, Turns: %d \n\n", game->state, game->turns);
	};
 
}
 
void initialize_game(struct ttt_game_state **game){
	*game = malloc(sizeof(struct ttt_game_state));
	(*game)->state = -1;
	(*game)->prev_state = -1;
	(*game)->turns = 0;

	initialize_player(&((*game)->p1), P1_SYMBOL);
	initialize_player(&((*game)->p2), P2_SYMBOL);

	(*game)->score_board = malloc(sizeof(char) * SCORE_BOARD_LENGTH);
	(*game)->game_board = malloc(sizeof(char) * GAME_BOARD_COLS * GAME_BOARD_ROWS);

	place_score(game);
	place_border((*game)->game_board);
	clear_ttt((*game)->game_board);
	
}
 
void initialize_player(struct ttt_player_state **p, char symbol){
               
	*p = malloc(sizeof(struct ttt_player_state));
	(*p)->wins = 0;
	(*p)->initials = malloc(sizeof(char) * 4);
	(*p)->symbol = symbol;
	(*p)->command = 0;
	(*p)->location_x = 1;
	(*p)->location_y = 1;
}
 
void place_score(struct ttt_game_state **game){
	sprintf((*game)->score_board, "--- P1: %d --- P2: %d ---\n", (int) (*game)->p1->wins, (int) (*game)->p2->wins);
}
 
void update_score(char *score_board, char p1, char p2){
	score_board[SCORE_BOARD_P1_POS] = 48 + p1;
	score_board[SCORE_BOARD_P2_POS] = 48 + p2;
}
 
void place_border(char *game_board){
	int i, j;

	// Calculate the starting position for all rows within the game board
	int rows[GAME_BOARD_ROWS + 1];
	for(i = 0; i < GAME_BOARD_COLS + 1; i++){
		rows[i] = i * GAME_BOARD_COLS;
	}

	// Fills EVEN rows with border from 0 - 6
	for(i = 0; i < GAME_BOARD_ROWS; i+= 2){
		for(j = rows[i]; j < rows[i + 1]; j++){
			game_board[j] = (char) BORDER;
		}
	}

	// Refactor 1
	// Fills EVEN columns (0 - 6) within ODD rows with border from 1 - 5
	for(i = 1; i < GAME_BOARD_ROWS; i+= 2){
		for(j = 0; j < GAME_BOARD_COLS; j+= 2){
			game_board[rows[i] + j] = (char) BORDER;
		}
	}
}
 
void clear_ttt(char *game_board){
	int i, j;
   
	// Calculate the starting position for all rows within the game board
	int rows[GAME_BOARD_ROWS + 1];
	for(i = 0; i < GAME_BOARD_COLS + 1; i++){
		rows[i] = i * GAME_BOARD_COLS;
	}
	// Refactor 1
	for(i = 1; i < GAME_BOARD_ROWS; i+= 2){
		for(j = 1; j < GAME_BOARD_COLS; j+=2){
			game_board[rows[i] + j] = (char) EMPTY;
		}
	}
}
 
// BUG - This needs to be edited to print current player position over the board
void print_game(struct ttt_game_state *game){
 
	int i;
	printf("\n%s", game->score_board);
	for(i = 0; i < GAME_BOARD_ROWS * GAME_BOARD_COLS; i++){
		
		if(game->state == 1 && i == (game->p1->location_x + (game->p1->location_y * GAME_BOARD_COLS))){
			printf("%c", (char) game->p1->symbol);
		}
		else if(game->state == 2 && i == (game->p2->location_x + (game->p2->location_y * GAME_BOARD_COLS))){
			printf("%c", (char) game->p2->symbol);
		}
		else printf("%c", game->game_board[i]);
		
		if(i % GAME_BOARD_COLS == GAME_BOARD_COLS - 1)
			printf("\n");
	}
	fflush(stdout);
}
 
void initialize_rematch(struct ttt_game_state *game, char winner){
	clear_ttt(game->game_board);
	game->turns = 0;
	game->prev_state = game->state;
	game->state = winner;
}
 
int process_game(struct ttt_game_state *game){
	switch(game->state){
		case -1:
			printf("Hello, and welcome to Tic-Tac-Toe!\n");
			printf("Please enter 3 initials for Player 1:\n");
			fflush(stdout);
			//get_initials(game->p1);
			printf("Please enter 3 initials for Player 2: ");
			fflush(stdout);
			//get_initials(game->p2);
			game->prev_state = game->state;
			game->state = 1;
		break;
		case 0:
			printf("Thank you for playing!\n");
			fflush(stdout);
		break;  
		case 1:
			process_command(game, game->p1);
		break;
		case 2:
			process_command(game, game->p2);
		break;
		case 3:
			printf("Would you like to play again? y/n: \n");
			fflush(stdout);
			process_command(game, game->p1);
		break;
		case 4:
			printf("Are you sure you'd like to quit? y/n: \n");
			fflush(stdout);
			process_command(game, game->p1);
		break;
	}
	update_score(game->score_board, game->p1->wins, game->p2->wins);
	print_game(game);

	return game->state;
}
 
void get_initials(struct ttt_player_state *p){
	char initials[4];
	fgets(initials, 4, stdin);
	int i = 0;
	for(i = 0; i < strlen(initials); i++){
		p->initials[i] = initials[i];
	}
}
 
void process_command(struct ttt_game_state *game, struct ttt_player_state *p){
	p->command = 0;

	while(!p->command){
		char cmd_curr = 0;
		while(cmd_curr != '\n'){
			p->command = cmd_curr;
			cmd_curr = fgetc(stdin);
		}
 
		switch(p->command){
			case 'w':
				if(p->location_y > 1)
					p->location_y -= 2;
			break;
			case 's':
				if(p->location_y < 5)
					p->location_y += 2;
			break;
			case 'a':
				if(p->location_x > 1)
					p->location_x -= 2;
			break;
			case 'd':
				if(p->location_x < 5)
					p->location_x += 2;
			break;
			case 'k':
				if(game->game_board[p->location_y * GAME_BOARD_ROWS + p->location_x] == EMPTY){
					game->game_board[p->location_y * GAME_BOARD_ROWS + p->location_x] = p->symbol;
					
					game->turns += 1;
					printf("Place\n");
					
					game->prev_state = game->state;
					if(check_win(game, p)){
						p->wins += 1;
						game->state = 3;
					}
					else if(game->turns == 9){
						game->state = 3;
					}
					else if(game->state == 1){
						game->state = 2;
					}
					else{						
						game->state = 1;
					}
				}
			break;
			case 'q':
				if(game->state != 4){
					game->prev_state = game->state;
					game->state = 4;
				}
			break;
			case 'y':
				if(game->state == 3)
					initialize_rematch(game, game->prev_state);
				else if(game->state == 4)
					game->state = 0;
			break;
			case 'n':
				if(game ->state == 3)
					game->state = 0;
				else if(game->state == 4)
					game->state = game->prev_state;
			break;
			default:
				p->command = 0;
			break;
		}
	}
	printf("%c", p->command);
	p->command = 0;
	return;
}

void process_player(struct ttt_game_state *game, struct ttt_player_state *p){
	p->command = 0;

	while(!p->command){
		char cmd = 0;
		char cmd_curr = 0;
		while(cmd_curr != '\n'){
			cmd = cmd_curr;
			cmd_curr = fgetc(stdin);
		}
		
		/*if(cmd == UP)
			p->command = 'w';
		else if(cmd == DOWN)
			p->command = 's';
		else if(cmd == LEFT)
			p->command = 'a';
		else if(cmd == RIGHT)
			p->command = 'd';
		else if(cmd == PLACE)
			p->command = 'k';
		else if(cmd == QUIT)
			p->command = 'q';
		else if(cmd == YES)
			p->command = 'y';
		else if(cmd == NO)
			p->command = 'n';*/
	}
	return process_command(game, p);
 
}
 
int check_win(struct ttt_game_state *game, struct ttt_player_state *p){
	
	//printf("Test %d\n", game->turns);
	fflush(stdout);
	if(game->turns < 5){
		//printf("hello");
		fflush(stdout);
		return 0;
	}
	//printf("helloooo");
	fflush(stdout);
	int i, j;
   
	//printf("Test");
	fflush(stdout);
	// Calculate the starting position for all rows within the game board
	// Refactor 1
	int rows[GAME_BOARD_ROWS + 1];
	for(i = 0; i < GAME_BOARD_COLS + 1; i++){
		rows[i] = i * GAME_BOARD_COLS;
	}
 
	//printf("Test2");
	fflush(stdout);
 
	for(i = 1; i < GAME_BOARD_ROWS; i+=2){
		if(check_equal_step(game->game_board, rows[i] + 1, p->symbol, 2, 3)){
			printf("Horizontal win!\n");
			fflush(stdout);
			return 1;
		}
	
	}
		for(j = 1; j < GAME_BOARD_COLS; j+=2){
			if(check_equal_step(game->game_board, rows[1] + j, p->symbol, GAME_BOARD_COLS * 2, 3) ){
				printf("Vertical win! \n");
				fflush(stdout);
				return 1;
			}
		}
    //printf("Test3");
	fflush(stdout);           
			   
   if(check_equal_step(game->game_board, rows[1] + 1, p->symbol, (GAME_BOARD_COLS * 2) + 2, 3) | check_equal_step(game->game_board, rows[5] + 1, p->symbol, (-1 * GAME_BOARD_COLS * 2) + 2, 3)){
	   printf("Great win! \n");
	   fflush(stdout);
	   return 1;
   }
	
	printf("No win"); fflush(stdout);
	return 0;
               
}
 
int check_equal_step(char *arr, char start, char symbol, char step, char instances){
	//printf("Testlkjdf");
	fflush(stdout);
 
	char count = 0;
	while(count < instances){
		printf("Count: %d, Ind: %d\n", count, start + step * count);
		if(arr[start + step * count] != symbol){
			//printf("No");
			fflush(stdout);
			return 0;
		}
		count++;
	}
	//printf("Yes");
	fflush(stdout);
	return 1;
}