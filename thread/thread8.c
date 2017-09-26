//
//多线程演示程序。
//创建多个线程并传入不同的参数，然后已相反的顺序去join线程。
//子线程随机等待时间并结束。
//这个演示的问题在于如果把sleep(1)去掉，子线程接收到的参数可能被主线程修改，
//因为传入的是一个指针。加上sleep则让主线程等待从而降低这一问题发生的几率。
//thread8a.c中修改了这个问题，直接将参数强制转换为void*并传入给子线程。

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 6

void *thread_function(void *arg);

int main()
{
	int res;
	pthread_t a_thread[NUM_THREADS];
	void *thread_result;
	int lots_of_threads;

	for(lots_of_threads = 0; lots_of_threads < NUM_THREADS; lots_of_threads++){
		res = pthread_create(&(a_thread[lots_of_threads]), NULL, thread_function, (void*)&lots_of_threads);
		if(res != 0){
			perror("Thread creation failed");
			exit(EXIT_FAILURE);
		}
		sleep(1);
	}

	printf("Waiting for threads to finish...\n");
	for(lots_of_threads = NUM_THREADS - 1; lots_of_threads >= 0; lots_of_threads--){
		res = pthread_join(a_thread[lots_of_threads], &thread_result);
		if(res == 0){
			printf("Picked up a thread %d\n",lots_of_threads);
		}
		else{
			perror("pthread_join failed");
		}
	}

	printf("All done\n");
	exit(EXIT_SUCCESS);
}

void *thread_function(void *arg)
{
	int my_number = *(int *)arg;
	int rand_num;

	printf("thread_function is running. Argument was %d\n", my_number);
	rand_num = 1+(int)(9.0*rand()/(RAND_MAX+1.0));
	sleep(rand_num);
	printf("Bye from %d\n", my_number);
	pthread_exit(NULL);
}
