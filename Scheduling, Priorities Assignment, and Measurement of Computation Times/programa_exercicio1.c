#define _GNU_SOURCE

#include "func.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <math.h>
#include <sys/types.h>

//Conversion defines
#define SecToNanosec 1E9
#define SecToMilisec 1E3
#define MilisecToNanosec 1E6

struct timespec start, stop;

//******************************* Manipulação do tempo ************************
long double sendResult(struct timespec begin, struct timespec end){
	struct timespec tempo;
	if((end.tv_nsec - begin.tv_nsec) < 0){
		tempo.tv_sec = end.tv_sec - begin.tv_sec - 1;
		tempo.tv_nsec = SecToNanosec + end.tv_nsec - begin.tv_nsec;
	}
	else{
		tempo.tv_sec = end.tv_sec - begin.tv_sec;
		tempo.tv_nsec = end.tv_nsec - begin.tv_nsec;
	}
	
	long double resultado = (long double)(tempo.tv_sec * (long double)SecToMilisec + tempo.tv_nsec/(long double)MilisecToNanosec);
	return resultado;
}
//******************************* MAIN **********************************************
int main(int argc, char **argv){
	struct sched_param parametro;
	
	
	//Máscara para utilizar apenas um core do CPU
	cpu_set_t singlecore;
	
	//Bloqueia todo o endereço de memória do processo caller na RAM, para evitar swap
	if(mlockall(MCL_CURRENT | MCL_FUTURE) == -1){
		perror("mlockall");
		exit(EXIT_FAILURE);
	}
	
	//Limpa a máscara e adiciona o CPU atual, e apenas deixa o programa usar esse core
	CPU_ZERO(&singlecore);
	CPU_SET(0, &singlecore);
	if(sched_setaffinity(0, sizeof(singlecore), &singlecore)  == -1){
		perror("sched_setaffinity");
		exit(EXIT_FAILURE);
	}
	
	parametro.sched_priority = sched_get_priority_max(SCHED_FIFO);
	if(sched_setscheduler(0, SCHED_FIFO, &parametro) == -1){
		perror("sched_setscheduler");
		exit(EXIT_FAILURE);
	}
	
	if(sched_setparam(0, &parametro) == -1){
		perror("sched_setparam");
		exit(EXIT_FAILURE);
	}
	
	//Começa relógio F1
	if(clock_gettime(CLOCK_MONOTONIC, &start) == -1){
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}
	
	f1(1, 1);
	
	//Termina relógio F1
	if(clock_gettime(CLOCK_MONOTONIC, &stop) == -1){
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}
	
	long double resultado1 = sendResult(start, stop);
	
	if(clock_gettime(CLOCK_MONOTONIC, &start) == -1){
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}
	
	f2(2, 2);
	
	if(clock_gettime(CLOCK_MONOTONIC, &stop) == -1){
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}
	
	long double resultado2 = sendResult(start, stop);
	
	if(clock_gettime(CLOCK_MONOTONIC, &start) == -1){
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}
	
	f3(3, 3);
	
	if(clock_gettime(CLOCK_MONOTONIC, &stop) == -1){
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}

	long double resultado3 = sendResult(start, stop);
	
	printf("Tempos obtidos das funcoes: \n");
	printf("\nTempo total F1: %Lfms\n", resultado1);
	printf("Tempo total F2: %Lfms\n", resultado2);
	printf("Tempo total F3: %Lfms\n\n", resultado3);
	
	if(munlockall() == -1){
		perror("mlockall");
		exit(EXIT_FAILURE);
	}
	
	return (EXIT_SUCCESS);
}
