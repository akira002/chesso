struct Chessboard
{
	char matrix[8][9]; //one char more than the one i've counted, for '/0', TERMINATOR CHAR
};

struct Chessboard matchChessboard;

//same here

char row0[18]; 

char col0[9];

//enum piece{knight, king, pawn};

void initChessboard();

void printChessboard();

bool playerHasWon();

void move(char playerMove[]);

void updateChessboard();

bool moveIsLegal(char playerMove[]);

bool playerHasWon();