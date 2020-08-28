# CS 4375 - Operating Systems

## Homework 6

* OSTEP Chapter 31, questions 2

### Author: Matthew S Montoya

## Chapter 31

In this homework, we’ll use semaphores to solve some well-known concurrency problems. Many of these are taken from Downey’s excellent “Little Book of Semaphores”, which does a good job of pulling together a number of classic problems as well as introducing a few new variants; interested readers should check out the Little Book for more fun.</br>

On Linux, you will be using native semaphores; on a Mac (where there is no semaphore support, you’ll have to first build an implementation (using locks and condition variables, as described in the chapter). Good luck!

**2. Let’s now generalize this a bit by investigating the rendezvous problem. The problem is as follows: you have two threads, each of which are about to enter the rendezvous point in the code. Neither should exit this part of the code before the other enters it. Consider using two semaphores for this task, and see ```rendezvous.c``` for details.**

Listed below is the terminal output after modifying ```rendezvous.c``` provided by the OSTEP book, and the modified ```rendezvous.c``` code. This is based on my understanding of Chapter 31 & Race Conditions. </br>

Within the original code, the problem noted, "if done correctly, each child should print their "before" message before either prints their "after" message. Test by adding sleep(1) calls in various locations." We see this conditon holding true in the output of the modified code.

```text
(base) montoms1@ubuntu:~/Downloads/github-repos/cs170-os/homework-6-GigaMatt/starter-code$ ./rendezvous.o
parent: begin
child 1: before
child 2: before
child 2: after
child 1: after
parent: end
```

Below is the modified code, based on the original code provided by the authors. To ensure we got the output listed above, we needed to add ```post()``` and ```wait()``` functions (operations), called from the ```semaphore.h``` library, in both *child_1* and *child_2*.  In doing so, we increment the value of the semaphore, wake up the thread that is blocked, then wait for the semaphore to be greater than zero before decrementing the semaphore again. In *child_1*, we'll call post(s1) and wait(s2), while calling post(s2) and wait(s1) in *child_2*. This ensures each child prints their "before" message before either prints their "after" message. Lastly, we initialize both semaphores by calling ```sem_init()``` and pass through *s1* and *s2*.

```C
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
```
