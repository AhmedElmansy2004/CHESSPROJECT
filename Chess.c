#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>

// for testing purposes
/*
wchar_t WHITEPAWNSHAPE[] = L"\033[34m1\033[0m";
wchar_t BLACKPAWNSHAPE[] = L"\033[31m1\033[0m";
wchar_t WHITEKNIGHTSHAPE[] = L"\033[34m&\033[0m";
wchar_t BLACKKNIGHTSHAPE[] = L"\033[31m&\033[0m";
wchar_t WHITEROOKSHAPE[] = L"\033[34m#\033[0m";
wchar_t BLACKROOKSHAPE[] = L"\033[31m#\033[0m";
wchar_t WHITEBISHOPSHAPE[] = L"\033[34mf\033[0m";
wchar_t BLACKBISHOPSHAPE[] = L"\033[31mf\033[0m";
wchar_t WHITEQUEENSHAPE[] = L"\033[34m*\033[0m";
wchar_t BLACKQUEENSHAPE[] = L"\033[31m*\033[0m";
wchar_t WHITEKINGSHAPE[] = L"\033[34mk\033[0m";
wchar_t BLACKKINGSHAPE[] = L"\033[31mk\033[0m";
*/

//struct to deal with 2D positions and its constructor
typedef struct{
    int x;
    int y;
}Position;

//The constructor for position struct
Position position(int x, int y){
  Position pos = {x, y};
  return pos;
}

// Chess unicode representation

wchar_t WHITEPAWNSHAPE[] = L"\033[34m♟\033[0m";
wchar_t BLACKPAWNSHAPE[] = L"\033[31m♟\033[0m";
wchar_t WHITEKNIGHTSHAPE[] = L"\033[34m♞\033[0m";
wchar_t BLACKKNIGHTSHAPE[] = L"\033[31m♞\033[0m";
wchar_t WHITEROOKSHAPE[] = L"\033[34m♜\033[0m";
wchar_t BLACKROOKSHAPE[] = L"\033[31m♜\033[0m";
wchar_t WHITEBISHOPSHAPE[] = L"\033[34m⚜\033[0m";
wchar_t BLACKBISHOPSHAPE[] = L"\033[31m⚜\033[0m";
wchar_t WHITEQUEENSHAPE[] = L"\033[34m♛\033[0m";
wchar_t BLACKQUEENSHAPE[] = L"\033[31m♛\033[0m";
wchar_t WHITEKINGSHAPE[] = L"\033[34m♚\033[0m";
wchar_t BLACKKINGSHAPE[] = L"\033[31m♚\033[0m";


// Enumeration types for piece type and color
typedef enum {PAWN = 0, KNIGHT = 1, ROOK = 2, BISHOP = 3, QUEEN = 4, KING = 5, EMPTY = 6}Type;
typedef enum {WHITE = 0, BLACK = 1, NONE = 2}Color;

// piece attributes
typedef struct{
    Type pieceType;
    Color pieceColor;
    wchar_t shape[32];
}Piece;

//piece constructor
Piece constructPiece(Type type, Color color){
  Piece piece = {type, color};
  switch(type){
    case PAWN:
      wcscpy(piece.shape , (color == WHITE) ? WHITEPAWNSHAPE : BLACKPAWNSHAPE);
      return piece;

    case KNIGHT:
      wcscpy(piece.shape , (color == WHITE) ? WHITEKNIGHTSHAPE : BLACKKNIGHTSHAPE);
      return piece;

    case ROOK:
      wcscpy(piece.shape , (color == WHITE) ? WHITEROOKSHAPE : BLACKROOKSHAPE);
      return piece;

    case BISHOP:
      wcscpy(piece.shape , (color == WHITE) ? WHITEBISHOPSHAPE : BLACKBISHOPSHAPE);
      return piece;

    case QUEEN:
      wcscpy(piece.shape , (color == WHITE) ? WHITEQUEENSHAPE : BLACKQUEENSHAPE);
      return piece;

    case KING:
      wcscpy(piece.shape , (color == WHITE) ? WHITEKINGSHAPE : BLACKKINGSHAPE);
      return piece;
    default:
      wcscpy(piece.shape, L" ");
      return piece;
  }
}

// Chess board declaration
Piece chessBoard[8][8];

//A visual to explain all controls
//void explainControls();

void intializeBoard(Piece pieces[32]);
void printBoard();

int getRow(char symbol);
int getCol(char symbol);

Position* getPawnPossibleMoves(int fromRow, int fromCol, Color playerColor, Position OpponentEPposition);
bool movePawn(int fromRow, int fromCol, int toRow, int toCol, Color playerColor, Position EPposition[], Position OpponentEPposition);
bool promotePawn(int fromRow, int fromCol, char promotion, Color playerColor);

Position* getKnightPossibleMoves(int fromRow, int fromCol, Color playerColor);
bool moveKnight(int fromRow, int fromCol, int toRow, int toCol, Color playerColor);

Position* getRookPossibleMoves(int fromRow, int fromCol, Color playerColor);
bool moveRook(int fromRow, int fromCol, int toRow, int toCol, Color playerColor);

Position* getBishopPossibleMoves(int fromRow, int fromCol, Color playerColor);
bool moveBishop(int fromRow, int fromCol, int toRow, int toCol, Color playerColor);

Position* getQueenPossibleMoves(int fromRow, int fromCol, Color playerColor);
bool moveQueen(int fromRow, int fromCol, int toRow, int toCol, Color playerColor);

Position* getKingPossibleMoves(int fromRow, int fromCol, Color playerColor);
bool moveKing(int fromRow, int fromCol, int toRow, int toCol, Color playerColor);

//void castle(char side);


Position playerTurn(Color playerColor, Position OpponentEPposition);

int main(){
  _setmode(_fileno(stdout), _O_U16TEXT);
  SetConsoleOutputCP(65001);
  setlocale(LC_ALL, "");  // Enable wide character output

  bool playAgain = true;
  while (playAgain)
  {
    //array to store all chess piece
    Piece pieces[32];
    //fill array with black pieces
    pieces[0] = constructPiece(ROOK, BLACK);
    pieces[1] = constructPiece(KNIGHT, BLACK);
    pieces[2] = constructPiece(BISHOP, BLACK);
    pieces[3] = constructPiece(QUEEN, BLACK);
    pieces[4] = constructPiece(KING, BLACK);
    pieces[5] = constructPiece(BISHOP, BLACK);
    pieces[6] = constructPiece(KNIGHT, BLACK);
    pieces[7] = constructPiece(ROOK, BLACK);
    for (int i = 8; i < 16; i++) {
      pieces[i] = constructPiece(PAWN, BLACK);
    }
    //fill array with white pieces
    for (int i = 16; i < 24; i++) {
      pieces[i] = constructPiece(PAWN, WHITE);
    }
    pieces[24] = constructPiece(ROOK, WHITE);
    pieces[25] = constructPiece(KNIGHT, WHITE);
    pieces[26] = constructPiece(BISHOP, WHITE);
    pieces[27] = constructPiece(QUEEN, WHITE);
    pieces[28] = constructPiece(KING, WHITE);
    pieces[29] = constructPiece(BISHOP, WHITE);
    pieces[30] = constructPiece(KNIGHT, WHITE);
    pieces[31] = constructPiece(ROOK, WHITE);
    // end of filling the array

    //explainControls();
    intializeBoard(pieces);

    int count = 0;
    Position EnPassant = {10, 10};

    while(1)
    {
      //white turn
      printBoard();
      EnPassant = playerTurn(WHITE, EnPassant);
      
      //black turn
      printBoard();
      EnPassant = playerTurn(BLACK, EnPassant);

      count++;
    }
    
    // if player wants to play again
    bool valid;
    do
    {
      valid = true;
      wprintf(L"Do you want to play again(y/n): ");
      char ans;
      int c;
      while ((c = getchar()) != '\n' && c != EOF);  //to empty th buffer
      
      scanf("%c", &ans);
      switch (tolower(ans))
      {
      case 'y':
        playAgain = true;
        break;
      case 'n':
        playAgain = false;
        break;
      default:
        valid = false;
        wprintf(L"\nTYPE A MEANINGFUL ANSWER!!!");
        break;
      }
    } while (!valid);
  }
}

void intializeBoard(Piece pieces[32]){
    //initially empty the entire board to avoid printed garbage values
    for(int i = 0; i < 8; i++) {
      for(int j = 0; j < 8; j++) {
        chessBoard[i][j] = constructPiece(EMPTY, NONE);
      }
    }
    //fill the board
    for(int i = 0; i < 8; i++) {
      chessBoard[0][i] = pieces[i];
      chessBoard[1][i] = pieces[i+8];
      chessBoard[6][i] = pieces[i+16];
      chessBoard[7][i] = pieces[i+24];
    }
}

void printBoard()
{
  wprintf(L"\t\t\t\t+---+---+---+---+---+---+---+---+---+---+");
  wprintf(L"\n\t\t\t\t| X | A | B | C | D | E | F | G | H | X |");
  for(int i = 0; i < 8; i++){
  	wprintf(L"\n\t\t\t\t|---|---|---|---|---|---|---|---|---|---|\n");
    wprintf(L"\t\t\t\t| %d | %ls | %ls | %ls | %ls | %ls | %ls | %ls | %ls | %d |", i+1, chessBoard[i][0].shape, chessBoard[i][1].shape, 
                                                                          chessBoard[i][2].shape, chessBoard[i][3].shape, chessBoard[i][4].shape, 
                                                                          chessBoard[i][5].shape, chessBoard[i][6].shape, chessBoard[i][7].shape, i+1);
  }
  wprintf(L"\n\t\t\t\t|---|---|---|---|---|---|---|---|---|---|\n");
  wprintf(L"\t\t\t\t| X | A | B | C | D | E | F | G | H | X |\n");
  wprintf(L"\t\t\t\t+---+---+---+---+---+---+---+---+---+---+\n");
}

int getCol(char symbol){
  switch (symbol)
  {
  case 'a':return 0;
  case 'b':return 1;
  case 'c':return 2;
  case 'd':return 3;
  case 'e':return 4;
  case 'f':return 5;
  case 'g':return 6;
  case 'h':return 7;
  default: return 10;
  }
}
int getRow(char symbol){
  switch (symbol)
  {
  case '1':return 0;
  case '2':return 1;
  case '3':return 2;
  case '4':return 3;
  case '5':return 4;
  case '6':return 5;
  case '7':return 6;
  case '8':return 7;
  default:return 10;
  }
}

Position playerTurn(Color playerColor, Position OpponentEPposition){
  char playerMove[10];

  int fromCol;
  int fromRow;
  int toCol;
  int toRow;

  Position EPposition[1];
  EPposition[0].x = 10;
  EPposition[0].y = 10;

  bool isPromoted = false;

  // Extract what the player wants to do in his turn
  while(1){
    wprintf(L"\nEnter Move: ");
    scanf("%s", &playerMove); //take player input

    if (playerMove[0] == '0') //check if player wants to castle
    {
      if (strcmp(playerMove, "0-0") == 0)  //kingside castling
      {
        //castle('k');
        return EPposition[0];
      }
      else if (strcmp(playerMove, "0-0-0") == 0)  //queenside castling
      {
        //castle('q');
        return EPposition[0];
      }
      else{
        wprintf(L"Wrong Expression, try again!");
        continue;
      }
    }
    
    fromCol = getCol(tolower(playerMove[0]));
    if(fromCol == 10){
      wprintf(L"Wrong Expression, try again!");
      continue;
    }
    fromRow = getRow(tolower(playerMove[1]));
    if(fromRow == 10){
      wprintf(L"Wrong Expression, try again!");
      continue;
    }

    if (chessBoard[fromRow][fromCol].pieceColor != playerColor) //check if player chose the right piece
    {
      wprintf(L"Choose a piece of your own, try again!");
      continue;
    }

    toCol = getCol(tolower(playerMove[2]));
    if(toCol == 10){
      wprintf(L"Wrong Expression, try again!");
      continue;
    }
    toRow = getRow(tolower(playerMove[3]));
    if(toRow == 10){
      wprintf(L"Wrong Expression, try again!");
      continue;
    }
    //check if there is a promotion
    if ((chessBoard[fromRow][fromCol].pieceType == PAWN) &&
       (toRow == 7 || toRow == 0))  
    {
      if (playerMove[4] == '=' && (tolower(playerMove[5]) == 'k' ||
                                   tolower(playerMove[5]) == 'b' ||
                                   tolower(playerMove[5]) == 'r' ||
                                   tolower(playerMove[5]) == 'q'    ))  //pawn promotion
      {
        isPromoted = true;
      }
      else
      {
        wprintf(L"Wrong Expression, try again!");
        continue;
      }
    }

    bool isValid = true;

    switch (chessBoard[fromRow][fromCol].pieceType)
    {
    case PAWN:
      //pawn promotion
      if (isPromoted)
      {
        isValid = promotePawn(fromRow, fromCol, tolower(playerMove[5]), playerColor);
      }
      if(isValid)
      {
        isValid = movePawn(fromRow, fromCol, toRow, toCol, playerColor, EPposition, OpponentEPposition);
      }
      break;
    case ROOK:
      isValid = moveRook(fromRow, fromCol, toRow, toCol, playerColor);
      break;
    case KNIGHT:
      isValid = moveKnight(fromRow, fromCol, toRow, toCol, playerColor);
      break;
    case BISHOP:
      isValid = moveBishop(fromRow, fromCol, toRow, toCol, playerColor);
      break;
    case QUEEN:
      isValid = moveQueen(fromRow, fromCol, toRow, toCol, playerColor);
      break;
    case KING:
      isValid = moveKing(fromRow, fromCol, toRow, toCol, playerColor);
      break;
    default:
      isValid = false;
      break;
    }
    
    if (!isValid)
    {
      wprintf(L"Move is NOT Valid, try again!");
      continue;
    }

    break;
  }
  return EPposition[0];
}

Position* getPawnPossibleMoves(int fromRow, int fromCol, Color playerColor, Position OpponentEPposition){
  Position* pawnPossibilities = (Position *)malloc(5+1 * sizeof(Position));
  int i = 0;
  switch (playerColor)
  {
  case WHITE:
    if (fromRow != 0 && chessBoard[fromRow-1][fromCol].pieceColor == NONE) //normal pawn move
    {
      pawnPossibilities[i].x = fromRow - 1;
      pawnPossibilities[i++].y = fromCol;
    }
    if (fromRow == 6 && chessBoard[fromRow-1][fromCol].pieceColor == NONE && chessBoard[fromRow-2][fromCol].pieceColor == NONE) //first pawn move
    {
      pawnPossibilities[i].x = fromRow - 2;
      pawnPossibilities[i++].y = fromCol;
    }
    if (chessBoard[fromRow-1][fromCol-1].pieceColor == BLACK)  //pawn attack from left
    {
      pawnPossibilities[i].x = fromRow - 1;
      pawnPossibilities[i++].y = fromCol - 1;
    }
    if (chessBoard[fromRow-1][fromCol+1].pieceColor == BLACK)  //pawn attack from right
    {
      pawnPossibilities[i].x = fromRow - 1;
      pawnPossibilities[i++].y = fromCol + 1;
    }
    //en passent code
    if ((fromRow == OpponentEPposition.x && fromCol+1 == OpponentEPposition.y) ||
        (fromRow == OpponentEPposition.x && fromCol-1 == OpponentEPposition.y))  
    {
      pawnPossibilities[i].x = OpponentEPposition.x - 1;
      pawnPossibilities[i++].y = OpponentEPposition.y;
    }
    
    break;

  case BLACK:
    if (fromRow != 7 && chessBoard[fromRow+1][fromCol].pieceColor == NONE) //normal pawn move
    {
      pawnPossibilities[i].x = fromRow + 1;
      pawnPossibilities[i++].y = fromCol;
    }
    if (fromRow == 1 && chessBoard[fromRow+1][fromCol].pieceColor == NONE && chessBoard[fromRow+2][fromCol].pieceColor == NONE) //first pawn move
    {
      pawnPossibilities[i].x = fromRow + 2;
      pawnPossibilities[i++].y = fromCol;
    }
    if (chessBoard[fromRow+1][fromCol-1].pieceColor == WHITE)  //pawn attack from left
    {
      pawnPossibilities[i].x = fromRow + 1;
      pawnPossibilities[i++].y = fromCol - 1;
    }
    if (chessBoard[fromRow+1][fromCol+1].pieceColor == WHITE)  //pawn attack from right
    {
      pawnPossibilities[i].x = fromRow + 1;
      pawnPossibilities[i++].y = fromCol + 1;
    }
    //en passent code
    if ((fromRow == OpponentEPposition.x && fromCol+1 == OpponentEPposition.y) ||
        (fromRow == OpponentEPposition.x && fromCol-1 == OpponentEPposition.y))
    {
      pawnPossibilities[i].x = OpponentEPposition.x + 1;
      pawnPossibilities[i++].y = OpponentEPposition.y;
    }

    break;
  
  default:
    break;
  }

  //null markers to ease looping over them
  pawnPossibilities[i].x = -1;
  pawnPossibilities[i++].y = -1;
  return pawnPossibilities;
}

bool movePawn(int fromRow, int fromCol, int toRow, int toCol, Color playerColor, Position EPposition[], Position OpponentEPposition){
  Position* possibleMoves = getPawnPossibleMoves(fromRow, fromCol, playerColor, OpponentEPposition);

  int i = 0;
  while(!(possibleMoves[i].x == -1 && possibleMoves[i].y == -1))
  {
    if (possibleMoves[i].x == toRow && possibleMoves[i].y == toCol)
    {
      if (chessBoard[toRow][toCol].pieceColor != NONE)  //pawn will capture(normal way)
      {
        chessBoard[toRow][toCol] = constructPiece(EMPTY, NONE);
        wprintf(L"\nPIECE CAPTURED\n");
      }
      if ((fromRow == OpponentEPposition.x && fromCol+1 == OpponentEPposition.y) ||
          (fromRow == OpponentEPposition.x && fromCol-1 == OpponentEPposition.y))     //pawn will capture(e.p.)
      {
        chessBoard[OpponentEPposition.x][OpponentEPposition.y] = constructPiece(EMPTY, NONE);
        wprintf(L"\nPIECE CAPTURED\n");
      }
      if (abs(fromRow - toRow) == 2)  //generate en passant
      {
        EPposition[0] = position(toRow, toCol);
      }
      else
      {
        EPposition[0] = position(10, 10);
      }

      Type pieceTypeNow = chessBoard[fromRow][fromCol].pieceType; //in case pawn was promoted
      
      chessBoard[fromRow][fromCol] = constructPiece(EMPTY, NONE);
      chessBoard[toRow][toCol] = constructPiece(pieceTypeNow, playerColor);
      
      free(possibleMoves);
      return true;
    }
    i++;
  }
  free(possibleMoves);
  return false;
}

bool promotePawn(int fromRow, int fromCol, char promotion, Color playerColor){

  //check if pawn is at its last rank
  switch (playerColor)
  {
  case WHITE:
    if (fromRow != 1) return false;
    break; 
  case BLACK:
    if (fromRow != 6) return false;
    break;
  }
  //check if promotion is valid and apply it
  switch (promotion)
  {
  case 'q':
    chessBoard[fromRow][fromCol] = constructPiece(QUEEN, playerColor);
    return true;
  case 'k':
    chessBoard[fromRow][fromCol] = constructPiece(KNIGHT, playerColor);
    return true;
  case 'r':
    chessBoard[fromRow][fromCol] = constructPiece(ROOK, playerColor);
    return true;
  case 'b':
    chessBoard[fromRow][fromCol] = constructPiece(BISHOP, playerColor);
    return true;
  default:
    return false;
  }  
}

Position* getKnightPossibleMoves(int fromRow, int fromCol, Color playerColor){
  Position* pawnPossibilities = (Position *)malloc(10 * sizeof(Position));
  int i = 0;

  Position posArray[8];
  posArray[0].x = fromRow+2; posArray[0].y = fromCol+1;
  posArray[1].x = fromRow+2; posArray[1].y = fromCol-1;
  posArray[2].x = fromRow-2; posArray[2].y = fromCol+1;
  posArray[3].x = fromRow-2; posArray[3].y = fromCol-1;
  posArray[4].x = fromRow+1; posArray[4].y = fromCol+2;
  posArray[5].x = fromRow-1; posArray[5].y = fromCol+2;
  posArray[6].x = fromRow+1; posArray[6].y = fromCol-2;
  posArray[7].x = fromRow-1; posArray[7].y = fromCol-2;

  int row, col;
  for (int j = 0; j < 8; j++)
  {
    row = posArray[j].x; col = posArray[j].y;
    if (chessBoard[row][col].pieceColor != playerColor && row >= 0 && row < 8 && col >= 0 && col < 8)
    {
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
  }
  
  //null markers to ease looping over them
  pawnPossibilities[i].x = -1;
  pawnPossibilities[i++].y = -1;
  return pawnPossibilities;
}
bool moveKnight(int fromRow, int fromCol, int toRow, int toCol, Color playerColor){
  Position* possibleMoves = getKnightPossibleMoves(fromRow, fromCol, playerColor);

  int i = 0;
  while(!(possibleMoves[i].x == -1 && possibleMoves[i].y == -1))
  {
    if (possibleMoves[i].x == toRow && possibleMoves[i].y == toCol)
    {
      if (chessBoard[toRow][toCol].pieceColor != NONE)  //knight will capture 
      {
        chessBoard[toRow][toCol] = constructPiece(EMPTY, NONE);
        wprintf(L"\nPIECE CAPTURED\n");
      }
      
      chessBoard[fromRow][fromCol] = constructPiece(EMPTY, NONE);
      chessBoard[toRow][toCol] = constructPiece(KNIGHT, playerColor);
      
      free(possibleMoves);
      return true;
    }
    i++;
  }
  free(possibleMoves);
  return false;
}

Position* getRookPossibleMoves(int fromRow, int fromCol, Color playerColor){
  Position* pawnPossibilities = (Position *)malloc(16 * sizeof(Position));
  int i = 0;
  Color opponentColor = (playerColor == WHITE)? BLACK : WHITE;

  int row = fromRow-1; int col = fromCol;
  while(row >= 0)  // check up
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    row--;
  }
  row = fromRow+1; col = fromCol;
  while(row <= 7)  // check down
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    row++;
  }
  row = fromRow; col = fromCol-1;
  while(col >= 0)  // check left
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    col--;
  }
  row = fromRow; col = fromCol+1;
  while(col <= 7)  // check right
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    col++;
  }
  
  //null markers to ease looping over them
  pawnPossibilities[i].x = -1;
  pawnPossibilities[i++].y = -1;
  return pawnPossibilities;
}

bool moveRook(int fromRow, int fromCol, int toRow, int toCol, Color playerColor){
  Position* possibleMoves = getRookPossibleMoves(fromRow, fromCol, playerColor);

  int i = 0;
  while(!(possibleMoves[i].x == -1 && possibleMoves[i].y == -1))
  {
    if (possibleMoves[i].x == toRow && possibleMoves[i].y == toCol)
    {
      if (chessBoard[toRow][toCol].pieceColor != NONE)  //rook will capture 
      {
        chessBoard[toRow][toCol] = constructPiece(EMPTY, NONE);
        wprintf(L"\nPIECE CAPTURED\n");
      }
      
      chessBoard[fromRow][fromCol] = constructPiece(EMPTY, NONE);
      chessBoard[toRow][toCol] = constructPiece(ROOK, playerColor);
      
      free(possibleMoves);
      return true;
    }
    i++;
  }
  free(possibleMoves);
  return false;
}

Position* getBishopPossibleMoves(int fromRow, int fromCol, Color playerColor){
  Position* pawnPossibilities = (Position *)malloc(16 * sizeof(Position));
  int i = 0;
  Color opponentColor = (playerColor == WHITE)? BLACK : WHITE;

  int row = fromRow-1; int col = fromCol+1; // diagonal top right
  while(row >= 0 && col <= 7)  
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    row--; col++;
  }
  row = fromRow-1; col = fromCol-1; // diagonal top left
  while(row >= 0 && col >= 0)  
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    row--; col--;
  }
  row = fromRow+1; col = fromCol+1;
  while(row <= 7 && col <= 7)  // diagonal bottom right
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    row++; col++;
  }
  row = fromRow+1; col = fromCol-1; // diagonal bottom left
  while(row <= 7 && col >= 0)
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    row++; col--;
  }
  
  //null markers to ease looping over them
  pawnPossibilities[i].x = -1;
  pawnPossibilities[i++].y = -1;
  return pawnPossibilities;
}
bool moveBishop(int fromRow, int fromCol, int toRow, int toCol, Color playerColor){
  Position* possibleMoves = getBishopPossibleMoves(fromRow, fromCol, playerColor);

  int i = 0;
  while(!(possibleMoves[i].x == -1 && possibleMoves[i].y == -1))
  {
    if (possibleMoves[i].x == toRow && possibleMoves[i].y == toCol)
    {
      if (chessBoard[toRow][toCol].pieceColor != NONE)  //bishop will capture 
      {
        chessBoard[toRow][toCol] = constructPiece(EMPTY, NONE);
        wprintf(L"\nPIECE CAPTURED\n");
      }
      
      chessBoard[fromRow][fromCol] = constructPiece(EMPTY, NONE);
      chessBoard[toRow][toCol] = constructPiece(BISHOP, playerColor);
      
      free(possibleMoves);
      return true;
    }
    i++;
  }
  free(possibleMoves);
  return false;
}
Position* getQueenPossibleMoves(int fromRow, int fromCol, Color playerColor){
  Position* pawnPossibilities = (Position *)malloc(30 * sizeof(Position));
  int i = 0;
  Color opponentColor = (playerColor == WHITE)? BLACK : WHITE;

  int row = fromRow-1; int col = fromCol;
  while(row >= 0)  // check up
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    row--;
  }
  row = fromRow+1; col = fromCol;
  while(row <= 7)  // check down
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    row++;
  }
  row = fromRow; col = fromCol-1;
  while(col >= 0)  // check left
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    col--;
  }
  row = fromRow; col = fromCol+1;
  while(col <= 7)  // check right
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    col++;
  }
  row = fromRow-1; col = fromCol+1; // diagonal top right
  while(row >= 0 && col <= 7)  
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    row--; col++;
  }
  row = fromRow-1; col = fromCol-1; // diagonal top left
  while(row >= 0 && col >= 0)  
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    row--; col--;
  }
  row = fromRow+1; col = fromCol+1;
  while(row <= 7 && col <= 7)  // diagonal bottom right
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    row++; col++;
  }
  row = fromRow+1; col = fromCol-1; // diagonal bottom left
  while(row <= 7 && col >= 0)
  {
    if (chessBoard[row][col].pieceColor == playerColor)
    {
      break;
    }
    else if(chessBoard[row][col].pieceColor == opponentColor){
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
      break;
    }
    else{
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
    row++; col--;
  }
  
  //null markers to ease looping over them
  pawnPossibilities[i].x = -1;
  pawnPossibilities[i++].y = -1;
  return pawnPossibilities;
}

bool moveQueen(int fromRow, int fromCol, int toRow, int toCol, Color playerColor){
  Position* possibleMoves = getQueenPossibleMoves(fromRow, fromCol, playerColor);

  int i = 0;
  while(!(possibleMoves[i].x == -1 && possibleMoves[i].y == -1))
  {
    if (possibleMoves[i].x == toRow && possibleMoves[i].y == toCol)
    {
      if (chessBoard[toRow][toCol].pieceColor != NONE)  //queen will capture 
      {
        chessBoard[toRow][toCol] = constructPiece(EMPTY, NONE);
        wprintf(L"\nPIECE CAPTURED\n");
      }
      
      chessBoard[fromRow][fromCol] = constructPiece(EMPTY, NONE);
      chessBoard[toRow][toCol] = constructPiece(QUEEN, playerColor);
      
      free(possibleMoves);
      return true;
    }
    i++;
  }
  free(possibleMoves);
  return false;
}

Position* getKingPossibleMoves(int fromRow, int fromCol, Color playerColor){
  Position* pawnPossibilities = (Position *)malloc(10 * sizeof(Position));
  int i = 0;

  Position posArray[8];
  posArray[0].x = fromRow-1; posArray[0].y = fromCol; //up
  posArray[1].x = fromRow+1; posArray[1].y = fromCol; //down
  posArray[2].x = fromRow; posArray[2].y = fromCol+1; //right
  posArray[3].x = fromRow; posArray[3].y = fromCol-1; //left
  posArray[4].x = fromRow-1; posArray[4].y = fromCol+1; //top right
  posArray[5].x = fromRow-1; posArray[5].y = fromCol-1; //top left
  posArray[6].x = fromRow+1; posArray[6].y = fromCol+1; //bottom right
  posArray[7].x = fromRow+1; posArray[7].y = fromCol-1; //bottom left

  int row, col;
  for (int j = 0; j < 8; j++)
  {
    row = posArray[j].x; col = posArray[j].y;
    if (chessBoard[row][col].pieceColor != playerColor && row >= 0 && row < 8 && col >= 0 && col < 8)
    {
      pawnPossibilities[i].x = row;
      pawnPossibilities[i++].y = col;
    }
  }
  
  //null markers to ease looping over them
  pawnPossibilities[i].x = -1;
  pawnPossibilities[i++].y = -1;
  return pawnPossibilities;
}
bool moveKing(int fromRow, int fromCol, int toRow, int toCol, Color playerColor){
  Position* possibleMoves = getKingPossibleMoves(fromRow, fromCol, playerColor);

  int i = 0;
  while(!(possibleMoves[i].x == -1 && possibleMoves[i].y == -1))
  {
    if (possibleMoves[i].x == toRow && possibleMoves[i].y == toCol)
    {
      if (chessBoard[toRow][toCol].pieceColor != NONE)  //king will capture 
      {
        chessBoard[toRow][toCol] = constructPiece(EMPTY, NONE);
        wprintf(L"\nPIECE CAPTURED\n");
      }
      
      chessBoard[fromRow][fromCol] = constructPiece(EMPTY, NONE);
      chessBoard[toRow][toCol] = constructPiece(KING, playerColor);
      
      free(possibleMoves);
      return true;
    }
    i++;
  }
  free(possibleMoves);
  return false;
}

//void castle(char side);

