#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "chesso.h"

int main()
{
	char nextMove[10];
	for (int i = 0; i<8; i++){
		playerPawnHasMoved[i] = false;
		cpuPawnHasMoved[i] = false;
	}
	printf("Welcome to Chesso, by Alessandro Cocilova \n");
	printf("Please type your move in the format: StartcolStartrow.EndcolEndrow (es. a1.a2) \n\n\n");
	initChessboard();
	initPieces();
	while (!playerHasWon()) { // senseless to pass the chessboard to this function, because there's only one chessboard
 	printChessboard();
 	do {
 		printf("Type your next move: ");
   		fgets(nextMove, 10, stdin);
 	} while (!moveIsLegal(decodePlayerMove(nextMove)));
   	move(nextMove);
 }

};

/*Inits the View side of the checkerboard*/
void initChessboard(){
	strcpy(row0, "  a b c d e f g h");
	strcpy(col0, "12345678");
	strcpy(matchChessboard[7], "-+-+r+c+");
	strcpy(matchChessboard[6], "dddddddd");
	strcpy(matchChessboard[5], "-+-+-+-+");
	strcpy(matchChessboard[4], "+-+-+-+-");
	strcpy(matchChessboard[3], "-+-+-+-+");
	strcpy(matchChessboard[2], "+-+-+-+-");
	strcpy(matchChessboard[1], "pppppppp");
	strcpy(matchChessboard[0], "+-+-k-h-");
};

/*Inits the Controller side of the checkerboard*/
void initPieces(){
	for (int i = 0; i < 8; i++){
		gamePieces[i].position[0] = 1;
		gamePieces[i].position[1] = i;
		gamePieces[i].myPieceType = PAWN;
		gamePieces[i].avaiableForEnpassant = false;
	}
	gamePieces[8].position[0] = 0;
	gamePieces[8].position[1] = 6;
	gamePieces[8].myPieceType = KNIGHT;
	gamePieces[8].avaiableForEnpassant = false;

	gamePieces[9].position[0] = 0;
	gamePieces[9].position[1] = 4;
	gamePieces[9].myPieceType = KING;
	gamePieces[9].avaiableForEnpassant = false;

	for (int i = 10; i < 18; i++){
		gamePieces[i].position[0] = 6;
		gamePieces[i].position[1] = i;
		gamePieces[i].myPieceType = PAWN;
		gamePieces[i].avaiableForEnpassant = false;
	}
	gamePieces[18].position[0] = 7;
	gamePieces[18].position[1] = 6;
	gamePieces[18].myPieceType = KNIGHT;
	gamePieces[18].avaiableForEnpassant = false;

	gamePieces[19].position[0] = 7;
	gamePieces[19].position[1] = 4;
	gamePieces[19].myPieceType = KING;
	gamePieces[19].avaiableForEnpassant = false;
};

void printChessboard(){

	for (int i = 7; i >= 0; i--){ // because row8 has to be printfed first
		printf("%c", col0[i]);
		for (int j = 0; j < 8; j++){
			printf(" %c", matchChessboard[i][j]);
		}
		printf("\n");
	}
	printf("%s\n", row0); 

};

/*Moves a piece from its square to the specified square*/
void move(char playerMove[]){
	/*playerMove used just to provide a feedback to the player of CPU's move*/
	printf("%s\n", playerMove);
	matchChessboard[lastMove[3]][lastMove[2]] = matchChessboard[lastMove[1]][lastMove[0]];
	updateChessboard();
};

/*Fixes the empty squares after the moves*/
void updateChessboard(){
	if ( (lastMove[1]+1) % 2 == 1){
		if ( (lastMove[0]+1) % 2 == 1){
			matchChessboard[lastMove[1]][lastMove[0]] = '+';
		}
		else{
			matchChessboard[lastMove[1]][lastMove[0]] = '-';
		}
	}
	else{
		if ( (lastMove[0]+1) % 2 == 1){
			matchChessboard[lastMove[1]][lastMove[0]] = '-';
		}
		else{
			matchChessboard[lastMove[1]][lastMove[0]] = '+';
		}
	}
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
	lastMove[0] = startCol;
	lastMove[1] = startRow;
	lastMove[2] = endCol;
	lastMove[3] = endRow;
	//printf("Dentro decode %d %d %d %d\n", startCol, startRow, endCol, endRow);

	return lastMove;
}

/*Checks if the passed move is legal. All other functions use global variable 'lastMove'. More elegant to make it uniform?*/
bool moveIsLegal(int playerMoveDecoded[]){
	//printf("Dentro moveIsLegal %d %d %d %d\n", playerMoveDecoded[0], playerMoveDecoded[1], playerMoveDecoded[2], playerMoveDecoded[3]);
	/*Piece not in the board limits*/
	if (playerMoveDecoded[0] < 0 | playerMoveDecoded[0] > 8 | playerMoveDecoded[1] < 0 | playerMoveDecoded[1] > 8 | playerMoveDecoded[2] < 0 | playerMoveDecoded[2] > 8 | playerMoveDecoded[3] < 0 | playerMoveDecoded[3] > 8) {
		if (isPlayerTurn) printf("Illegal move: specified move outside board limits\n");
		return 0;
	}
	/*Trying to move from and empty square. In the matrix order is row-col, not col-row, so the order is inverted*/
	if ( (matchChessboard[playerMoveDecoded[1]][playerMoveDecoded[0]] =='+') | (matchChessboard[playerMoveDecoded[1]][playerMoveDecoded[0]] =='-') ) {
		if (isPlayerTurn) printf("Illegal move: trying to move an empty square\n");
		return 0;
	}

	/*Trying to move adversary's pieces*/
	if ( ((matchChessboard[playerMoveDecoded[1]][playerMoveDecoded[0]] =='d') & isPlayerTurn) | 
		 ((matchChessboard[playerMoveDecoded[1]][playerMoveDecoded[0]] =='c') & isPlayerTurn) | 
		 ((matchChessboard[playerMoveDecoded[1]][playerMoveDecoded[0]] =='r') & isPlayerTurn) | 
		 ((matchChessboard[playerMoveDecoded[1]][playerMoveDecoded[0]] =='p') & !isPlayerTurn) | 
		 ((matchChessboard[playerMoveDecoded[1]][playerMoveDecoded[0]] =='h') & !isPlayerTurn) | 
		 ((matchChessboard[playerMoveDecoded[1]][playerMoveDecoded[0]] =='k') & !isPlayerTurn) ) {
		if (isPlayerTurn) printf("Illegal move: trying to move piece that belongs to the adversary\n");
		return 0;
	}

	/*MUST BE REFACTORED CONSIDERING THE NEW DATA STRUCTURES!!!*/
	
	/*Pawn moves for player*/
	if (matchChessboard[playerMoveDecoded[1]][playerMoveDecoded[0]] =='p') {
		int tmp = playerMoveDecoded[0] - playerMoveDecoded[2];

		if (playerMoveDecoded[0] == playerMoveDecoded[2]){
			if (playerMoveDecoded[3] - playerMoveDecoded[1] == 1) {
				playerPawnHasMoved[playerMoveDecoded[0]] = true;
				return 1;
			}
			else if ( (playerMoveDecoded[3] - playerMoveDecoded[1] == 2) & !playerPawnHasMoved[playerMoveDecoded[0]] & playerMoveDecoded[1] == 1) { /*added, otherwise a pawn that has changed his row eating, could use another's pawn +2 bonus*/
				playerPawnHasMoved[playerMoveDecoded[0]] = true;
				return 1;
			}
		}
		/*If I move to a next column*/
		else if (tmp*tmp == 1){
			if ( (matchChessboard[playerMoveDecoded[3]][playerMoveDecoded[2]] =='d') | (matchChessboard[playerMoveDecoded[3]][playerMoveDecoded[2]] =='c') | 
				(matchChessboard[playerMoveDecoded[3]][playerMoveDecoded[2]] =='r') ) {
				playerPawnHasMoved[playerMoveDecoded[0]] = true;
				return 1;
			}
		}
		/*EN PASSANT TO BE IMPLENTED!*/
	}

	/*Pawn moves for cpu*/
	if (matchChessboard[playerMoveDecoded[1]][playerMoveDecoded[0]] =='d') {
		int tmp = playerMoveDecoded[0] - playerMoveDecoded[2];

		if (playerMoveDecoded[0] == playerMoveDecoded[2]){
			if (playerMoveDecoded[3] - playerMoveDecoded[1] == 1) {
				playerPawnHasMoved[playerMoveDecoded[0]] = true;
				return 1;
			}
			else if ( (playerMoveDecoded[3] - playerMoveDecoded[1] == 2) & !playerPawnHasMoved[playerMoveDecoded[0]] & playerMoveDecoded[1] == 1) {
				playerPawnHasMoved[playerMoveDecoded[0]] = true;
				return 1;
			}
		}
		/*If I move to a next column*/
		else if (tmp*tmp == 1){
			if ( (matchChessboard[playerMoveDecoded[3]][playerMoveDecoded[2]] =='p') | (matchChessboard[playerMoveDecoded[3]][playerMoveDecoded[2]] =='h') | 
				(matchChessboard[playerMoveDecoded[3]][playerMoveDecoded[2]] =='k') ) {
				playerPawnHasMoved[playerMoveDecoded[0]] = true;
				return 1;
			}
		}
		/*EN PASSANT TO BE IMPLENTED!*/
	}

	return 0;
};

bool playerHasWon(){
	//temporary placeholder function
	return 0;
};