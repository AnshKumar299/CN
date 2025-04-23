#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TOTAL_FRAMES 20
#define MAX_SEQ 7  // Sequence numbers range from 0 to MAX_SEQ (modulo arithmetic)
#define MAX_BUFFER_SIZE (1 << (3 - 1))  // Let's assume m = 3, so buffer size = 2^(m-1) = 4

float LOSS_PROBABILITY = 0.1;

int isLost() {
    return ((float)rand() / RAND_MAX) < LOSS_PROBABILITY;
}

void delay(int ms) {
    usleep(ms);
}

typedef struct {
    int ack_no;
    int advertised_window;  // This field tells sender how many frames receiver can currently accept
} ACK;

int receiver_buffer[MAX_BUFFER_SIZE] = {0};
int buffer_occupancy = 0;

ACK receiveFrame(int frame) {
    ACK ack;

    if (!isLost()) {
        if (buffer_occupancy < MAX_BUFFER_SIZE) {
            printf("Receiver: Received Frame %d\n", frame);
            receiver_buffer[frame % MAX_BUFFER_SIZE] = 1;
            buffer_occupancy++;
        } else {
            printf("Receiver: Buffer full! Cannot accept Frame %d\n", frame);
        }
    } else {
        printf("Receiver: Frame %d lost\n", frame);
    }

    ack.ack_no = frame;
    ack.advertised_window = MAX_BUFFER_SIZE - buffer_occupancy;
    return ack;
}

void optimizedSlidingWindowProtocol() {
    int base = 0;
    int next_frame = 0;
    int total_sent = 0;
    int sender_window = MAX_BUFFER_SIZE;

    while (base < TOTAL_FRAMES) {
        int send_limit = base + sender_window;

        while (next_frame < send_limit && next_frame < TOTAL_FRAMES) {
            printf("Sender: Sending Frame %d\n", next_frame);
            delay(200);
            ACK ack = receiveFrame(next_frame);

            if (!isLost()) {
                if (ack.advertised_window > 0) {
                    printf("Sender: Received ACK for Frame %d | Receiver Window Available: %d\n", ack.ack_no, ack.advertised_window);
                } else {
                    printf("Sender: Received ACK for Frame %d | Receiver Buffer FULL!\n", ack.ack_no);
                }

                base++;
                next_frame++;
                sender_window = ack.advertised_window;

                // Simulate frame delivery at app layer to free buffer
                if (buffer_occupancy > 0 && rand() % 2 == 0) {
                    printf("Receiver: Delivered frame to app, freeing buffer.\n");
                    buffer_occupancy--;
                }

            } else {
                printf("Sender: ACK for Frame %d lost. Retrying...\n", next_frame);
                // Do not increment next_frame, resend
            }
        }

        if (sender_window == 0) {
            printf("Sender: Waiting... Receiver Buffer Full.\n");
            delay(500);  // simulate wait time
            sender_window = 1; // give another try
        }
    }
    printf("\nAll frames transmitted successfully!\n");
}

int main() {
    srand(time(NULL));
    printf("Optimized Sliding Window Protocol Simulation (With Buffer Feedback)\n");
    optimizedSlidingWindowProtocol();
    return 0;
}
