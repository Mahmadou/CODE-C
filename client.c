#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define DELAY 500

// Fonction pour envoyer un bit sous forme de signal.
void send_bit_signal(pid_t server_pid, int bit_signal) {
    if (kill(server_pid, bit_signal) == -1) {
        perror("Erreur lors de l'envoi du signal");
        exit(EXIT_FAILURE);
    }
    usleep(DELAY);
}

// Fonction pour envoyer un caractère au serveur en séquence de signaux.
void send_char_as_signals(char character, pid_t server_pid) {
    for (int i = 7; i >= 0; --i) {
        int bit = (character >> i) & 1;
        send_bit_signal(server_pid, bit ? SIGUSR2 : SIGUSR1);
    }
}

// Fonction principale du client
int main(int argc, char *argv[]) {
    if (argc != 3 || atoi(argv[1]) <= 0) {
        fprintf(stderr, "Usage: %s <Server PID> <Message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t server_pid = atoi(argv[1]);

    for (int i = 0; i < strlen(argv[2]); ++i) {
        send_char_as_signals(argv[2][i], server_pid);
    }

    return 0;
}
