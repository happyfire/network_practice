//信号量 semaphore
//本示例演示信号量semaphore的使用方法。
//头文件： #include <semaphore.h>
//		信号量使用类型 sem_t 定义。
//使用 sem_init 初始化信号量，第一个参数为sem_t类型的指针，第二个参数为0，第三个参数为信号量的初始值。sem_init返回值为0则调用成功，非0 则出错。
//不同线程对于信号量的操作是原子的，有两个操作 sem_post和sem_wait。
//sem_post将信号量的值加一，sem_wait先测试信号量的值是否大于0，如果大于0则减一，否则阻塞等待。
//如果有多个线程调用sem_wait去等待一个信号量从0变成非0，当另外的线程通过sem_post将该信号量值加一后，只有一个等待的线程sem_wait会返回，将信号量减一后该线程继续向下执行，而其他等待的线程则继续等待在sem_wait上。
//当使用完毕后，使用 sem_destroy 释放信号量资源。
//
//注意apple系统的信号量接口不同，请看相关代码
//另外本示例增加了原书thread3a示例FAST失败的修复，但没有再兼容apple代码，因此apple上不可用
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void *thread_function(void *arg);

#ifdef __APPLE__
sem_t p_bin_sem;
#else
sem_t bin_sem;
#endif

sem_t bin_sem2;

#define WORK_SIZE 1024
char work_area[WORK_SIZE];

void post_semaphore()
{
#ifdef __APPLE__
	sem_post(p_bin_sem);
#else
	sem_post(&bin_sem);
#endif
}

void wait_semaphore()
{
#ifdef __APPLE__
	sem_wait(p_bin_sem);
#else
	sem_wait(&bin_sem);
#endif
}

int main()
{
	int res;
	pthread_t a_thread;
	void *thread_result;

#ifdef __APPLE__
	p_bin_sem = sem_open("/mysem",O_CREAT, S_IRUSR | S_IWUSR, 0);
#else
	res = sem_init(&bin_sem, 0, 0);
	if(res != 0){
		perror("Semaphore initialization failed");
		exit(EXIT_FAILURE);
	}

	res = sem_init(&bin_sem2, 0, 0);
	if(res != 0){
		perror("Semaphore init failed 2.");
		exit(EXIT_FAILURE);
	}
#endif

	res = pthread_create(&a_thread, NULL, thread_function, NULL);
	if(res != 0){
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	printf("Input some text. Enter 'end' to finish\n");
	while(strncmp("end", work_area, 3) != 0){
		//原书thread3a示例中的FAST失败示例，本程序使用额外的信号量进行等待处理
		if(strncmp(work_area, "FAST", 4) ==0){
			//post_semaphore(); //这是多余的一次post，原书只是举例会执行多次
			strcpy(work_area, "Wheeee...");
		} else {
			fgets(work_area, WORK_SIZE, stdin);
		}
		post_semaphore();

		//等待子线程计数完毕
		//注意如果是输入了end由于子线程会跳出循环从而不会post bin_sem2，所以end的情况不用等bin_sem2
		if(strncmp("end", work_area, 3) != 0) {
			sem_wait(&bin_sem2);
		}
	}
	printf("\nWaiting for thread to finish...\n");
	res = pthread_join(a_thread, &thread_result);
	if(res != 0){
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}

	printf("Thread joined\n");
#ifdef __APPLE__
	sem_close(p_bin_sem);
	sem_unlink("/mysem");
#else
	sem_destroy(&bin_sem);
	sem_destroy(&bin_sem2);
#endif
	exit(EXIT_SUCCESS);
}

void * thread_function(void *arg)
{

	wait_semaphore();
	while(strncmp("end", work_area, 3) != 0){
		printf("You input %s %lu characters\n", work_area, strlen(work_area) - 1);
		sem_post(&bin_sem2);
		wait_semaphore();
	}

	pthread_exit(NULL);
}
