#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "chesso.h"

int main()
{
	/*can refactor this?*/
	char nextMove[10];
	char* encodedMove;

	printf("Welcome to Chesso, by Alessandro Cocilova \n");
	printf("Please type your move in the format: StartcolStartrow.EndcolEndrow (es. a1.a2) \n\n\n");
	initChessboard();
	while (!playerHasWon()) {
 		printChessboard();
 		if (isPlayerTurn){
 			do {
 				printf("Type your next move: ");
   				fgets(nextMove, 10, stdin);
 			} while (!moveIsLegal(decodePlayerMove(nextMove)));
 		}
 		else {
 			do {
 				generateRandomMove();
 			} while (!moveIsLegal(lastMove));
 			encodedMove = encodeMove();
 			strcpy(nextMove, encodedMove);
 			free(encodedMove);
 		}
 	
   		move(nextMove);
   		resetEnPassant();
   		isPlayerTurn = !isPlayerTurn;
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
		chessboard[1][i]->position[0] = 1;
		chessboard[1][i]->position[1] = i;

		/*In this complementary data structure I memorize all the white pieces*/
		whitePieces[i] = chessboard[1][i];
	}

	chessboard[0][6] = malloc(sizeof(piece));

	chessboard[0][6]->myPieceType = KNIGHT;
	chessboard[0][6]->color = WHITE;
	chessboard[0][6]->avaiableForEnpassant = false;
	chessboard[0][6]->position[0] = 0;
	chessboard[0][6]->position[1] = 6;

	whitePieces[8] = chessboard[0][6];

	chessboard[0][4] = malloc(sizeof(piece));

	chessboard[0][4]->myPieceType = KING;
	chessboard[0][4]->color = WHITE;
	chessboard[0][4]->avaiableForEnpassant = false;
	chessboard[0][4]->position[0] = 0;
	chessboard[0][4]->position[1] = 4;

	whitePieces[9] = chessboard[0][4];

	for (int i = 0; i < 8; i++){
		chessboard[6][i] = malloc(sizeof(piece));

		chessboard[6][i]->myPieceType = PAWN;
		chessboard[6][i]->color = BLACK;
		chessboard[6][i]->avaiableForEnpassant = false;
		chessboard[6][i]->position[0] = 6;
		chessboard[6][i]->position[1] = i;

		/*In this complementary data structure I memorize all the black pieces*/
		blackPieces[i] = chessboard[6][i];
	}

	chessboard[7][6] = malloc(sizeof(piece));

	chessboard[7][6]->myPieceType = KNIGHT;
	chessboard[7][6]->color = BLACK;
	chessboard[7][6]->avaiableForEnpassant = false;
	chessboard[7][6]->position[0] = 7;
	chessboard[7][6]->position[1] = 6;

	blackPieces[8] = chessboard[7][6];

	chessboard[7][4] = malloc(sizeof(piece));

	chessboard[7][4]->myPieceType = KING;
	chessboard[7][4]->color = BLACK;
	chessboard[7][4]->avaiableForEnpassant = false;
	chessboard[7][4]->position[0] = 7;
	chessboard[7][4]->position[1] = 4;

	blackPieces[9] = chessboard[7][4];

	/*Where i've to do the free()? When the games ends?*/

	/*Consider refactoring*/
	for (int i = 0; i<8; i++){
		whitePawnHasMoved[i] = false;
		blackPawnHasMoved[i] = false;
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
	/*playerMove used just to provide a feedback to the player of AI's move*/
	if (isPlayerTurn) printf("\nWhite moves: %s\n", playerMove);
	else printf("\nBlack moves: %s\n\n", playerMove);

	if (chessboard[lastMove[0]][lastMove[1]] != NULL) {
		/*The square where i've moved the piece, now targets to the piece*/
		chessboard[lastMove[2]][lastMove[3]] = chessboard[lastMove[0]][lastMove[1]];
		/*Update the piece's internal data structure*/
		chessboard[lastMove[2]][lastMove[3]]->position[0] = lastMove[2];
		chessboard[lastMove[2]][lastMove[3]]->position[1] = lastMove[3];
		/*Old square contains now a pointer to null, meaning that it has no more pieces*/
		chessboard[lastMove[0]][lastMove[1]] = NULL;
		if(enPassantExploited) {
			/*Remove the piece from a square even if i didn't move a piece in it*/
			chessboard[lastMove[0]][lastMove[3]] = NULL;
			enPassantExploited = false;
		}
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

bool isCheck(int move[], pieceColor myColor){
	int kingPos[2];
	kingPos[0] = move[2];
	kingPos[1] = move[3];

	/*Knight and King attacks in the same way for black and white*/
	/*Can be attacked by an adversary's knight?*/
	if (kingPos[0]+2 < 8){
		if (kingPos[1]+1 < 8) {
			if (chessboard[kingPos[0]+2][kingPos[1]+1] != NULL){
				if ((chessboard[kingPos[0]+2][kingPos[1]+1]->color != myColor ) & 
					(chessboard[kingPos[0]+2][kingPos[1]+1]->myPieceType == KNIGHT )) return 1;
			}				
		}
		if (kingPos[1]-1 >= 0) {
			if (chessboard[kingPos[0]+2][kingPos[1]-1] != NULL){
				if ((chessboard[kingPos[0]+2][kingPos[1]-1]->color != myColor ) & 
					(chessboard[kingPos[0]+2][kingPos[1]-1]->myPieceType == KNIGHT )) return 1;
			}	
		}
	}
	if (kingPos[0]-2 >= 0){
		if (kingPos[1]+1 < 8) {
			if (chessboard[kingPos[0]-2][kingPos[1]+1] != NULL){
				if ((chessboard[kingPos[0]-2][kingPos[1]+1]->color != myColor ) & 
					(chessboard[kingPos[0]-2][kingPos[1]+1]->myPieceType == KNIGHT )) return 1;
			}
		}
		if (kingPos[1]-1 >= 0) {
			if (chessboard[kingPos[0]-2][kingPos[1]-1] != NULL){
				if ((chessboard[kingPos[0]-2][kingPos[1]-1]->color != myColor ) & 
					(chessboard[kingPos[0]-2][kingPos[1]-1]->myPieceType == KNIGHT )) return 1;
			}
		}
	}
	if (kingPos[1]+2 < 8){
		if (kingPos[0]+1 < 8) {
			if (chessboard[kingPos[0]+1][kingPos[1]+2] != NULL){
				if ((chessboard[kingPos[0]+1][kingPos[1]+2]->color != myColor ) & 
					(chessboard[kingPos[0]+1][kingPos[1]+2]->myPieceType == KNIGHT )) return 1;
			}
		}
		if (kingPos[0]-1 >= 0) {
			if (chessboard[kingPos[0]-1][kingPos[1]+2] != NULL){
				if ((chessboard[kingPos[0]-1][kingPos[1]+2]->color != myColor ) & 
					(chessboard[kingPos[0]-1][kingPos[1]+2]->myPieceType == KNIGHT )) return 1;
			}
		}
	}
	if (kingPos[1]-2 >= 0){
		if (kingPos[0]+1 < 8) {
			if (chessboard[kingPos[0]+1][kingPos[1]-2] != NULL){
				if ((chessboard[kingPos[0]+1][kingPos[1]-2]->color != myColor ) & 
					(chessboard[kingPos[0]+1][kingPos[1]-2]->myPieceType == KNIGHT )) return 1;
			}
		}
		if (kingPos[0]-1 >= 0) {
			if (chessboard[kingPos[0]-1][kingPos[1]-2] != NULL){
				if ((chessboard[kingPos[0]-1][kingPos[1]-2]->color != myColor ) & 
					(chessboard[kingPos[0]-1][kingPos[1]-2]->myPieceType == KNIGHT )) return 1;
			}
		}
	}
	/*Can be attacked by adversary's king?*/
	if (kingPos[0]+1 < 8){
		if (kingPos[1]+1 < 8) {
			if (chessboard[kingPos[0]+1][kingPos[1]+1] != NULL){
				if ((chessboard[kingPos[0]+1][kingPos[1]+1]->color != myColor ) & 
					(chessboard[kingPos[0]+1][kingPos[1]+1]->myPieceType == KING )) return 1;
			}
		}
		if (kingPos[1]-1 >= 0) {
			if (chessboard[kingPos[0]+1][kingPos[1]-1] != NULL){
				if ((chessboard[kingPos[0]+1][kingPos[1]-1]->color != myColor ) & 
					(chessboard[kingPos[0]+1][kingPos[1]-1]->myPieceType == KING )) return 1;
			}
		}
	}
	if (kingPos[0]-1 >= 0){
		if (kingPos[1]+1 < 8) {
			if (chessboard[kingPos[0]-1][kingPos[1]+1] != NULL){
				if ((chessboard[kingPos[0]-1][kingPos[1]+1]->color != myColor ) & 
					(chessboard[kingPos[0]-1][kingPos[1]+1]->myPieceType == KING )) return 1;
			}
		}
		if (kingPos[1]-1 >= 0) {
			if (chessboard[kingPos[0]-1][kingPos[1]-1] != NULL){
				if ((chessboard[kingPos[0]-1][kingPos[1]-1]->color != myColor ) & 
					(chessboard[kingPos[0]-1][kingPos[1]-1]->myPieceType == KING )) return 1;
			}
		}
	}
	if (kingPos[1]+1 < 8) {
		if (chessboard[kingPos[0]][kingPos[1]+1] != NULL){
			if ((chessboard[kingPos[0]][kingPos[1]+1]->color != myColor ) & 
				(chessboard[kingPos[0]][kingPos[1]+1]->myPieceType == KING )) return 1;
		}
	}
	if (kingPos[1]-1 >= 0) {
		if (chessboard[kingPos[0]][kingPos[1]-1] != NULL){
			if ((chessboard[kingPos[0]][kingPos[1]-1]->color != myColor ) & 
				(chessboard[kingPos[0]][kingPos[1]-1]->myPieceType == KING )) return 1;
		}
	}
	if (kingPos[0]+1 < 8) {
		if (chessboard[kingPos[0]+1][kingPos[1]] != NULL){
			if ((chessboard[kingPos[0]+1][kingPos[1]]->color != myColor ) & 
				(chessboard[kingPos[0]+1][kingPos[1]]->myPieceType == KING )) return 1;
		}
	}
	if (kingPos[0]-1 >= 0) {
		if (chessboard[kingPos[0]-1][kingPos[1]] != NULL){
			if ((chessboard[kingPos[0]-1][kingPos[1]]->color != myColor ) & 
				(chessboard[kingPos[0]-1][kingPos[1]]->myPieceType == KING )) return 1;
		}
	}

	/*Can be attacked by a Pawn?*/
	/*Black pawns attack from botton to top*/
	if (myColor == WHITE){
		if (kingPos[0]+1 < 8){
			if (kingPos[1]+1 < 8) {
				if (chessboard[kingPos[0]+1][kingPos[1]+1] != NULL){
					if ((chessboard[kingPos[0]+1][kingPos[1]+1]->color == BLACK ) & 
						(chessboard[kingPos[0]+1][kingPos[1]+1]->myPieceType == PAWN )) return 1;
				}
			}
			if (kingPos[1]-1 >= 0) {
				if (chessboard[kingPos[0]+1][kingPos[1]-1] != NULL){
					if ((chessboard[kingPos[0]+1][kingPos[1]-1]->color == BLACK ) & 
						(chessboard[kingPos[0]+1][kingPos[1]-1]->myPieceType == PAWN )) return 1;
				}
			}
		}
	} 
	/*White pawns attack from top to bottom*/
	else {
		if (kingPos[0]+1 < 8){
			if (kingPos[1]+1 < 8) {
				if (chessboard[kingPos[0]-1][kingPos[1]+1] != NULL){
					if ((chessboard[kingPos[0]-1][kingPos[1]+1]->color == WHITE ) & 
						(chessboard[kingPos[0]-1][kingPos[1]+1]->myPieceType == PAWN )) return 1;
				}
			}
			if (kingPos[1]-1 >= 0) {
				if (chessboard[kingPos[0]-1][kingPos[1]-1] != NULL){
					if ((chessboard[kingPos[0]-1][kingPos[1]-1]->color == WHITE ) & 
						(chessboard[kingPos[0]-1][kingPos[1]-1]->myPieceType == PAWN )) return 1;
				}
			}
		}	
	}

	

	return 0;
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
			/*Single step move*/
			if (move[1] == move[3]){
				/*Pawn that goes on in straight line, cannot eat another piece*/
				if (chessboard[move[2]][move[3]] == NULL){
						if (move[2] - move[0] == 1) {
						whitePawnHasMoved[move[1]] = true;
						return 1;
					}
					/*Double step move*/
					else if ( (move[2] - move[0] == 2) & !whitePawnHasMoved[move[1]] & move[0] == 1) { /*added pawn's line check, otherwise a pawn that has changed his row eating, could use another's pawn +2 bonus*/
						whitePawnHasMoved[move[1]] = true;
						whitePieces[move[1]]->avaiableForEnpassant = true;
						return 1;
					}
				}
			}
			/*If I move to a next column, legit only if i'm eating an adversary's piece of for en passant*/
			else if ( (move[2]-move[0] == 1) & ((move[3] - move[1]) == 1 | (move[3] - move[1]) == -1) ){
				if (chessboard[move[2]][move[3]] != NULL){
					if (chessboard[move[2]][move[3]]->color == BLACK) {
						whitePawnHasMoved[move[1]] = true;
						return 1;
					}
				}
				/*En passant capture*/
				else if (chessboard[move[0]][move[3]] != NULL){
					if (chessboard[move[0]][move[3]]->avaiableForEnpassant == true) {
					/*Not necessary update bool hasMoved (if it can capture en passant, has already moved)*/
					enPassantExploited = true;
					return 1; 
					}
				}
			}
		}

		if (chessboard[move[0]][move[1]]->color == BLACK){
			/*Single step move*/
			if (move[1] == move[3]){
				/*Pawn that goes on in straight line, cannot eat another piece*/
				if (chessboard[move[2]][move[3]] == NULL){
						if (move[2] - move[0] == -1) {
						blackPawnHasMoved[move[1]] = true;
						return 1;
					}
					/*Double step move*/
					else if ( (move[2] - move[0] == -2) & !blackPawnHasMoved[move[1]] & move[0] == 6) { /*added pawn's line check, otherwise a pawn that has changed his row eating, could use another's pawn +2 bonus*/
						blackPawnHasMoved[move[1]] = true;
						blackPieces[move[1]]->avaiableForEnpassant = true;
						return 1;
					}
				}
			}
			/*If I move to a next column, legit only if i'm eating an adversary's piece of for en passant*/
			else if ( (move[2]-move[0] == -1) & ((move[3] - move[1]) == 1 | (move[3] - move[1]) == -1) ){
				if (chessboard[move[2]][move[3]] != NULL){
					if (chessboard[move[2]][move[3]]->color == WHITE) {
						blackPawnHasMoved[move[1]] = true;
						return 1;
					}
				}
				/*En passant capture*/
				else if (chessboard[move[0]][move[3]] != NULL){
					if (chessboard[move[0]][move[3]]->avaiableForEnpassant == true) {
					/*Not necessary update bool hasMoved (if it can capture en passant, has already moved)*/
					enPassantExploited = true;
					return 1; 
					}
				}
			}
		}


	}

	if (chessboard[move[0]][move[1]]->myPieceType == KING){
		if ((move[2] == move[0] | move[2] == move[0]+1 | move[2] == move[0]-1) & 
			(move[3] == move[1] | move[3] == move[1]+1 | move[3] == move[1]-1) ){
			/*If square is empty or moving there I would take an adversary's piece*/
			if (chessboard[move[2]][move[3]] == NULL){
				if (!isCheck(move, chessboard[move[0]][move[1]]->color)) return 1;
				else {
					printf("Illegal move: in that position it will be check\n");
					return 0;
				}
			}
			else if ( ((chessboard[move[2]][move[3]]->color == BLACK) & isPlayerTurn) |  
					((chessboard[move[2]][move[3]]->color == WHITE) & !isPlayerTurn) ) {
				if (!isCheck(move, chessboard[move[0]][move[1]]->color)) return 1;
				else {
					printf("Illegal move: in that position it will be check\n");
					return 0;
				}
			}	
		}
	}

	if (chessboard[move[0]][move[1]]->myPieceType == KNIGHT){
		if( ( (move[2]-move[0] == 1) & ((move[3]-move[1] == 2) | (move[3]-move[1] == -2)) ) |
			( (move[2]-move[0] == -1) & ((move[3]-move[1] == 2) | (move[3]-move[1] == -2)) ) |
			( (move[2]-move[0] == 2) & ((move[3]-move[1] == 1) | (move[3]-move[1] == -1)) ) |
			( (move[2]-move[0] == -2) & ((move[3]-move[1] == 1) | (move[3]-move[1] == -1)) ) ){
			/*If square is empty or moving there I would take an adversary's piece*/
			if (chessboard[move[2]][move[3]] == NULL){
				return 1;
			}
			else if ( ((chessboard[move[2]][move[3]]->color == BLACK) & isPlayerTurn) |  
					((chessboard[move[2]][move[3]]->color == WHITE) & !isPlayerTurn) ) {
				return 1;
			}	
		}
	}


	return 0;
};

/*The possibility to capture a pawn with en passant lasts only one turn*/
/*so I reset all the bool to false at the end  of the turn*/
void resetEnPassant(){
	if (isPlayerTurn){
		for (int i = 0; i<8; i++){
			/*if (blackPieces[i]->avaiableForEnpassant){
				printf("Black Pawn in %d %d was avaiableForEnpassant\n", blackPieces[i]->position[0], blackPieces[i]->position[1]);				
			}*/
			blackPieces[i]->avaiableForEnpassant = false;
		}
	}
	else {
		for (int i = 0; i<8; i++){
			/*if (whitePieces[i]->avaiableForEnpassant){
				printf("white Pawn in %d %d was avaiableForEnpassant\n", whitePieces[i]->position[0], whitePieces[i]->position[1]);				
			}*/
			whitePieces[i]->avaiableForEnpassant = false;
		}
	}
}

bool playerHasWon(){
	/*Seems antiintuitive, but in my turn i've to check if my adversary has won, before consider which move to choose*/
	if (!isPlayerTurn){
		for (int i = 0; i<8; i++) {
			if (chessboard[7][i] != NULL) {
				if ( (chessboard[7][i]->color == WHITE) & (chessboard[7][i]->myPieceType == PAWN) ){
					printf("Match won by Player\n");
					return 1;
				}
			}
		}
	}

	else {
		for (int i = 0; i<8; i++) {
			if (chessboard[0][i] != NULL) {
				if ( (chessboard[0][i]->color == BLACK) & (chessboard[0][i]->myPieceType == PAWN) ){
					printf("Match won by AI\n");
					return 1;
				}
			}
		}
	}
	return 0;
};

void generateRandomMove(){
	lastMove[0] = rand()%8;
	lastMove[1] = rand()%8;
	lastMove[2] = rand()%8;
	lastMove[3] = rand()%8;
};

char* encodeMove(){
	char* moveEncoded;
	moveEncoded = malloc(6*sizeof(char));
	switch (lastMove[1]){
		case 0: moveEncoded[0] = 'a'; break;
		case 1: moveEncoded[0] = 'b'; break;
		case 2: moveEncoded[0] = 'c'; break;
		case 3: moveEncoded[0] = 'd'; break;
		case 4: moveEncoded[0] = 'e'; break;
		case 5: moveEncoded[0] = 'f'; break;
		case 6: moveEncoded[0] = 'g'; break;
		case 7: moveEncoded[0] = 'h'; break;
	}
	switch (lastMove[3]){
		case 0: moveEncoded[3] = 'a'; break;
		case 1: moveEncoded[3] = 'b'; break;
		case 2: moveEncoded[3] = 'c'; break;
		case 3: moveEncoded[3] = 'd'; break;
		case 4: moveEncoded[3] = 'e'; break;
		case 5: moveEncoded[3] = 'f'; break;
		case 6: moveEncoded[3] = 'g'; break;
		case 7: moveEncoded[3] = 'h'; break;
	}
	moveEncoded[1] = '0' + lastMove[0]+1;
	moveEncoded[4] = '0' + lastMove[2]+1;
	moveEncoded[2] = '.';
	moveEncoded[5] = '\0';

	return moveEncoded;
}