//本示例演示了线程的取消。
//发起取消的一方使用 pthread_cancel() 去取消一个线程。但线程是否能取消，怎么取消则是由被取消线程自己决定。
//被取消线程使用 pthread_setcancelstate()去设置自己是否能被取消，默认为PTHREAD_CANCEL_ENABLE。
//使用 pthread_setcanceltype()设置取消类型，默认为PTHREAD_CANCEL_DEFERRED	。DEFERRED类型会推迟取消动作直到线程运行到一个取消点，
//取消点通常是一个阻塞操作，比如pthread_join, pthread_cond_wait, pthread_cond_timewait, pthread_testcancel, sem_wait, sigwait。
//void pthread_testcancel(void)
//检查本线程是否处于Canceld状态，如果是，则进行取消动作，否则直接返回。
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg);

int main(){
	int res;
	pthread_t a_thread;
	void *thread_result;

	res = pthread_create(&a_thread, NULL, thread_function, NULL);
	if(res != 0){
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	sleep(3);
	printf("Canceling thread...\n");
	res = pthread_cancel(a_thread);
	if(res != 0){
		perror("Thread cancelation failed");
		exit(EXIT_FAILURE);
	}
	printf("Waiting for thread to finish...\n");
	res = pthread_join(a_thread, &thread_result);
	if(res != 0){
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

void *thread_function(void *arg)
{
	int i, res;
	res = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if(res != 0){
		perror("Thread pthread_setcancelstate failed");
		exit(EXIT_FAILURE);
	}
	res = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	if(res != 0){
		perror("Thread pthread_setcanceltype failed");
		exit(EXIT_FAILURE);
	}
	printf("thread_function is running\n");
	for(i = 0; i < 10; i++){
		printf("Thread is still running (%d)...\n", i);
		sleep(1);
	}
	pthread_exit(0);
}

