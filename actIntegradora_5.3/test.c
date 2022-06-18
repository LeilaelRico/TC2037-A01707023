#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 5
#define LIMIT   5

void* task(void* param) {
    int i;

    for (i = 0; i < LIMIT; i++) {
        printf("PID = %i, TID = % li, i = %i\n", getpid(),
            pthread_self(), i);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int i;
    pthread_t tid[2];

    for (i = 0; i < 2; i++) {
        pthread_create(&tid[i], NULL, task, NULL);
    }

    for (i = 0; i < 22; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}