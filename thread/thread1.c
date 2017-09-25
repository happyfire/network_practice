//本实例演示了pthread的基本用法
//		使用pthred_create创建线程，创建成功后子线程立即开始执行，可通过最后一个参数设置线程的参数，即线程函数调用时传入的参数。
//在线程函数内部，调用pthread_exit退出线程，pthread_exit的参数是向主线程返回的结果，注意这个指针不能指向局部变量。也可通过return直接返回。
//主线程通过调用pthread_join等待子线程结束，并获取子线程传出的结果。ptread_join会阻塞住直到指定的线程返回。
//在任意线程中调用exit都会直接终止进程。
//线程有独立的局部变量，其他变量都会共享。

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *thread_function(void *arg);

char message[] = "Hello World";

int main()
{
	int res;
	pthread_t a_thread;
	void *thread_result;

	res = pthread_create(&a_thread, NULL, thread_function, (void*)message);
	if(res != 0){
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}
	printf("Waiting for thread to finish...\n");

	res = pthread_join(a_thread, &thread_result);
	if(res !=0){
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}

	printf("Thread joined, it returned: %s\n", (char*)thread_result);
	printf("Message is now %s\n", message);
	exit(EXIT_SUCCESS);
}

void *thread_function(void *arg)
{
	printf("thread_function is running. Argument was %s\n", (char *)arg);
	sleep(3);
	strcpy(message, "Bye!");
	pthread_exit("Thank you for the CPU time"); //or return
	//return "Thank you for the CPU time!";
}