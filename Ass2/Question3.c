#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TOTAL_FRAMES 10
#define WINDOW_SIZE 4
#define LOSS_PROBABILITY 0.2

int isLost() {
    return ((float)rand() / RAND_MAX) < LOSS_PROBABILITY;
}

void delay(int ms) {
    usleep(ms * 1000);
}

void nonNACK_GBN() {
    printf("\n[1] Go-Back-N: Non-NACK Based Communication\n");
    int base = 0, nextSeq = 0;

    while (base < TOTAL_FRAMES) {
        while (nextSeq < base + WINDOW_SIZE && nextSeq < TOTAL_FRAMES) {
            printf("Sender: Sending Frame %d\n", nextSeq);
            nextSeq++;
        }

        for (int i = base; i < nextSeq; i++) {
            delay(500);
            if (!isLost()) {
                printf("Receiver: Received Frame %d, Sending ACK\n", i);
                base++;
            } else {
                printf("Receiver: Frame %d lost or ACK lost\n", i);
                printf("Sender: Timeout -> Resending from Frame %d\n", base);
                nextSeq = base;
                break;
            }
        }
    }
}

void nack_GBN() {
    printf("\n[2] Go-Back-N: NACK Based Communication\n");
    int base = 0, nextSeq = 0;

    while (base < TOTAL_FRAMES) {
        while (nextSeq < base + WINDOW_SIZE && nextSeq < TOTAL_FRAMES) {
            printf("Sender: Sending Frame %d\n", nextSeq);
            nextSeq++;
        }

        for (int i = base; i < nextSeq; i++) {
            delay(500);
            if (!isLost()) {
                printf("Receiver: Received Frame %d, Sending ACK\n", i);
                base++;
            } else {
                printf("Receiver: Frame %d lost, Sending NACK\n", i);
                printf("Sender: Received NACK -> Resending from Frame %d\n", i);
                base = i;
                nextSeq = base;
                break;
            }
        }
    }
}

void piggybacked_GBN() {
    printf("\n[3] Go-Back-N: Piggybacked Communication (Bi-directional)\n");
    int baseA = 0, nextA = 0;
    int baseB = 0, nextB = 0;

    while (baseA < TOTAL_FRAMES || baseB < TOTAL_FRAMES) {
        printf("\n-- A to B Transmission --\n");
        while (nextA < baseA + WINDOW_SIZE && nextA < TOTAL_FRAMES) {
            printf("A: Sending Frame %d", nextA);
            if (baseB < TOTAL_FRAMES) {
                printf(" [Piggybacked ACK for B's Frame %d]", baseB);
                baseB++;
            }
            printf("\n");
            nextA++;
        }

        for (int i = baseA; i < nextA; i++) {
            delay(500);
            if (!isLost()) {
                printf("B: Received Frame %d from A, Sending ACK\n", i);
                baseA++;
            } else {
                printf("B: Frame %d from A lost\n", i);
                nextA = baseA;
                break;
            }
        }

        printf("\n-- B to A Transmission --\n");
        while (nextB < baseB + WINDOW_SIZE && nextB < TOTAL_FRAMES) {
            printf("B: Sending Frame %d", nextB);
            if (baseA < TOTAL_FRAMES) {
                printf(" [Piggybacked ACK for A's Frame %d]", baseA);
                baseA++;
            }
            printf("\n");
            nextB++;
        }

        for (int i = baseB; i < nextB; i++) {
            delay(500);
            if (!isLost()) {
                printf("A: Received Frame %d from B, Sending ACK\n", i);
                baseB++;
            } else {
                printf("A: Frame %d from B lost\n", i);
                nextB = baseB;
                break;
            }
        }
    }
}

int main() {
    srand(time(NULL));
    int choice;

    printf("Go-Back-N Protocol Simulation in C\n");
    printf("1. Non-NACK Based\n2. NACK Based\n3. Piggybacked (Bi-directional)\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            nonNACK_GBN();
            break;
        case 2:
            nack_GBN();
            break;
        case 3:
            piggybacked_GBN();
            break;
        default:
            printf("Invalid choice!\n");
    }

    return 0;
}
