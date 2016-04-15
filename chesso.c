#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "chesso.h"

int main()
{
	char nextMove[10];
	printf( "Welcome to Chesso, by Alessandro Cocilova \n");
	printf( "Please type your move in the format: StartcolStartrow.EndcolEndrow (es. a1.a2) \n\n\n");
	initChessboard();
	while (!playerHasWon()) { // senseless to pass the chessboard to this function, because there's only one chessboard
 	printChessboard();
 	printf( "Type your next move: ");
   	fgets(nextMove, 10, stdin);
   	move(nextMove);

 	//verifica validità mossa
 	//aggiorna scacchiera
 }

};

void initChessboard(){
	strcpy(row0, "  a b c d e f g h");
	strcpy(col0, "12345678");
	strcpy(matchChessboard.matrix[7], "-+-+k+h+");
	strcpy(matchChessboard.matrix[6], "pppppppp");
	strcpy(matchChessboard.matrix[5], "-+-+-+-+");
	strcpy(matchChessboard.matrix[4], "+-+-+-+-");
	strcpy(matchChessboard.matrix[3], "-+-+-+-+");
	strcpy(matchChessboard.matrix[2], "+-+-+-+-");
	strcpy(matchChessboard.matrix[1], "pppppppp");
	strcpy(matchChessboard.matrix[0], "+-+-k-h-");
};

void printChessboard(){

	for (int i = 7; i >= 0; i--){ // because row8 has to be printfed first
		printf("%c", col0[i]);
		for (int j = 0; j < 8; j++){
			printf(" %c", matchChessboard.matrix[i][j]);
		}
		printf("\n");
	}
	printf("%s\n", row0); 

};

void move(char playerMove[]){
	printf("%s\n", playerMove);

	if (moveIsLegal(playerMove)){
		updateChessboard();
	}
};

void updateChessboard(){
	//move pawn in a1 to a2
	//matchChessboard.row2[1] = "-"
	//matchChessboard.row3[1] = "p"
};

bool moveIsLegal(char playerMove[]){
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
	switch (playerMove[2]){
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
	endRow = playerMove[3] - '0' -1;
	printf("%d %d %d %d\n", startCol, startRow, endCol, endRow);
	return 1;
};

bool playerHasWon(){
	//temporary placeholder function
	return 0;
};