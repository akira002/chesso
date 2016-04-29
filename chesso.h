typedef enum {PAWN, KNIGHT, KING} pieceType;

typedef enum {BLACK, WHITE} pieceColor;

typedef struct {
	pieceType myPieceType;
	pieceColor color;
	int position[2]; //row, col. set to -1, -1 if piece has been eaten
	bool avaiableForEnpassant;
} piece;

piece* chessboard[8][8];

piece* whitePieces[10];
piece* blackPieces[10];

// more elegant integrate it into the struct?
bool whitePawnHasMoved[8];

bool blackPawnHasMoved[8];

bool enPassantExploited = false;

char row0[18]; 

char col0[9];

/*Global var, mantains the last move actually tried by the player that moved last*/
int lastMove[4];

bool isPlayerTurn = true; /*when false is AI's turn*/

void initChessboard();

void printChessboard();

bool playerHasWon();

void move(char playerMove[]);

int* decodePlayerMove(char playerMove[]);

bool isCheck(int move[], pieceColor myColor);

bool moveIsLegal(int move[]);

void resetEnPassant();

bool playerHasWon();