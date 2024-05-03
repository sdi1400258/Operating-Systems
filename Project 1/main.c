#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>




#include "sembuf.h"
#include "msg.h"

#define MAX_MESSAGE_LENGTH 500
#define MAX_HASH_SIZE 33
#define SHMSIZE (MAX_MESSAGE_LENGTH + MAX_HASH_SIZE)
#define TOTAL_SEMAPHORES 16

// [ 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 ]
// [ E F E F E F E F E F E F E F E F ]
// [ 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 ]

int P1_ENC1_SHMID, ENC1_CHA_SHMID, CHA_ENC2_SHMID, ENC2_P2_SHMID;
int P2_ENC2_SHMID, ENC2_CHA_SHMID, CHA_ENC1_SHMID, ENC1_P1_SHMID;

int P1_ENC1_KEY = 159357, ENC1_CHA_KEY = 246800, CHA_ENC2_KEY = 951753, ENC2_P2_KEY = 864200;
int P2_ENC2_KEY = 789456, ENC2_CHA_KEY = 456123, CHA_ENC1_KEY = 456789, ENC1_P1_KEY = 456000;

char * P1_ENC1_PTR = NULL, * ENC1_CHA_PTR = NULL, * CHA_ENC2_PTR = NULL, * ENC2_P2_PTR = NULL;
char * P2_ENC2_PTR = NULL, * ENC2_CHA_PTR = NULL, * CHA_ENC1_PTR = NULL, * ENC1_P1_PTR = NULL;

int SEM_KEY = 345678;
int semid;

char WHAT[MAX_MESSAGE_LENGTH] = "what?";

void createSharedMemorySegments() {
    if ((P1_ENC1_SHMID = shmget(P1_ENC1_KEY, SHMSIZE, 0666 | IPC_CREAT)) == -1) {
        printf("Warning: P1_ENC1 creation skipped \n");
    }
    if ((ENC1_CHA_SHMID = shmget(ENC1_CHA_KEY, SHMSIZE, 0666 | IPC_CREAT)) == -1) {
        printf("Warning: ENC1_CHA creation skipped \n");
    }
    if ((CHA_ENC2_SHMID = shmget(CHA_ENC2_KEY, SHMSIZE, 0666 | IPC_CREAT)) == -1) {
        printf("Warning: CHA_ENC2 creation skipped \n");
    }
    if ((ENC2_P2_SHMID = shmget(ENC2_P2_KEY, SHMSIZE, 0666 | IPC_CREAT)) == -1) {
        printf("Warning: ENC2_P2 creation skipped \n");
    }

    if ((P2_ENC2_SHMID = shmget(P2_ENC2_KEY, SHMSIZE, 0666 | IPC_CREAT)) == -1) {
        printf("Warning: P2_ENC2 creation skipped \n");
    }
    if ((ENC2_CHA_SHMID = shmget(ENC2_CHA_KEY, SHMSIZE, 0666 | IPC_CREAT)) == -1) {
        printf("Warning: ENC2_CHA creation skipped \n");
    }
    if ((CHA_ENC1_SHMID = shmget(CHA_ENC1_KEY, SHMSIZE, 0666 | IPC_CREAT)) == -1) {
        printf("Warning: CHA_ENC1 creation skipped \n");
    }
    if ((ENC1_P1_SHMID = shmget(ENC1_P1_KEY, SHMSIZE, 0666 | IPC_CREAT)) == -1) {
        printf("Warning: ENC1_P1 creation skipped \n");
    }
}

void destroySharedMemorySegments() {
    shmctl(P1_ENC1_SHMID, IPC_RMID, 0);
    shmctl(ENC1_CHA_SHMID, IPC_RMID, 0);
    shmctl(CHA_ENC2_SHMID, IPC_RMID, 0);
    shmctl(ENC2_P2_SHMID, IPC_RMID, 0);

    shmctl(P2_ENC2_SHMID, IPC_RMID, 0);
    shmctl(ENC2_CHA_SHMID, IPC_RMID, 0);
    shmctl(CHA_ENC1_SHMID, IPC_RMID, 0);
    shmctl(ENC1_P1_SHMID, IPC_RMID, 0);
}

void attachToSharedMemorySegments(bool showaddresses) {
    P1_ENC1_PTR = (char *) shmat(P1_ENC1_SHMID, (char *) 0, 0);
    ENC1_CHA_PTR = (char *) shmat(ENC1_CHA_SHMID, (char *) 0, 0);
    CHA_ENC2_PTR = (char *) shmat(CHA_ENC2_SHMID, (char *) 0, 0);
    ENC2_P2_PTR = (char *) shmat(ENC2_P2_SHMID, (char *) 0, 0);

    P2_ENC2_PTR = (char *) shmat(P2_ENC2_SHMID, (char *) 0, 0);
    ENC2_CHA_PTR = (char *) shmat(ENC2_CHA_SHMID, (char *) 0, 0);
    CHA_ENC1_PTR = (char *) shmat(CHA_ENC1_SHMID, (char *) 0, 0);
    ENC1_P1_PTR = (char *) shmat(ENC1_P1_SHMID, (char *) 0, 0);

    if (showaddresses) {
        printf("P1_ENC1_PTR = %p \n", P1_ENC1_PTR);
        printf("ENC1_CHA_PTR = %p \n", ENC1_CHA_PTR);
        printf("CHA_ENC2_PTR = %p \n", CHA_ENC2_PTR);
        printf("ENC2_P2_PTR = %p \n", ENC2_P2_PTR);

        printf("P2_ENC2_PTR = %p \n", P2_ENC2_PTR);
        printf("ENC2_CHA_PTR = %p \n", ENC2_CHA_PTR);
        printf("CHA_ENC1_PTR = %p \n", CHA_ENC1_PTR);
        printf("ENC1_P1_PTR = %p \n", ENC1_P1_PTR);
    }
}

void detachFromSharedMemorySegment() {
    shmdt(P1_ENC1_PTR);
    shmdt(ENC1_CHA_PTR);
    shmdt(CHA_ENC2_PTR);
    shmdt(ENC2_P2_PTR);

    shmdt(P2_ENC2_PTR);
    shmdt(ENC2_CHA_PTR);
    shmdt(CHA_ENC1_PTR);
    shmdt(ENC1_P1_PTR);
}

void down(int i) {
    struct sembuf semopr;
    semopr.sem_num = i;
    semopr.sem_op = -1;
    semopr.sem_flg = 0;
    if (semop(semid, &semopr, 1) == -1) {
        perror("semop down:");
        exit(1);
    }
}

void up(int i) {
    struct sembuf semopr;
    semopr.sem_num = i;
    semopr.sem_op = +1;
    semopr.sem_flg = 0;
    if (semop(semid, &semopr, 1) == -1) {
        perror("semop up: ");
        exit(1);
    }
}

void put(char message[MAX_MESSAGE_LENGTH], char hash[MAX_HASH_SIZE], char * buffer) {
    memcpy(buffer, message, MAX_MESSAGE_LENGTH);
    memcpy(buffer + MAX_MESSAGE_LENGTH, hash, MAX_HASH_SIZE);
}

void get(char message[MAX_MESSAGE_LENGTH], char hash[MAX_HASH_SIZE], char * buffer) {
    memcpy(message, buffer, MAX_MESSAGE_LENGTH);
    memcpy(hash, buffer + MAX_MESSAGE_LENGTH, MAX_HASH_SIZE);
}

void createSemaphores() {
    int i;
    union senum arg;

    semid = semget(SEM_KEY, TOTAL_SEMAPHORES, IPC_CREAT | 0600);

    if (semid != -1) {
        semctl(semid, 0, IPC_RMID, 0);

        semid = semget(SEM_KEY, TOTAL_SEMAPHORES, IPC_CREAT | 0600);
    }

    if (semid == -1) {
        printf("Warning: semaphores creation skipped \n");
    } else {
        for (i = 0; i < TOTAL_SEMAPHORES; i++) {
            if (i % 2 == 0) {
                arg.val = 1;
            } else {
                arg.val = 0;
            }
            semctl(semid, i, SETVAL, arg);
        }
    }
}

void destroySemaphores() {
    if (semid != -1) {
        semctl(semid, 0, IPC_RMID, 0);
    }
}

void readMessageFromKeyboard(int id, char message[MAX_MESSAGE_LENGTH]) {
    strcpy(message, "");
    do {
        printf("You are P%d. Type your message (TERM to quit): ", id);
        if (fgets(message, MAX_MESSAGE_LENGTH, stdin) == NULL) {
            break;
        }
    } while (strlen(message) <= 1);

    message[strlen(message) - 1] = '\0';
}

void p1() {
    char message_out[MAX_MESSAGE_LENGTH] = "";
    char message_in[MAX_MESSAGE_LENGTH] = "";
    char hash[MAX_HASH_SIZE] = "";

    int id = 1;

    printf("Starting process P%d \n", id);

    sleep(1); // debug

    while (1) {
        // read from keyboard, send to E1
        down(0);
        readMessageFromKeyboard(id, message_out);
        put(message_out, hash, P1_ENC1_PTR);
        up(1);

        if (strcmp(message_out, "TERM") == 0 || strcmp(message_out, "term") == 0) {
            break;
        }

        // get from E1
        down(15);
        get(message_in, hash, ENC1_P1_PTR);
        up(14);

        // check for integrity
        while (strcmp(message_in, "what?") == 0 || strcmp(message_in, "###") == 0) {
            if (strcmp(message_in, "what?") == 0) {
                printf("P1: sorry, P2 did not receive the message correctly \n");
                printf("P1: resending initial message: %s  \n", message_out);

                down(0);
                put(message_out, hash, P1_ENC1_PTR);
                up(1);
            } else if (strcmp(message_in, "###") == 0) {
                printf("P1: sorry I can't understand what P2 is saying, sending 'what?' \n");
                down(0);
                put(WHAT, hash, P1_ENC1_PTR);
                up(1);
            }

            // get from E1
            down(15);
            get(message_in, hash, ENC1_P1_PTR);
            up(14);
        }

        printf("P1: Received message from P2: '%s' \n", message_in); // debug

        if (strcmp(message_in, "TERM") == 0 || strcmp(message_in, "term") == 0) {
            break;
        }
    }

    printf("process P%d exited \n", id);
}

void e1() {
    char message[MAX_MESSAGE_LENGTH];
    char hash[MAX_HASH_SIZE];
    char hash2[MAX_HASH_SIZE];

    printf("process E1 started \n");

    printf("E1: Waiting for P1 \n");

    while (true) {
        // take from P1
        down(1);
        get(message, hash, P1_ENC1_PTR);
        printf("E1: Received message from P1: %s \n", message);
        up(0);

        // work
        char * q = GetMD5String(message, strlen(message));
        strncpy(hash, q, MAX_HASH_SIZE);
        free(q);

        printf("E1: Hash(%s) calculated: %s \n", message, hash);

        // give to C
        down(2);
        put(message, hash, ENC1_CHA_PTR);
        up(3);

        // end job:
        if (strcmp(message, "TERM") == 0 || strcmp(message, "term") == 0) {
            break;
        }

        // =====================================================

        // take from C
        down(13);
        get(message, hash, CHA_ENC1_PTR);
        printf("E1: Received message from P2: %s \n", message);
        up(12);

        // verify
        q = GetMD5String(message, strlen(message));
        strncpy(hash2, q, MAX_HASH_SIZE);
        free(q);

        if (strcmp(hash2, hash) == 0) { //message not corrupted
            printf("E1: Message correct: %s, hash:%s \n", message, hash);
        } else {
            printf("E1: Message corrupted: %s, hash:%s \n", message, hash);
            strcpy(message, "###");
        }

        // give to P1
        down(14);
        put(message, hash, ENC1_P1_PTR);
        up(15);

        // end job:
        if (strcmp(message, "TERM") == 0 || strcmp(message, "term") == 0) {
            break;
        }
    }


    printf("process E1 exited \n");
}

void c(int noise_probability) {
    char message[MAX_MESSAGE_LENGTH];
    char hash[MAX_HASH_SIZE];
    printf("process C started \n");

    // consume from E1_C
    printf("C: Waiting for E1 \n");
    srand(time(0));

    while (true) {
        // take from E1
        down(3);
        get(message, hash, ENC1_CHA_PTR);
        printf("C: Received message from P1: %s, hash:%s \n", message, hash);
        up(2);

        // add noise
        if (rand() % 100 < noise_probability && strcmp(message, "TERM") != 0 && strcmp(message, "term") != 0 && strcmp(message, "what?") != 0) {
            int l = rand() % strlen(message);
            message[l] = '#';
            printf("C: Message changed  : %s, hash:%s \n", message, hash);
        } else {
            printf("C: Message unchanged: %s, hash:%s \n", message, hash);
        }

        // give to E2
        down(4);
        put(message, hash, CHA_ENC2_PTR);
        up(5);

        // end job:
        if (strcmp(message, "TERM") == 0 || strcmp(message, "term") == 0) {
            break;
        }

        // ===========================================================

        // take from E2
        down(11);
        get(message, hash, ENC2_CHA_PTR);
        up(10);

        // add noise
        if (rand() % 100 < noise_probability && strcmp(message, "TERM") != 0 && strcmp(message, "term") != 0 && strcmp(message, "what?") != 0) {
            int l = rand() % strlen(message);
            message[l] = '#';
            printf("C: Message changed  : %s, hash:%s \n", message, hash);
        } else {
            printf("C: Message unchanged: %s, hash:%s \n", message, hash);
        }

        // give to E1
        down(12);
        put(message, hash, CHA_ENC1_PTR);
        up(13);

        // end job:
        if (strcmp(message, "TERM") == 0 || strcmp(message, "term") == 0) {
            break;
        }
    }

    printf("process C exited \n");
}

void e2() {
    char message[MAX_MESSAGE_LENGTH];
    char hash[MAX_HASH_SIZE];
    char hash2[MAX_HASH_SIZE];
    printf("process E2 started \n");

    while (true) {
        // take from E1
        down(5);
        get(message, hash, CHA_ENC2_PTR);
        printf("E2: Received message from P1: %s, hash:%s \n", message, hash);
        up(4);

        // verify
        char * q = GetMD5String(message, strlen(message));
        strncpy(hash2, q, MAX_HASH_SIZE);
        free(q);

        if (strcmp(hash2, hash) == 0) { //message not corrupted
            printf("E2: Message correct: %s, hash:%s \n", message, hash);
        } else {
            printf("E2: Message corrupted: %s, hash:%s \n", message, hash);
            strcpy(message, "###");
        }

        // give to P2
        down(6);
        put(message, hash, ENC2_P2_PTR);
        up(7);

        // end job:
        if (strcmp(message, "TERM") == 0 || strcmp(message, "term") == 0) {
            break;
        }

        // =====================================================

        // take from P2
        down(9);
        get(message, hash, P2_ENC2_PTR);
        printf("E2: Received message from P2: %s \n", message);
        up(8);

        // work
        q = GetMD5String(message, strlen(message));
        strncpy(hash, q, MAX_HASH_SIZE);
        free(q);

        printf("E2: Hash(%s) calculated: %s \n", message, hash);

        // give to C
        down(10);
        put(message, hash, ENC2_CHA_PTR);
        up(11);

        // end job:
        if (strcmp(message, "TERM") == 0 || strcmp(message, "term") == 0) {
            break;
        }
    }


    printf("process E2 exited \n");
}

void p2() {
    char message_in[MAX_MESSAGE_LENGTH] = "";
    char message_out[MAX_MESSAGE_LENGTH] = "";
    char hash[MAX_HASH_SIZE];
    int id = 2;
    printf("Starting process P%d \n", id);

    while (true) {
        // take from E2
        down(7);
        get(message_in, hash, ENC2_P2_PTR);
        printf("P2: Received message: %s, hash:%s \n", message_in, hash); // debug
        up(6);

        while (strcmp(message_in, "what?") == 0 || strcmp(message_in, "###") == 0) {
            if (strcmp(message_in, "what?") == 0) {
                printf("P2: sorry, P1 did not receive the message correctly \n");
                printf("P2: resending initial message: %s  \n", message_out);

                down(8);
                put(message_out, message_out, P2_ENC2_PTR);
                up(9);
            } else if (strcmp(message_in, "###") == 0) {
                printf("P2: sorry I can't understand what P1 is saying, sending 'what?' \n");
                down(8);
                put(WHAT, hash, P2_ENC2_PTR);
                up(9);
            }

            down(7);
            get(message_in, hash, ENC2_P2_PTR);
            printf("P2: Received message: %s, hash:%s \n", message_in, hash); // debug
            up(6);
        }

        if (strcmp(message_in, "TERM") == 0 || strcmp(message_in, "term") == 0) {
            break;
        }

        printf("P2: Received message from P1: '%s' \n", message_in); // debug

        // ===============================================================

        // give to E2
        down(8);
        readMessageFromKeyboard(id, message_out);
        put(message_out, hash, P2_ENC2_PTR);
        up(9);

        if (strcmp(message_out, "TERM") == 0 || strcmp(message_out, "term") == 0) {
            break;
        }
    }


    printf("process P%d exited\n", id);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("missing probability \n");
        return -1;
    }

    int noise_probability = atoi(argv[1]);
    
    if (noise_probability < 0 && noise_probability >= 100) {
        printf("Noise probability = %d %% \n", noise_probability);
        printf("invalid value for probability \n");
        return -1;
    }

    printf("Noise probability = %d %% \n", noise_probability);

    createSharedMemorySegments();

    attachToSharedMemorySegments(true);

    createSemaphores();

    pid_t pids[5];


    pids[0] = fork();

    if (pids[0] == 0) {
        p1();
        return 0;
    }


    pids[1] = fork();

    if (pids[1] == 0) {
        e1();
        return 0;
    }


    pids[2] = fork();

    if (pids[2] == 0) {
        c(noise_probability);
        return 0;
    }


    pids[3] = fork();

    if (pids[3] == 0) {
        e2();
        return 0;
    }


    pids[4] = fork();

    if (pids[4] == 0) {
        p2();
        return 0;
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);

    detachFromSharedMemorySegment();

    destroySharedMemorySegments();

    destroySemaphores();

    printf("Done. \n");

    return 0;
}

