//本示例演示了用低效的轮询方法同步两个线程的执行。
//通过对一个全局变量run_now的值的控制，两个线程轮流执行。

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg);
int run_now = 1;
char message[] = "Hello World";

int main()
{
	int res;
	pthread_t a_thread;
	void *thread_result;
	int print_count1 = 0;

	res = pthread_create(&a_thread, NULL, thread_function, (void*)message);
	if(res != 0){
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	while(print_count1++ < 20){
		if(run_now == 1){
			printf("1\n");
			run_now = 2;
		}
		else{
			sleep(1);
		}
	}

	printf("\nWaiting for thread to finish...\n");
	res = pthread_join(a_thread, &thread_result);
	if(res !=0){
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}

	printf("Thread joined, result is %s\n", (char*)thread_result);
	exit(EXIT_SUCCESS);
}

void *thread_function(void *arg)
{
	int print_count2 = 0;

	while(print_count2++ <20){
		if(run_now == 2){
			printf("2\n");
			run_now = 1;
		}
		else{
			sleep(1);
		}
	}

	sleep(3);
	return NULL;
}
