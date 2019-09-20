#include "thread.h"


void rq_enqueue(thread_t * new){

	thread_t * tmp;

	if(rq->head == NULL){

		rq->head = new;
		rq->tail = new;
		rq->head->next = NULL;
		rq->head->prev = NULL;

	}else{

		tmp = rq->tail;

		rq->tail = new;

		rq->tail->prev = NULL;

		rq->tail->next = tmp;

		tmp->prev = rq->tail;
	}
}

thread_t * rq_dequeue(){
	
	thread_t * hd = rq->head;
	if(rq->head !=NULL){
		rq->head = rq->head->prev;
	}

	if(rq->head !=NULL){
		rq->head->next = NULL;
	}

	return hd; 
}

thread_t * rq_top(){
	
	return rq->head; 
}

void rq_print_tl(){

	thread_t * tmp;

	for(tmp=rq->tail; tmp!=NULL ; tmp = tmp->next){

		printf(" I am tid: %d \n",tmp->tid);
	}
}

void rq_print_hd(){

	thread_t * tmp;

	for(tmp=rq->head; tmp!=NULL ; tmp = tmp->prev){

		printf(" I am tid: %d \n",tmp->tid);
	}
}
