#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

volatile sig_atomic_t received_signal = 0;

// Gestionnaire de signal pour le serveur
void signal_handler(int signum) {
    if (signum == SIGUSR1 || signum == SIGUSR2) {
        received_signal = signum;
    }
}

// Fonction principale du serveur
int main() {
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);

    printf("Serveur en attente de signaux...\n");

    while (1) {
        pause();

        if (received_signal == SIGUSR1) {
            printf("0");
        } else if (received_signal == SIGUSR2) {
            printf("1");
        }
        
        received_signal = 0;
    }

    return 0;
}
