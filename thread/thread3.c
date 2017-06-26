//信号量 semaphore
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
#endif

	res = pthread_create(&a_thread, NULL, thread_function, NULL);
	if(res != 0){
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	printf("Input some text. Enter 'end' to finish\n");
	while(strncmp("end", work_area, 3) != 0){
		fgets(work_area, WORK_SIZE, stdin);
		post_semaphore();
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
#endif
	exit(EXIT_SUCCESS);
}

void * thread_function(void *arg)
{

	wait_semaphore();
	while(strncmp("end", work_area, 3) != 0){
		printf("You input %lu characters\n", strlen(work_area) - 1);
		wait_semaphore();
	}
	pthread_exit(NULL);
}
