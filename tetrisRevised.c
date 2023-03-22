#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>


enum direction{up, down, left, right, rotate};

size_t rotateTetrimeno(size_t x, size_t y, size_t r);
int getInput();
void printTetrimeno(size_t tr, size_t tc, char tetrimeno[tr][tc]);
bool drawBoard(size_t br, size_t bc, char board[br][bc]);
bool checkCollision(size_t tr, size_t tc, size_t br, size_t bc,
                    int input, char tetrimeno[tr][tc], char board[br][bc]);
void updateBoard(size_t tr, size_t tc, size_t br, size_t bc,
        char tetrimeno[tr][tc], char board[br][bc]);
void checkLines(size_t br, size_t bc, char board[br][bc]);
void clearLine(size_t br, size_t bc, char board[br][bc], size_t j);

//position of the falling tetrimeno
size_t t_xy[2] = {0};

int score = {0};

// w value continues the game or exits
bool w = true;

int main(void)
{

    initscr();
    raw();
//    timeout(500);
    curs_set(0);

    size_t tr = 7; // number of rows for the double array that
                   // holds the tetrimenos

    size_t tc = 17; // number of columns for the double array that
                    // holds the tetrimenos

    char tetrimeno[tr][tc]; // this double array holds the tetrimenos

    size_t rw = sizeof(char)*5; // rw is the number of bytes to write when creating the tetrimenos

  
  //creating the tetrimenos
    tetrimeno[0][tc-1] = '\0';
    strncpy(tetrimeno[0],"    ", rw);
    strncat(tetrimeno[0],"XX  ",rw);
    strncat(tetrimeno[0]," XX ",rw);
    strncat(tetrimeno[0],"    ",rw);

    tetrimeno[1][tc-1] = '\0';
    strncpy(tetrimeno[1],"    ",rw);
    strncat(tetrimeno[1]," AA ",rw);
    strncat(tetrimeno[1],"AA  ",rw);
    strncat(tetrimeno[1],"    ",rw);

    tetrimeno[2][tc-1] = '\0';
    strncpy(tetrimeno[2],"    ",rw);
    strncat(tetrimeno[2],"BB  ",rw);
    strncat(tetrimeno[2],"BB  ",rw);
    strncat(tetrimeno[2],"    ",rw);

    tetrimeno[3][tc-1] = '\0';
    strncpy(tetrimeno[3],"    ",rw);
    strncat(tetrimeno[3],"E   ",rw);
    strncat(tetrimeno[3],"E   ",rw);
    strncat(tetrimeno[3],"EE  ",rw);

    tetrimeno[4][tc-1] = '\0';
    strncpy(tetrimeno[4],"    ",rw);
    strncat(tetrimeno[4]," F  ",rw);
    strncat(tetrimeno[4]," F  ",rw);
    strncat(tetrimeno[4],"FF  ",rw);

    tetrimeno[5][tc-1] = '\0';
    strncpy(tetrimeno[5],"    ",rw);
    strncat(tetrimeno[5]," G  ",rw);
    strncat(tetrimeno[5],"GGG ",rw);
    strncat(tetrimeno[5],"    ",rw);

    tetrimeno[6][tc-1] = '\0';
    strncpy(tetrimeno[6],"J   ",rw);
    strncat(tetrimeno[6],"J   ",rw);
    strncat(tetrimeno[6],"J   ",rw);
    strncat(tetrimeno[6],"J   ",rw);

   
    size_t br = 19; // the number of rows for the array that holds
                    // the board
    size_t bc = 12; // the number of columns for the array that holds
                    // the board
                    
    char board[br][bc]; 

    //initializing the board
    for (size_t i = 0;i < br;++i){
     for (size_t j = 0;j < bc;++j){
      if (j == 0 || j == bc-1 || i == br-1){ 
        board[i][j] = '#';
      } else{
        board[i][j] = ' ';
      }
     }
    }

    //initializing the position of the tetrimeno
    t_xy[0] = 5;
    t_xy[1] = 0;

    int input = 0; // input holds the input from stdin
    bool collide = false; // collide value determines whether
                          // the tetrimeno is colliding with
                          // another object in the board

    //start of the tetris game
    while (w){
     input = getInput();
     collide = checkCollision(tr,tc,br,bc,input,tetrimeno,board); 
     if (input == down && collide){
       updateBoard(tr,tc,br,bc,tetrimeno,board);
       collide = false;
     }
     clear();
     printTetrimeno(tr,tc,tetrimeno);
     w = drawBoard(br,bc,board);
     checkLines(br,bc,board);
     refresh(); 
    }
    endwin();
    return EXIT_SUCCESS;
}


//this function rotates the tetrimeno, returning the corresponding index
size_t rotateTetrimeno(size_t x, size_t y, size_t r)
{
    size_t i = 0;

    //rotate 0 degrees
    if (r == 0){
     i = (y*4) + x;
    } else if (r == 1){ //rotate 90 degrees
     i = (12+y) - (4*x);
    } else if (r == 2){ //rotate 180 degrees
     i = (15 - (4*y)) - x;
    } else if (r == 3){ //rotate 270 degrees
     i = (3-y) + (4*x);
    }

    return i;
}

//initializing the rotation parameters and tetrimeno parameter
size_t nrotation = 0; // nrotation represents the current
                      // orientation of the tetrimeno

size_t ntetrimeno = 0; // ntetrimeno represents the current tetrimeno
                       // this is the row number in the tetrimeno
                       // double array

//this function gets input from the user
int getInput()
{
    
    bool correctInput = true;
    int input = 0;

    keypad(stdscr,true);
    while (correctInput){

     timeout(500);
     input = getch();
    
     if (input < 0 || input == KEY_DOWN){
      input = down;
      correctInput = false;
     } else if (input == 'r'){
        input = rotate; 
        correctInput = false;
     } else if (input == KEY_LEFT){
        input = left;
        correctInput = false;
     } else if (input == KEY_RIGHT){
        input = right;
        correctInput = false;
     } else if (input =='q'){
        input = 'q';
        correctInput = false;
     } else
        continue;
    } 
    
    return input;
}

//this function prints the falling tetrimeno
void printTetrimeno(size_t tr, size_t tc, char tetrimeno[tr][tc])
{
    size_t r = 0;
    for (size_t y = 0;y < 4;++y){
     for (size_t x = 0;x <4;++x){
      r = rotateTetrimeno(x,y,nrotation);
      if (ntetrimeno < tr && r < tc-1){
       if (tetrimeno[ntetrimeno][r] != ' '){
        mvaddch(t_xy[1]+y,t_xy[0]+x+35,tetrimeno[ntetrimeno][r]);
       }
      } 
     }
    }
}

//this function prints the board and checks if the tetrimeno is above the board
bool drawBoard(size_t br, size_t bc, char board[br][bc])
{

    int x = 19;
    int y = 0;

    for (size_t j = 0;j < br;++j){
     for (size_t i = 0;i < bc;++i){
      if (board[j][i] != ' '){
       mvaddch(j,i+35,board[j][i]);
      }
     }
    }

    move(y,x);
    refresh();
    printw("Score: %d",score);

    for (size_t i = 0;i < bc;++i){
     if ((board[1][i] != ' ' && board[1][i] != '#') || (w == false)){
          return false;
     }
    }
     
    return true;
}

//this function checks for collisions
bool checkCollision(size_t tr, size_t tc, size_t br, size_t bc, int input,
                   char tetrimeno[tr][tc], char board[br][bc])
{
    size_t temp = 0;

    // update the position of the falling tetrimeno
    if (input == 'q'){
     w = false;
     return true;
    } else if (input == right){
       temp = t_xy[0];
       ++t_xy[0];
    } else if (input == left){
       temp = t_xy[0];
       --t_xy[0];
    } else if (input == down){
       temp = t_xy[1];
       ++t_xy[1];
    } else if (input == rotate){
       temp = nrotation;
       ++nrotation;
       if (nrotation > 3)
         nrotation = 0;
    }

    bool collide = false;
    size_t r = 0;

    for (size_t y = 0;y < 4;++y){
        if (collide == true)
            break;
     for (size_t x = 0;x < 4;++x){
      r = rotateTetrimeno(x,y,nrotation);
      if (ntetrimeno < tr && r < tc && t_xy[1] + y < br && t_xy[0] + x < bc)
       if (tetrimeno[ntetrimeno][r] != ' ' && board[t_xy[1]+y][t_xy[0]+x] != ' '){
          collide = true; 
          break;
       }
     }
    }  
   
   if (collide){
    if (input == rotate){
     nrotation = temp;
    } else if (input == right || input == left){
       t_xy[0] = temp;
    } else if (input == down){
       t_xy[1] = temp;
    }
   }
   return collide;

}

//this function updates the board with the falling tetrimeno
void updateBoard(size_t tr, size_t tc, size_t br, size_t bc,
                char tetrimeno[tr][tc], char board[br][bc])
{
    size_t r = 0;
    for (size_t j = 0;j < 4;++j){
     for (size_t i = 0;i < 4;++i){
      r = rotateTetrimeno(i,j,nrotation);
      if (r < tc  && ntetrimeno < tr && j+t_xy[1] < br && i+t_xy[0] < bc)
       if (tetrimeno[ntetrimeno][r] != ' '){
        board[j+t_xy[1]][i+t_xy[0]] = tetrimeno[ntetrimeno][r];
      }    
     }
    }
    
    srand(time(0));
    t_xy[0] = 5;
    t_xy[1] = 0;
    ntetrimeno = rand() % 7;
}

//this function checks for lines
void checkLines(size_t br, size_t bc, char board[br][bc])
{
    size_t count = 0;
    size_t tetriscount = 0;

    for (size_t j = 0;j < br;++j){
     count = 0;
     for (size_t i = 0;i < bc;++i){
      if (board[j][i] != ' ' && board[j][i] != '#'){
        ++count;
      }
      if (count == 10){
        clearLine(br,bc,board,j);
        ++tetriscount;
        count = 0;
        score += 100;
        move(5,19); 
        printw("scored!!!\n");
        refresh();
        
      }
     }
     if (tetriscount >= 4){
      move(5,19);
      printw("Tetris\n");
      refresh();
      score += 400;
     }
    }
}




//this function clears the line and updates the board
void clearLine(size_t br, size_t bc, char board[br][bc], size_t j)
{

    for (size_t i = 0;i < bc;++i){
     if (board[j][i] != '#'){
       board[j][i] = ' ';
     }
    }

    for (;j > 0;--j){
     for (size_t i = 1;i < 11;++i){
      board[j][i] = board[j-1][i];
     }
    }


}
      

      








