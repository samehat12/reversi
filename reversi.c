//
// Author: Sameha Tasnim
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "reversi.h"
#define initializeBoard init_board
#define configureBoard  configure_board


void printBoard(char board[][26], int n) {

    /* print column headers */
    printf("  ");
    for (int i = 0; i < n; i++)
        printf("%c", 'a' + i);

    /* print the row with the board */
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("%c ", 'a' + i); // print row label
        for (int j = 0; j < n; j++)
            printf("%c", board[i][j]); // print cell state
        printf("\n");
    }
}

bool positionInBounds(int n, int row, int col) {
    return row >= 0 && row < n && col >= 0 && col < n;
}

bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol) {

    /* make first move in the given direction */
    int curr_row = row + deltaRow;
    int curr_col = col + deltaCol;

    bool found_path = false;
    
    bool valid_pos = positionInBounds(n, curr_row, curr_col);

    while (valid_pos) {

        /* get the colour of the current position on the board */
        char curr_pos = board[curr_row][curr_col];

        /* if nothing is along this direction, cannot play here */
        if (curr_pos == 'U')
            return false;
        
        /* if next position is opponent's tile, I found the beginning of a path */
        if (curr_pos != colour) {
            found_path = true;
            curr_row += deltaRow;
            curr_col += deltaCol;
        
        /* if next position has my tile, stop the search and return if I found a path */
        } else {
            return found_path;
        }

        valid_pos = positionInBounds(n, curr_row, curr_col);
    }

    return false;
}

void init_board(char board[][26], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            board[i][j] = 'U';
        }
    }
    
    board[n/2-1][n/2-1] = 'W';
    board[n/2][n/2-1] = 'B';
    board[n/2-1][n/2] = 'B';
    board[n/2][n/2] = 'W';
}

void configure_board(char board[][26], int n) {

    char input[4];

    printf("Enter board configuration:\n");
    while (1) {
        scanf("%s", input);
        if (strcmp(input, "!!!") == 0) break;
        
        char colour = input[0];
        int row = input[1] - 'a';
        int col = input[2] - 'a';
        
        board[row][col] = colour;
    }
}

/* this function gets the availble moves for a color, prints them and stores them for later */
void get_available_moves(char board[][26], int n, char colour) {

  printf("Available moves for %c:\n", colour);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        
      /* if someone already played here, it is not available*/
      if (board[i][j] != 'U'){
        continue;
      }
      
      bool legal_move = false;

      /* start looking from increasing rows */
      for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
          /* look at increasing columns per row */
          for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {

              /* skip if checking the same position */
              if (deltaRow == 0 && deltaCol == 0){
                continue;
              }

              legal_move = checkLegalInDirection(board, n, i, j, colour, deltaRow, deltaCol);

              if (legal_move){
                break;
              }
          }
          
          if (legal_move){
            break;
          }
      }
      
      if (legal_move) {
        printf("%c%c\n", i+'a', j+'a');
      }
    }
  }
}

void flip(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol) {

    /* get the position to start flipping */
    int curr_row = row + deltaRow;
    int curr_col = col + deltaCol;
    
    while (positionInBounds(n, curr_row, curr_col) && board[curr_row][curr_col] != colour) {
      
        board[curr_row][curr_col] = colour;

        /* move to next position in the given direction */
        curr_row += deltaRow;
        curr_col += deltaCol;
    }
}

/* check if valid move and if so, flip the tiles in a particular direction */
bool check_valid_move(char board[][26], int n, char move[]) {

  /* get the player */
  char player = move[0];

  /* get the position */
  int row = move[1] - 'a';
  int col = move[2] - 'a';
  
  bool on_board = positionInBounds(n, row, col);
  bool taken_tile = board[row][col] != 'U';

  /* check if the move in on the board or the tile already has a player on it */
  if (!on_board || taken_tile){
    return false;
  }

  bool legal_move = false;

  /* check for valid move and flips in all 8 directions */
  for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
      for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {

          /* skip if checking the same position */
          if (deltaRow == 0 && deltaCol == 0){
            continue;
          }

          if (checkLegalInDirection(board, n, row, col, player, deltaRow, deltaCol)) {
              legal_move = true;
              flip(board, n, row, col, player, deltaRow, deltaCol);
          }
      }
  }
  
  if (legal_move){
    board[row][col] = player;
  }

  return legal_move;
}

int main(void) {

  int n;
  char board[26][26];
  
  /* get the board dimensions */
  printf("Enter the board dimension: ");
  scanf("%d", &n);
  
  /* initialize the board with the starting pieces */
  initializeBoard(board, n);
  printBoard(board, n);
  
  /* configure the board at a certain game state */
  configureBoard(board, n);
  printBoard(board, n);
  
  /* get the available moves for white and black player */
  get_available_moves(board, n, 'W');
  get_available_moves(board, n, 'B');
  
  /* get a move from the player */
  char move[4];
  printf("Enter a move:\n");
  scanf("%s", move);

  bool valid_move = check_valid_move(board, n, move);
  
  if (valid_move) {
      printf("Valid move.\n");
  } else {
      printf("Invalid move.\n");
  }

  printBoard(board, n);

  return 0;
}
