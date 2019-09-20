#ifndef THREAD_INCLUDED
#define THREAD_INCLUDED
// #define __USE_GNU

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <ucontext.h>
#include <time.h>

#include "chan.h"
#include "sem.h"

void _BEGINMONITOR(void);

void _thrstart(void);

void Thread_init(void);

int Thread_new(int func(void *), void *args, size_t nbytes, ...);

void Thread_exit(int code);

int Thread_self(void);

int Thread_join(int tid);

void Thread_pause(void);

void sig_handler(int signo, siginfo_t *info, void *context);

void _swtch(void *from, void *to);



typedef struct Thread_struct {
	unsigned long ip;
	unsigned long sp;
	void * funcptr;
	void * args;
	int nbytes;

	int tid;
	struct Thread_struct * next;
	struct Thread_struct * prev;

}thread_t;


thread_t * thread_array[2];

typedef struct run_queue {

	thread_t * head;
	thread_t * tail;

}RQ;



void rq_enqueue(thread_t * new);
 
thread_t * rq_dequeue();

thread_t * rq_top();

void rq_print_hd(void);

void rq_print_tl(void);

RQ * rq;

int MAX_THREADS;
int CURR_THREAD;

int threadlib;
int curr_flag;

void _ENDMONITOR(void);

#endif


/*
https://www.csd.uoc.gr/~hy527/as1/thread.3.html
*/