#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void send_char_as_signals(char c, pid_t server_pid) {
    for (int i = 7; i >= 0; --i) {
        kill(server_pid, (c >> i) & 1 ? SIGUSR2 : SIGUSR1);
        usleep(500); // 0.5ms
    }
}

int main(int argc, char *argv[]) {
    // Vérifier le nombre d'arguments
    if (argc != 3) {
        printf("Usage: %s <Server PID> <Message>\n", argv[0]);
        return 1;
    }

    // Récupérer le PID du serveur et le message à envoyer
    pid_t server_pid = atoi(argv[1]);
    char *message = argv[2];

    // Envoyer chaque caractère du message
    for (int i = 0; i < strlen(message); ++i) {
        send_char_as_signals(message[i], server_pid);
    }

    // Envoyer le caractère nul à la fin du message
    send_char_as_signals('\0', server_pid);

    return 0;
}
