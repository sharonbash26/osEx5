//sharon bash 205698152
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <wait.h>

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

void systemprintError() {
    const char *err = "Error in system call\n";
    size_t len = strlen(err);
    ssize_t res = write(STDERR_FILENO, err, len * sizeof(char));
    if (-1 == res) {
        //try to write again and finsih
        write(STDERR_FILENO, err, len * sizeof(char));
    }
}

void handle_child(int fd) {
    char *args[] = {"./draw.out", NULL};
    dup2(fd, 0);  // son just read w r d  we read instnce fd =0 to fd of pipe
   if(execvp(args[0], args)<0){
       systemprintError();
   }
    
    dup2(0, fd); // we change to be noramll
}

int main() {
    pid_t pid;
    char input;
    int arrForPipe[2]; //here 2 file discprtor
    if (0 > pipe(arrForPipe)) {
        systemprintError();
    }// in arg[0] read ...

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Error in system call");
        exit(1);
    }
    if (pid == 0) {
        //divide the parts of child and parent
        handle_child(arrForPipe[0]);
       if(close(arrForPipe[0])<0){
         systemprintError();
       }
       
    } else {
        //parent

        do {
            input = getch();
            if ((input != 'w') && (input != 'a') && (input != 'q') && (input != 'd') && (input != 's')) {
                continue;
            } else {
                if (write(arrForPipe[1], &input, sizeof(char)) < 0) {////1-write to pipe ,input from wherer e read and need to send ginal that we end to write letter to pipe
                    systemprintError();
                 if(kill(pid, SIGKILL)<0){
                      systemprintError();
                 } //pd OF SON TO KILL HIM,SIGKILL THIS IS SIGNAL SAY TO KILL
                    waitpid(pid, NULL, 0); //WE WAIT THAT SON FINISHED
                    exit(1);
                }

              if(kill(pid,SIGUSR2)<0){
               systemprintError();
              }  //we want to say to son that we have to him new leeter for him and say to him signal that say that kill just send signal
                //in kodm this was instanc 0 pid
            }

        } while (input != 'q');
        waitpid(pid, NULL, 0); // AGAIN WAIT TO THIS SON

     if(close(arrForPipe[1])<0){
       systemprintError();
    }

}
}