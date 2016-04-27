#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "chesso.h"

int main()
{
	char nextMove[10];

	printf("Welcome to Chesso, by Alessandro Cocilova \n");
	printf("Please type your move in the format: StartcolStartrow.EndcolEndrow (es. a1.a2) \n\n\n");
	initChessboard();
	while (!playerHasWon()) { // senseless to pass the chessboard to this function, because there's only one chessboard
 	printChessboard();
 	do {
 		printf("Type your next move: ");
   		fgets(nextMove, 10, stdin);
 	} while (!moveIsLegal(decodePlayerMove(nextMove)));
   	move(nextMove);
 }

};

/*Inits the checkerboard, allocating space for 20 structs, one for each piece in game, initializing them and making the corresponding squares target them*/
void initChessboard(){
	/*Every empty square should be a nil pointer*/
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			chessboard[i][j] = NULL;
		}
	}

	for (int i = 0; i < 8; i++){
		chessboard[1][i] = malloc(sizeof(piece));

		chessboard[1][i]->myPieceType = PAWN;
		chessboard[1][i]->color = WHITE;
		chessboard[1][i]->avaiableForEnpassant = false;
	}

	chessboard[0][6] = malloc(sizeof(piece));

	chessboard[0][6]->myPieceType = KNIGHT;
	chessboard[0][6]->color = WHITE;
	chessboard[0][6]->avaiableForEnpassant = false;

	chessboard[0][4] = malloc(sizeof(piece));

	chessboard[0][4]->myPieceType = KING;
	chessboard[0][4]->color = WHITE;
	chessboard[0][4]->avaiableForEnpassant = false;

	for (int i = 0; i < 8; i++){
		chessboard[6][i] = malloc(sizeof(piece));

		chessboard[6][i]->myPieceType = PAWN;
		chessboard[6][i]->color = BLACK;
		chessboard[6][i]->avaiableForEnpassant = false;
	}

	chessboard[7][6] = malloc(sizeof(piece));

	chessboard[7][6]->myPieceType = KNIGHT;
	chessboard[7][6]->color = BLACK;
	chessboard[7][6]->avaiableForEnpassant = false;

	chessboard[7][4] = malloc(sizeof(piece));

	chessboard[7][4]->myPieceType = KING;
	chessboard[7][4]->color = BLACK;
	chessboard[7][4]->avaiableForEnpassant = false;

	/*Where i've to do the free()? When the games ends?*/

	/*Consider refactoring*/
	for (int i = 0; i<8; i++){
		playerPawnHasMoved[i] = false;
		cpuPawnHasMoved[i] = false;
	}
};

void printChessboard(){
	for (int i = 7; i>= 0; i--){
		printf("%d ", i+1);
		for (int j = 0; j < 8; j++){

			/*Empty square*/
			if (chessboard[i][j] == NULL) {
				if ( (i+1)%2 == 0){/*Pairwise row*/
					if ( (j+1)%2 == 0){/*Pairwise column*/
						printf("+ ");
					}
					else {
						printf("- ");
					}
				}
				else {
					if ( (j+1)%2 == 0){/*Pairwise column*/
						printf("- ");
					}
					else {
						printf("+ ");
					}
				}
			}

			/*Square occupied by a Pawn*/
			else if (chessboard[i][j]->myPieceType == PAWN) {
				if (chessboard[i][j]->color == WHITE) {
					printf("p ");
				} else {
					printf("d ");
				}
			}

			/*Square occupied by a Knight*/
			else if (chessboard[i][j]->myPieceType == KNIGHT) {
				if (chessboard[i][j]->color == WHITE) {
					printf("h ");
				} else {
					printf("c ");
				}
			}

			/*Square occupied by a King*/
			else if (chessboard[i][j]->myPieceType == KING) {
				if (chessboard[i][j]->color == WHITE) {
					printf("k ");
				} else {
					printf("r ");
				}
			}

		}
		printf("\n");
	}
	printf("  a b c d e f g h\n");
};

/*Moves a piece from its square to the specified square updating the model*/
void move(char playerMove[]){
	/*playerMove used just to provide a feedback to the player of CPU's move*/
	printf("%s\n", playerMove);

	if (chessboard[lastMove[0]][lastMove[1]] != NULL) {
		/*The square where i've moved the piece, now targets to the piece*/
		chessboard[lastMove[2]][lastMove[3]] = chessboard[lastMove[0]][lastMove[1]];
		/*Old square contains now a pointer to null, meaning that it has no more pieces*/
		chessboard[lastMove[0]][lastMove[1]] = NULL;
	}
	else perror("The selected square is empty");
};

/*Translates the player move from standard chess ('a1b1') form to numeric form*/
int* decodePlayerMove(char playerMove[]){
	int startRow, startCol, endRow, endCol;
	switch (playerMove[0]){
		case 'a': startCol = 0; break;
		case 'b': startCol = 1; break;
		case 'c': startCol = 2; break;
		case 'd': startCol = 3; break;
		case 'e': startCol = 4; break;
		case 'f': startCol = 5; break;
		case 'g': startCol = 6; break;
		case 'h': startCol = 7; break;
	}
	switch (playerMove[3]){
		case 'a': endCol = 0; break;
		case 'b': endCol = 1; break;
		case 'c': endCol = 2; break;
		case 'd': endCol = 3; break;
		case 'e': endCol = 4; break;
		case 'f': endCol = 5; break;
		case 'g': endCol = 6; break;
		case 'h': endCol = 7; break;
	}
	startRow = playerMove[1] - '0' -1;
	endRow = playerMove[4] - '0' -1;

	/*I'm ignoring the third char in the string, since it contains the dot*/
	/*Memorize them in row, col format!*/
	lastMove[0] = startRow;
	lastMove[1] = startCol;
	lastMove[2] = endRow;
	lastMove[3] = endCol;
	//printf("Dentro decode %d %d %d %d\n", startCol, startRow, endCol, endRow);

	return lastMove;
}

/*Checks if the passed move is legal. All other functions use global variable 'lastMove'. More elegant to make it uniform?*/
bool moveIsLegal(int move[]){

	/*Piece not in the board limits*/
	if (move[0] < 0 | move[0] > 8 | move[1] < 0 | move[1] > 8 | move[2] < 0 | move[2] > 8 | move[3] < 0 | move[3] > 8) {
		if (isPlayerTurn) printf("Illegal move: specified move outside board limits\n");
		return 0;
	}
	/*Trying to move from and empty square. In the matrix order is row-col, not col-row, so the order is inverted*/
	if ( chessboard[move[0]][move[1]] == NULL ) {
		if (isPlayerTurn) printf("Illegal move: trying to move an empty square\n");
		return 0;
	}

	/*Trying to move adversary's pieces*/
	if ( ((chessboard[move[0]][move[1]]->color == BLACK) & isPlayerTurn) | 
		 ((chessboard[move[0]][move[1]]->color == WHITE) & !isPlayerTurn) ) {
		if (isPlayerTurn) printf("Illegal move: trying to move piece that belongs to the adversary\n");
		return 0;
	}

	if (chessboard[move[0]][move[1]]->myPieceType == PAWN){

		if (chessboard[move[0]][move[1]]->color == WHITE){
			int tmp = move[1] - move[3];
			/*Single step move*/
			if (move[1] == move[3]){
				/*Pawn that goes on in straight line, cannot eat another piece*/
				if (chessboard[move[2]][move[3]] == NULL){
						if (move[2] - move[0] == 1) {
						playerPawnHasMoved[move[1]] = true;
						return 1;
					}
					/*Double step move*/
					else if ( (move[2] - move[0] == 2) & !playerPawnHasMoved[move[1]] & move[0] == 1) { /*added pawn's line check, otherwise a pawn that has changed his row eating, could use another's pawn +2 bonus*/
						playerPawnHasMoved[move[1]] = true;
						return 1;
					}
				}
			}
			/*If I move to a next column, legit only if i'm eating an adversary's piece*/
			else if (tmp*tmp == 1){
				if (chessboard[move[2]][move[3]]->color == BLACK) {
					playerPawnHasMoved[move[1]] = true;
					return 1;
				}
			}
			/*EN PASSANT TO BE IMPLENTED!*/
		}

		/*IF COLOR = BLACK*/


	}

	return 0;
};

bool playerHasWon(){
	//temporary placeholder function
	return 0;
};