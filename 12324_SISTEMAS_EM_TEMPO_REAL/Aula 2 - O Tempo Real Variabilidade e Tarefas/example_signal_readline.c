//install readline on linux: sudo apt-get install libreadline-dev 
// to compile: gcc -o example_sr example_signal_readline.c -lreadline

#include <setjmp.h>
#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

int keyPressed(int count, int key) {

        printf("key pressed: %d\n",key);
        rl_on_new_line();
        return 0;
}

int main() {
        rl_catch_signals = 0;
        rl_bind_keyseq("\\C-g",keyPressed);
        rl_bind_keyseq("\\p",keyPressed);
        rl_bind_keyseq("\\z",keyPressed);

        while(1) {
                char *line = readline("rl> ");
        }
}