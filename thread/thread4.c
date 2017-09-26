// mutex
//本示例演示互斥量的使用。
//互斥量的类型为 pthread_mutex_t，使用 pthread_mutex_init初始化，其中第二个参数为属性，可指定锁的类型。
//
//mutex对象使用 pthread_mutex_lock加锁，如果mutex已经锁上，调用加锁的线程会阻塞直到mutex可用。该函数返回时，mutex对象处于加锁状态，且锁的拥有者是调用线程。
//
//mutex有多种类型，不同类型的锁处理加锁解锁的规则有所区别。
//默认类型下(PTHREAD_MUTEX_DEFAULT),必须先加锁再由加锁的线程解锁，其他都是未定义行为，比如重复锁定，非加锁线程去解锁，未加锁就解锁都是非法的。
//PTHREAD_MUTEX_NORMAL类型，重复加锁会造成死锁(deadlock),未加锁或已解锁时解锁是未定义行为。
//PTHREAD_MUTEX_ERRORCHECK类型，会提供错误检查功能。重复加锁会返回错误，未加锁或已解锁时解锁也会返回错误。
//PTHREAD_MUTEX_RECURSIVE类型，会对锁引用计数。加锁增加计数，解锁减少计数。当解锁后计数为0才是真正解锁，其他线程可获得该锁。未加锁或已解锁时解锁会返回错误。
//
//pthread_mutex_trylock()和pthread_mutex_lock()功能一样，区别在于如果mutex已经被锁住，pthread_mutex_trylock调用会立即返回。如果mutex类型为PTHREAD_MUTEX_RECRSIVE,且mutex的拥有者是调用线程，则mutex引用计数加一，且pthread_mutex_trylock立即返回成功。
//
//pthread_mutex_unlock()释放锁，行为根据锁的类型而定。当unlock时有多个线程阻塞在锁上时，锁变成可用，并且有调度策略决定哪个线程获得该锁。

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void *thread_function(void *arg);
pthread_mutex_t work_mutex; // protects both work_area and time_to_exit

#define WORK_SIZE 1024
char work_area[WORK_SIZE];
int time_to_exit = 0;

int main()
{
	int res;
	pthread_t a_thread;
	void *thread_result;
	res = pthread_mutex_init(&work_mutex, NULL);
	if(res != 0){
		perror("Mutex initialization failed");
		exit(EXIT_FAILURE);
	}

	res = pthread_create(&a_thread, NULL, thread_function, NULL);
	if(res != 0){
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	pthread_mutex_lock(&work_mutex);
	printf("Input some text. Enter 'end' to finish\n");
	while(!time_to_exit){
		fgets(work_area, WORK_SIZE, stdin);
		pthread_mutex_unlock(&work_mutex);
		while(1){
			pthread_mutex_lock(&work_mutex);
			if(work_area[0] != '\0'){
				pthread_mutex_unlock(&work_mutex);
				sleep(1);
			}
			else{
				break;
			}
		}
	}
	pthread_mutex_unlock(&work_mutex);
	printf("\nWaiting for thread to finish...\n");
	res = pthread_join(a_thread, &thread_result);
	if(res != 0){
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}
	printf("Thread joined\n");
	pthread_mutex_destroy(&work_mutex);
	exit(EXIT_SUCCESS);
}

void *thread_function(void *arg)
{
	sleep(1);
	pthread_mutex_lock(&work_mutex);
	while(strncmp("end", work_area, 3) != 0){
		printf("You input %lu characters\n", strlen(work_area) - 1);
		work_area[0] = '\0';
		pthread_mutex_unlock(&work_mutex);
		sleep(1);
		pthread_mutex_lock(&work_mutex);
		while(work_area[0] == '\0'){
			pthread_mutex_unlock(&work_mutex);
			sleep(1);
			pthread_mutex_lock(&work_mutex);
		}
	}
	printf("time to exit");
	time_to_exit = 1;
	work_area[0] = '\0';
	pthread_mutex_unlock(&work_mutex);
	pthread_exit(0);
}

