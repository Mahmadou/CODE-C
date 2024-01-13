#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1024

char message[MAX_MESSAGE_LENGTH];
int current_char = 0;

void handle_signal(int signal) {
    if (signal == SIGUSR1 || signal == SIGUSR2) {
        message[current_char] = (message[current_char] << 1) | (signal == SIGUSR2);
        if (++current_char >= MAX_MESSAGE_LENGTH || message[current_char - 1] == '\0') {
            char timestamp[64];
            time_t now = time(NULL);
            strftime(timestamp, sizeof(timestamp), "[%H:%M:%S - %d/%m/%Y]", localtime(&now));

            printf("%s : %s\n", timestamp, message);

            FILE *log_file = fopen("messages.logs", "a");
            if (log_file != NULL) {
                fprintf(log_file, "%s : %s\n", timestamp, message);
                fclose(log_file);
            }

            memset(message, 0, MAX_MESSAGE_LENGTH);
            current_char = 0;
        }
    }
}

int main() {
    printf("Server PID: %d\n", getpid());

    struct sigaction sa = { .sa_handler = &handle_signal };
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    while (1) {
        pause();
    }

    return 0;
}
