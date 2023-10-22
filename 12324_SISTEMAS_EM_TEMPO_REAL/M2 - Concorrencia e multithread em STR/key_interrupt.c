#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

void customHandlerCrtlINT(int signum) {
    printf("Custom handler: Received signal Crtl-C\n");
    // Perform custom actions here
}

void customHandlerCrtlTSTP(int signum) {
    printf("Custom handler: Received signal Crtl-Z\n");
    // Perform custom actions here
}

int main() {

    // Set the customHandler function to handle SIGINT (Ctrl-C) or SIGTSTP (Ctrl-Z)
    signal(SIGINT, customHandlerCrtlINT);
    signal(SIGTSTP, customHandlerCrtlTSTP);
    

    printf("Press Ctrl-C or Z to trigger the custom handler...\n");

    while (1) {
        sleep(1);
    }

    return 0;
}