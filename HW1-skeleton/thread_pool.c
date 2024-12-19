/*
 * Copyright (c) 2017, Hammurabi Mendes.
 * Licence: BSD 2-clause
 */

#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include "ll_double.h"
#include "clients_common.h"
#include "thread_pool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>

#define NUM_THREADS 16

int queue_size = 0;
pthread_mutex_t queue_lock;
pthread_cond_t queue_not_empty;
int finish = 0;
atomic_ulong operations_completed = 0;
pthread_t threads[NUM_THREADS];

struct request {
	struct client *client;
};

struct List list;

void put_request(struct client *client) { //Get a new request and add it to the list. The request will be taken care of when a thread opens up
	pthread_mutex_lock(&queue_lock);
	struct request *request = malloc(sizeof(struct request));
	request->client = client;
	ll_insert_head(&list, request);
	queue_size++;
	pthread_cond_signal(&queue_not_empty);
	pthread_mutex_unlock(&queue_lock);
}

void *consumer() {
	struct request *request;
	while(finish == 0) {
		pthread_mutex_lock(&queue_lock);

		while(queue_size == 0) {
			pthread_cond_wait(&queue_not_empty, &queue_lock);
			if(finish == 1){
				pthread_mutex_unlock(&queue_lock);
				break;
			}
			request = ll_remove_tail(&list)->data;
			queue_size--;
			pthread_mutex_unlock(&queue_lock);


			struct client *client = request->client;
			if(read_request(client)) {
				write_reply(client);
			}

			if(client->status == STATUS_OK) {
				atomic_fetch_add(&operations_completed, 1);
			}

			free(client);
			free(request);
		}
	}

	return NULL;
}

int start_threads(void){
	pthread_mutex_init(&queue_lock, NULL);
	pthread_cond_init(&queue_not_empty, NULL);

	ll_init(&list);

	for(int i = 0; i < NUM_THREADS; i++){
 		pthread_t consumerID;
		pthread_create(&consumerID, NULL, consumer, NULL);
	}
	return 1;
}

int finish_threads(void){
	pthread_mutex_lock(&queue_lock);
	finish = 1;
	pthread_cond_broadcast(&queue_not_empty);
	pthread_mutex_unlock(&queue_lock);
	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join(threads[i], NULL);
	}
	return 1;
}

#endif /* THREAD_POOL_H */
