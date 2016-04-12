#include <stdio.h>
#include "chesso.h"

int main()
{
	char nextMove[10];
	initChessboard();
	printf( "Welcome to Chesso, by Alessandro Cocilova \n");
	printf( "Please type your move in the format: StartcolStartrow.EndcolEndrow (es. a1.b1) \n\n\n");
	while (!playerHasWon()) { // senseless to pass the chessboard to this function, because there's only one chessboard
 	printChessboard();
 	printf( "Type your next move: ");
   	gets(nextMove);
   	move(nextMove);

 	//verifica validità mossa
 	//aggiorna scacchiera
 }

};

//STRUCT CHANGHED ===> TO BE CHANGED 
void initChessboard(){
	strcpy(matchChessboard.row8, "8-+-+k+h+");
	strcpy(matchChessboard.row7, "7pppppppp");
	strcpy(matchChessboard.row6, "6-+-+-+-+");
	strcpy(matchChessboard.row5, "5+-+-+-+-");
	strcpy(matchChessboard.row4, "4-+-+-+-+");
	strcpy(matchChessboard.row3, "3+-+-+-+-");
	strcpy(matchChessboard.row2, "2pppppppp");
	strcpy(matchChessboard.row1, "1+-+-k-h-");
	strcpy(matchChessboard.row0, " abcdefgh");
};

void printChessboard(){
	//se non è prima invocazione, cancella vecchia scacchiera da stdout
	if (!firstChessboardPrint){
		//remove from stdout 8+1+8+1 (squares/pieces, linenum, spaces between them, final \n)
		//do this procedure 9 times (one per row)
	}
	else {
		firstChessboardPrint = false;
	}

	for (i = 1; i < 9; i++){
		for (j = 1; i < 9); i++){
			print(" %s"matrixChessboard[i][j]);
		}
		print("\n");
	} 

};

void move(char playerMove[]){
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
	//temporary placeholder function
	return 1;
};

bool playerHasWon(){
	//temporary placeholder function
	return 0;
};