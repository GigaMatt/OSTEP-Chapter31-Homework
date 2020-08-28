/**
 * CS 170 - Theory of Operating Systems
 * Homework 6: Chapter 31, Questions 2
 * Modifications By: Matthew S Montoya
 * Based on OSTEP Code by Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau
 * Last Modified: 9 April 2020
 */

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>  //sem_init included here
#include <pthread.h>    //pthread_t included here

sem_t s1, s2;   //sem_t included in semaphore.h

void *child_1(void *arg) {
    printf("child 1: before\n");

    /* Increment Sem; Wake Thread blocked on Sem */
    sem_post(&s1);
    /* Wait Until Sem > 0; Decrement Sem & Return */
    sem_wait(&s2);

    printf("child 1: after\n");
    return NULL;
}

void *child_2(void *arg) {
    sleep(1);
    printf("child 2: before\n");
    
    /* Increment Sem; Wake Thread blocked on Sem */
    sem_post(&s2);
    /* Wait Until Sem > 0; Decrement Sem & Return */
    sem_wait(&s1);
    
    printf("child 2: after\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;   // included in pthread.h
    printf("parent: begin\n");

    // init semaphores here
    sem_init(&s1, 1, 0);
    sem_init(&s2, 1, 0);

    pthread_create(&p1, NULL, child_1, NULL);
    pthread_create(&p2, NULL, child_2, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("parent: end\n");
    return 0;
}