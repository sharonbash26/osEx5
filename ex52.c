//sharon bash 205698152
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define VERTICAL 1
#define HOZRITANL 0
#define SIZEOFBOARD (20)
#define GAMERUN 1
#define GAMESTOP 0

void initTheGame();

typedef struct point {
    int row;
    int col;
} point;
typedef struct object {
    point p1;//line -kv
    point p2;
    point p3;
    int status;

} object;
typedef struct Game {
    int running;// if the game is run
    char board[SIZEOFBOARD][SIZEOFBOARD];
    object minus;

} Game;

Game g;// create on stack

void printBoard();

void caseW() {
    if (g.minus.status ==HOZRITANL && (0 < g.minus.p2.row<=SIZEOFBOARD-2)) {
        // if cell1 is the left one
        if (g.minus.p1.col < g.minus.p3.col) {
            g.minus.p1.row -= 1;
            g.minus.p1.col += 1;
            g.minus.p3.row += 1;
            g.minus.p3.col -= 1;
        }
        else {
            g.minus.p1.row += 1;
            g.minus.p1.col -= 1;
            g.minus.p3.row -= 1;
            g.minus.p3.col += 1;
        }
    }
    if (g.minus.status == VERTICAL && (1 < g.minus.p2.col <= SIZEOFBOARD - 2)) {
        if (g.minus.p1.row < g.minus.p3.row) {
            g.minus.p1.row+=1;
            g.minus.p1.col+=1;
            g.minus.p3.row-=1;
            g.minus.p3.col-=1;
        }
        else {
            g.minus.p1.row-=1;
            g.minus.p1.col-=1;
            g.minus.p3.row+=1;
            g.minus.p3.col+=1;
        }
    }
}


void caseS(){
    if((g.minus.p1.row==18)||(g.minus.p3.row==18)){
        return;
    } else{
        ++(g.minus.p1.row);
        ++(g.minus.p2.row);
        ++(g.minus.p3.row);
    }
}
void caseD(){
    if((g.minus.p1.col==18)||(g.minus.p3.col==18)){
      return; 
    }else{
        ++(g.minus.p1.col);
        ++(g.minus.p2.col);
        ++(g.minus.p3.col);
    }
}
void caseA(){
    if (g.minus.p1.col == 1||(g.minus.p3.col==1)) {
        return;
    } else {
        --(g.minus.p1.col);
        --(g.minus.p2.col);
        --(g.minus.p3.col);
    }
}
//do when he recive letter and do task
void reciveSignal(int reSignal) {
   signal(SIGUSR2,reciveSignal);
    char output=(char)getchar();//OUTPUT FROM PIPE   
    switch (output) {
        case 'w':
             caseW();

               break;
        case 'q':
            g.running=GAMESTOP;

            break;
        case 's':
               caseS();

                break;
            //a-left
        case 'a':
             caseA();
             break;

        case 'd':
            caseD();

            break;
        default:
            break;

    }
    printBoard();

}

void printBoard() {
    int i, j;
    system("clear");//delte all
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 20; j++) {
            if ((i == g.minus.p1.row && j == g.minus.p1.col) ||
                (i == g.minus.p2.row && j == g.minus.p2.col) ||
                (i == g.minus.p3.row && j == g.minus.p3.col)) {
                printf("-");
            } else {
                printf("%c", g.board[i][j]);
            }
        }
        printf("\n");
    }


}

//just down object
void downObject() {
    if (g.minus.p1.row == 18 || g.minus.p3.row == 18) {
        initTheGame();// strat the game from start
    } else {
        g.minus.p1.row++;
        g.minus.p2.row++;
        g.minus.p3.row++;
    }
}

// every second do down and print to screen the board and object
void doAllSecond() {
    downObject();
    printBoard();
    alarm(1);

}

void initObject() {
    g.minus.status = HOZRITANL;
    g.minus.p1.row = 0;
    g.minus.p1.col = 10;
    g.minus.p2.row = 0;
    g.minus.p2.col = 11;
    g.minus.p3.row = 0;
    g.minus.p3.col = 12;
}

void initTheGame() {
    int i = 0;
    int j = 0;
    g.running = GAMERUN; // the game is running
    for (i = 0; i < SIZEOFBOARD; i++) {
        for (j = 0; j < SIZEOFBOARD; j++) {
            if (j == 0 || j == 19 || i == 19) {
                g.board[i][j] = '*';
            } else {
                g.board[i][j] = ' ';
            }
        }
    }
    initObject();

}

int main() {
    signal(SIGUSR2, reciveSignal);// when ex52 recieeve signal SIGURE2 he does the function rceviesignal
    signal(SIGALRM, doAllSecond); // ...
    initTheGame();
    alarm(1);// to dend the signal all 1 second
    while (g.running == GAMERUN) {}
    close(0);
}