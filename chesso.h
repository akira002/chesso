struct Chessboard
{
	/*
	char row8[9]; //8 for each square of the line, 1 for the line number
	char row7[9]; //spaces in the print, not here -> separe model from view
	char row6[9];
	char row5[9];
	char row4[9];
	char row3[9];
	char row2[9];
	char row1[9];
	char row0[9]; //will remain always the same: a b c d ...*/
	char squareMatrix[9][9];
} chessboard;

chessboard matchChessboard; //malloc?

bool firstChessboardPrint = true;

//enum piece{knight, king, pawn};

void initChessboard();

void printChessboard();

bool playerHasWon();

void move(char playerMove[]);

void updateChessboard();

bool moveIsLegal(char playerMove[]);

bool playerHasWon();