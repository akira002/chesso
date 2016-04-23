char matchChessboard[8][9];

bool playerPawnHasMoved[8];
bool cpuPawnHasMoved[8];

//same here

char row0[18]; 

char col0[9];

int lastMove[4];

bool isPlayerTurn = true; /*when false is CPU's turn*/

//enum piece{knight, king, pawn};

void initChessboard();

void printChessboard();

bool playerHasWon();

void move(char playerMove[]);

void updateChessboard();

int* decodePlayerMove(char playerMove[]);

bool moveIsLegal(int playerMoveDecoded[]);

bool playerHasWon();