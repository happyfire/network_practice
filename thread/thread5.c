//
//本示例演示了线程属性的使用，并使用了detach state属性，设置子线程为detached，这样子线程和主线程脱落，子线程自行结束，主线程不需要等待子线程join。
//线程属性的类型为 pthread_attr_t
//使用 pthread_attr_init 初始化， 使用 pthread_attr_destroy 释放。
//使用 pthread_attr_setdetachstate 设置detach状态
//
//注意：如果不设置detached属性，程序运行起来看上去没什么区别。实际是一个线程如果是joinable的（默认），则必须调用pthread_join去回收他的资源，包含线程描述符和栈，否则资源无法释放；而线程如果设置为detached的，则不需要调用pthread_join，该线程结束后资源会被系统回收。
//当然，当进程结束时，所有的线程资源都会被回收。

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg);

char message[] = "Hello World";
int thread_finished = 0;

int main()
{
	int res;
	pthread_t a_thread;
	pthread_attr_t thread_attr;

	res = pthread_attr_init(&thread_attr);
	if(res != 0){
		perror("Attribute creation failed");
		exit(EXIT_FAILURE);
	}
	res = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	if(res != 0){
		perror("Setting detached attribute failed");
		exit(EXIT_FAILURE);
	}
	res = pthread_create(&a_thread, &thread_attr, thread_function, (void *)message);
	if(res != 0){
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	pthread_attr_destroy(&thread_attr);
	while(!thread_finished){
		printf("Waiting for thread to say it's finished...\n");
		sleep(1);
	}
	printf("Other thread finished, bye!\n");
	exit(EXIT_SUCCESS);
}

void *thread_function(void *arg)
{
	printf("thread_function is running. Argument was %s\n", (char *)arg);
	sleep(4);
	printf("Second thread setting finished flag, and exiting now\n");
	thread_finished = 1;
	pthread_exit(NULL);
}