#include "threads.h"
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include<sys/time.h>
#define STACK_SIZE 65536

int created = 0;
char *stack_array[MAX_THREADS];
tcb_t thread_context[MAX_THREADS];
tcb_t *current_thread_context;
int new_pos;

struct itimerval timer;
struct itimerval stoptimer;

void sigusr_handler(int signal_number) {
	if(setjmp(thread_context[new_pos].buffer) == 0) {
		created = 1;
	}
	else {
		current_thread_context->function(current_thread_context->argument);
	}
}

void alarm_activate(){
    timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 100000;
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 100000;

	if(setitimer(ITIMER_REAL, &timer, NULL) == -1) {
		perror("setitimer");
		exit(EXIT_FAILURE);
	}
}

void alarm_deactivate(){
    stoptimer.it_interval.tv_sec = 0;
	stoptimer.it_interval.tv_usec = 0;
	stoptimer.it_value.tv_sec = 0;
	stoptimer.it_value.tv_usec = 0;

	if(setitimer(ITIMER_REAL, &stoptimer, NULL) == -1) {
		perror("setitimer");
		exit(EXIT_FAILURE);
	}
}

void sigalm_handler(int signal) {
	thread_yield();
}

void thread_init(int preemption_enabled){
    for(int i = 0; i < MAX_THREADS; i++){
        thread_context[i].state = STATE_INVALID; 
        stack_array[i] = malloc(STACK_SIZE);
    }
    thread_context[0].function = NULL;
    thread_context[0].argument = NULL;
    thread_context[0].return_value = NULL;
    thread_context[0].state = STATE_ACTIVE;
    current_thread_context = &thread_context[0];

    created = 0;
    struct sigaction sigusr_hints;

	memset(&sigusr_hints, 0, sizeof(struct sigaction));
	sigusr_hints.sa_handler = sigusr_handler;
	sigusr_hints.sa_flags = SA_ONSTACK; 
	sigemptyset(&sigusr_hints.sa_mask);

	if(sigaction(SIGUSR1, &sigusr_hints, NULL) == -1) {
		perror("sigaction/SIGUSR1");
		exit(EXIT_FAILURE);
	}

    struct sigaction options; //setting up handler for thread switching

	memset(&options, 0, sizeof(struct sigaction));
	options.sa_handler = sigalm_handler;

	if(sigaction(SIGALRM, &options, NULL) == -1) {
		perror("sigaction/SIGALRM");
		exit(EXIT_FAILURE);
	}
}

int thread_create(void *(*function)(void *), void *argument){
    for(int i = 0; i < MAX_THREADS; i++){
        if(thread_context[i].state == STATE_INVALID){
            thread_context[i].function = function;
            thread_context[i].argument = argument;
            thread_context[i].state = STATE_ACTIVE;

            new_pos = i;
            stack_t new_stack;

            new_stack.ss_flags = 0;
            new_stack.ss_size = STACK_SIZE;
            new_stack.ss_sp = stack_array[i];

            if(sigaltstack(&new_stack, NULL) == -1) {
                perror("sigaltstack");
                exit(EXIT_FAILURE);
            }

            raise(SIGUSR1);

            while(!created) {}; 
           
            return(thread_context[i].number);
        }
    }
    return 0;
}

int thread_yield(void){
    alarm_deactivate();
    for(int x = 0; x < MAX_THREADS - 1; x++){
        int i = ((current_thread_context->number+1) + x)%MAX_THREADS;
        if(thread_context[i].state == STATE_ACTIVE){
            if(setjmp(current_thread_context->buffer) == 0){
                current_thread_context = &thread_context[i];
                longjmp(thread_context[i].buffer, 1);
            }
            alarm_activate();
            return 1;
        }
    }
    return 0;
}

void thread_exit(void *return_value){
    current_thread_context->state = STATE_FINISHED;
    current_thread_context->return_value = return_value;

    if(current_thread_context->joiner_thread_number == -1){
        thread_yield();
    }
    else{
        current_thread_context->state = STATE_ACTIVE;
        if(setjmp(current_thread_context->buffer) == 0){
                current_thread_context = &thread_context[current_thread_context->joiner_thread_number];
                longjmp(thread_context[current_thread_context->joiner_thread_number].buffer, 1);
        }
    }
}

void thread_join(int target_thread_number){ 
    if(thread_context[target_thread_number].state == STATE_FINISHED){
        thread_context[target_thread_number].state = STATE_INVALID;
        return;
    }
    else{
        thread_context[target_thread_number].joiner_thread_number = current_thread_context->number;
        current_thread_context->state = STATE_BLOCKED;
        if(setjmp(current_thread_context->buffer) == 0){
                current_thread_context = &thread_context[target_thread_number];
                longjmp(thread_context[target_thread_number].buffer, 1);
        }
        //thread_context[target_thread_number].state = STATE_ACTIVE;
        return;
    }

}
