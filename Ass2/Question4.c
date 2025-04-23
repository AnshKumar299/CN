#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // For Sleep()

#define TOTAL_FRAMES 10
#define WINDOW_SIZE 4
#define LOSS_PROBABILITY 0.2

int isLost() {
    return ((float)rand() / RAND_MAX) < LOSS_PROBABILITY;
}

void delay(int ms) {
    usleep(ms);
}

void selectiveRepeat_nonNACK() {
    printf("\n[1] Selective Repeat: Non-NACK Based\n");

    int acked[TOTAL_FRAMES] = {0};
    int base = 0;

    while (base < TOTAL_FRAMES) {
        for (int i = 0; i < WINDOW_SIZE && base + i < TOTAL_FRAMES; i++) {
            int frame = base + i;
            if (!acked[frame]) {
                printf("Sender: Sending Frame %d\n", frame);
            }
        }

        for (int i = 0; i < WINDOW_SIZE && base + i < TOTAL_FRAMES; i++) {
            int frame = base + i;
            delay(300);

            if (!acked[frame]) {
                if (!isLost()) {
                    printf("Receiver: Received Frame %d, Sending ACK\n", frame);
                    acked[frame] = 1;
                } else {
                    printf("Receiver: Frame %d lost or ACK lost\n", frame);
                }
            }
        }

        while (acked[base] && base < TOTAL_FRAMES) {
            base++;
        }
    }
}

void selectiveRepeat_nack() {
    printf("\n[2] Selective Repeat: NACK Based\n");

    int acked[TOTAL_FRAMES] = {0};
    int base = 0;

    while (base < TOTAL_FRAMES) {
        for (int i = 0; i < WINDOW_SIZE && base + i < TOTAL_FRAMES; i++) {
            int frame = base + i;
            if (!acked[frame]) {
                printf("Sender: Sending Frame %d\n", frame);
            }
        }

        for (int i = 0; i < WINDOW_SIZE && base + i < TOTAL_FRAMES; i++) {
            int frame = base + i;
            delay(300);

            if (!acked[frame]) {
                if (!isLost()) {
                    printf("Receiver: Received Frame %d, Sending ACK\n", frame);
                    acked[frame] = 1;
                } else {
                    printf("Receiver: Frame %d lost, Sending NACK\n", frame);
                    printf("Sender: Received NACK for Frame %d, will resend\n", frame);
                }
            }
        }

        while (acked[base] && base < TOTAL_FRAMES) {
            base++;
        }
    }
}

void selectiveRepeat_piggybacked() {
    printf("\n[3] Selective Repeat: Piggybacked Communication (Bi-directional)\n");

    int ackA[TOTAL_FRAMES] = {0}, ackB[TOTAL_FRAMES] = {0};
    int baseA = 0, baseB = 0;

    while (baseA < TOTAL_FRAMES || baseB < TOTAL_FRAMES) {
        printf("\n-- A to B --\n");
        for (int i = 0; i < WINDOW_SIZE && baseA + i < TOTAL_FRAMES; i++) {
            int frame = baseA + i;
            if (!ackA[frame]) {
                printf("A: Sending Frame %d", frame);
                if (baseB < TOTAL_FRAMES && !ackB[baseB]) {
                    printf(" [Piggybacked ACK for Frame %d]", baseB);
                    ackB[baseB] = 1;
                }
                printf("\n");
            }
        }

        for (int i = 0; i < WINDOW_SIZE && baseA + i < TOTAL_FRAMES; i++) {
            int frame = baseA + i;
            delay(300);

            if (!ackA[frame]) {
                if (!isLost()) {
                    printf("B: Received Frame %d from A, Sending ACK\n", frame);
                    ackA[frame] = 1;
                } else {
                    printf("B: Frame %d from A lost\n", frame);
                }
            }
        }

        printf("\n-- B to A --\n");
        for (int i = 0; i < WINDOW_SIZE && baseB + i < TOTAL_FRAMES; i++) {
            int frame = baseB + i;
            if (!ackB[frame]) {
                printf("B: Sending Frame %d", frame);
                if (baseA < TOTAL_FRAMES && !ackA[baseA]) {
                    printf(" [Piggybacked ACK for Frame %d]", baseA);
                    ackA[baseA] = 1;
                }
                printf("\n");
            }
        }

        for (int i = 0; i < WINDOW_SIZE && baseB + i < TOTAL_FRAMES; i++) {
            int frame = baseB + i;
            delay(300);

            if (!ackB[frame]) {
                if (!isLost()) {
                    printf("A: Received Frame %d from B, Sending ACK\n", frame);
                    ackB[frame] = 1;
                } else {
                    printf("A: Frame %d from B lost\n", frame);
                }
            }
        }

        while (ackA[baseA] && baseA < TOTAL_FRAMES) baseA++;
        while (ackB[baseB] && baseB < TOTAL_FRAMES) baseB++;
    }
}

int main() {
    srand(time(NULL));
    int choice;

    printf("Selective Repeat Protocol Simulation in C\n");
    printf("1. Non-NACK Based\n2. NACK Based\n3. Piggybacked (Bi-directional)\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: selectiveRepeat_nonNACK(); break;
        case 2: selectiveRepeat_nack(); break;
        case 3: selectiveRepeat_piggybacked(); break;
        default: printf("Invalid choice\n");
    }

    return 0;
}
