char matchChessboard[8][9];

typedef enum {PAWN, KNIGHT, KING} pieceType;

typedef struct {
	pieceType myPieceType;
	int position[2]; //row, col. set to -1, -1 if piece has been eaten
	bool avaiableForEnpassant;
} piece;

piece gamePieces[20];

// not scalable
bool playerPawnHasMoved[8];

bool cpuPawnHasMoved[8];


char row0[18]; 

char col0[9];

int lastMove[4];

bool isPlayerTurn = true; /*when false is CPU's turn*/

void initChessboard();

void initPieces();

void printChessboard();

bool playerHasWon();

void move(char playerMove[]);

void updateChessboard();

int* decodePlayerMove(char playerMove[]);

bool moveIsLegal(int playerMoveDecoded[]);

bool playerHasWon();