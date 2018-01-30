/// ///// Created by Kellen Johnson

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ttt.h"
 
 
/*
	Running main method
*/ 
void main(){
	
	// Create Game
	struct ttt_game_state *game;
	initialize_game(&game);
	
	// Run the game while it's not in the exiting state
	while(process_game(game)){
		// Debugging String
		printf("State: %d, Turns: %d \n\n", game->state, game->turns);
	};
 
}
 
void initialize_game(struct ttt_game_state **game){
	
	// Initialize Memory for game pointer
	*game = malloc(sizeof(struct ttt_game_state));
	
	// Set initial state to -1, avoids terminating while loop in main
	(*game)->state = -1;
	(*game)->prev_state = -1;
	// No turns taken 
	(*game)->turns = 0;

	// Initialize Players
	initialize_player(&((*game)->p1), P1_SYMBOL, P1_LABEL);
	initialize_player(&((*game)->p2), P2_SYMBOL, P2_LABEL);

	// Allocate Memory for Printing Arrays
	(*game)->score_board = malloc(sizeof(char) * SCORE_BOARD_LENGTH);
	(*game)->game_board = malloc(sizeof(char) * GAME_BOARD_COLS * GAME_BOARD_ROWS);

	// Create scoreboard string
	place_score(game);
	// Place BORDER within the gameboard
	place_border((*game)->game_board);
	// Set PLACEable spaces to EMPTY
	clear_ttt((*game)->game_board);
}
 
void initialize_player(struct ttt_player_state **p, char symbol, char label){
               
	// Allocate memory for player pointer
	*p = malloc(sizeof(struct ttt_player_state));
	// No wins
	(*p)->wins = 0;
	// Allocate memory for Initials String
	(*p)->initials = malloc(sizeof(char) * 4);
	// Set symbol to input symbol
	(*p)->symbol = symbol;
	// Set label to input label
	(*p)->label = label;
	// No command
	(*p)->command = 0;
	
	// Default location set to upper left
	(*p)->location_x = 1;
	(*p)->location_y = 1;
}
 
void place_score(struct ttt_game_state **game){
	// Embed printinable string within scoreboard array of input game
	sprintf((*game)->score_board, "--- P1: %d --- P2: %d ---\n", (int) (*game)->p1->wins, (int) (*game)->p2->wins);
}
 
void update_score(char *score_board, char p1, char p2){
	// add current wins to ASCII value for 'ZERO'
	// (Game should terminate once a player hits 9 wins
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
			printf("%c", (char) game->p1->label);
		}
		else if(game->state == 2 && i == (game->p2->location_x + (game->p2->location_y * GAME_BOARD_COLS))){
			printf("%c", (char) game->p2->label);
		}
		else printf("%c", game->game_board[i]);
		
		if(i % GAME_BOARD_COLS == GAME_BOARD_COLS - 1)
			printf("\n");
	}
	fflush(stdout);
}
 
void initialize_rematch(struct ttt_game_state *game, char winner){
	
	// CLEAR all PLACEd player symbols on board
	clear_ttt(game->game_board);
	// Clear turns in current game
	game->turns = 0;
	// Record previous state
	game->prev_state = game->state;
	// Set next turn to be the winner of previous game
	game->state = winner;
}
 
int process_game(struct ttt_game_state *game){
	switch(game->state){
		// Game initialization
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
		// Exiting Game
		case 0:
			printf("Thank you for playing!\n");
			fflush(stdout);
		break;  
		// Player 1's turn
		case 1:
			process_player(game, game->p1);
		break;
		// Player 2's turn
		case 2:
			process_player(game, game->p2);
		break;
		// Game completed (can choose to play again)
		case 3:
			// Allow playing again if wins aren't maxed out
			if(game->p1->wins < 9 && game->p2->wins < 9){
				printf("Would you like to play again? y/n: \n");
				fflush(stdout);
				process_player(game, game->p1);
			}
			else{
				printf("Max wins achieved! Good game!\n");
				fflush(stdout);
				game->state = 0;
			}
		break;
		// Quitting state. Must confirm to quit
		case 4:
			printf("Are you sure you'd like to quit? y/n: \n");
			fflush(stdout);
			process_player(game, game->p1);
		break;
	}
	// Update the scoreboard before printing
	update_score(game->score_board, game->p1->wins, game->p2->wins);
	// Print score board and game board
	print_game(game);

	// Return game state, keeps while loop running in main
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
 
	// State machine for processing input commands
	switch(p->command){
		
		// Move Up
		case 'w':
			if(p->location_y > 1)
				p->location_y -= 2;
		break;
		// Move down
		case 's':
			if(p->location_y < 5)
				p->location_y += 2;
		break;
		// Move Left
		case 'a':
			if(p->location_x > 1)
				p->location_x -= 2;
		break;
		// Move Right
		case 'd':
			if(p->location_x < 5)
				p->location_x += 2;
		break;
		// Place symbol
		case 'k':
			// Make sure current position is valid (EMPTY)
			if(game->game_board[p->location_y * GAME_BOARD_ROWS + p->location_x] == EMPTY){
				game->game_board[p->location_y * GAME_BOARD_ROWS + p->location_x] = p->symbol;
				
				// Increment turns
				game->turns += 1;
				// Record previous state
				game->prev_state = game->state;
				
				// Check to see if the current player has won
				if(check_win(game, p)){
					// Increment player wins
					p->wins += 1;
					// Move to end game state
					game->state = 3;
				}
				// If the board is filled (and nobody won)
				else if(game->turns == 9){
					// Move to end of game (Cat game)
					game->state = 3;
				}
				//	Toggle player turn
				else if(game->state == 1){
					game->state = 2;
				}
				// Toggle Player turn
				else{						
					game->state = 1;
				}
			}
		break;
		// Input for quitting the game
		case 'q':
			// Initialize quitting state
			if(game->state != 4){
				game->prev_state = game->state;
				game->state = 4;
			}
		break;
		case 'y':
			// Confirm rematch, rematch with winner going first
			if(game->state == 3)
				initialize_rematch(game, game->prev_state);
			// Confirm quitting
			else if(game->state == 4)
				game->state = 0;
		break;
		case 'n':
			// Deny rematch
			if(game ->state == 3)
				game->state = 0;
			// Deny quitting, return to previous state
			else if(game->state == 4)
				game->state = game->prev_state;
		break;
		// Should never happen, but simply clears the input command
		default:
			p->command = 0;
		break;
	}
	
	return;
}

void process_player(struct ttt_game_state *game, struct ttt_player_state *p){
	// Clear command
	p->command = 0;
	
	// Wait until a valid command is input
	while(!p->command){
		
		// Get the last entered character (prior to newline)
		char cmd = 0;
		char cmd_curr = 0;
		while(cmd_curr != '\n'){
			cmd = cmd_curr;
			cmd_curr = fgetc(stdin);
		}
		
		// Decipher command
		if(cmd == UP)
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
			p->command = 'n';
	}
	// Pass command onto state machine
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
 
	// Number of iterations to check
	char count = 0;
	while(count < instances){
		// If current character doesn't match the symbol, return false
		if(arr[start + step * count] != symbol){
			return 0;
		}
		count++;
	}
	
	// No odd symbols found, return true
	return 1;
}