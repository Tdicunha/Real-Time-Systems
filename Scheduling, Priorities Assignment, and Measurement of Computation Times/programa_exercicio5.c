/* Miguel Varandas - 20162198982 e Telmo Cunha - 2018308321 */

#define _GNU_SOURCE
#include <stdio.h>	
#include <time.h>	
#include <sched.h>	
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include "func2.h"
#include <sys/mman.h>
#include <sched.h>
#include <errno.h>



#define CLASS 2	
#define GROUP 6	

#define T1 100
#define T2 200
#define T3 300
#define NSECTSEC 1000000000 
double maxt1=0;  
double maxt2=0;
double maxt3=0;
double mint1=10000;  
double mint2=10000;
double mint3=10000;
double maxtempr1=0;
double maxtempr2=0;
double maxtempr3=0;

void Thread1(void* IN);
void Thread2(void* IN);
void Thread3(void* IN);

double subtrai_soma(struct timespec *A,struct timespec *B,int operacao);
void setperiodo(int T,struct timespec *P);



int main(int argc,char *argv[]){

	mlockall(MCL_CURRENT | MCL_FUTURE);
	// Apenas 1 core
	cpu_set_t mask_main;
	CPU_ZERO(&mask_main);
	CPU_SET(0,&mask_main);
	
	if(sched_setaffinity(getpid(), sizeof(mask_main), &mask_main)<0){
		perror( " Erro em sched_setaffinity : " );
	}

	struct timespec inicio,valorinicial,thread_pack[3];
	clock_gettime(CLOCK_MONOTONIC,&inicio);
	valorinicial.tv_sec=1;
	valorinicial.tv_nsec=(float)0;
	subtrai_soma(&inicio,&valorinicial,0); // guarda o momento inicial
	// momento inicial definido
	int temp;
	// envia o tempo inicial para as threads começarem as 3 no mesmo momento
	for(temp=0;temp<=2;temp++){
		thread_pack[temp].tv_sec=inicio.tv_sec;
		thread_pack[temp].tv_nsec=inicio.tv_nsec;		
	}

	// define as threads
	struct sched_param parameter;		
	pthread_attr_t atribute;
	pthread_t KEY[3];

	pthread_attr_init(&atribute);	//inicia a variavel da thread
	pthread_attr_setschedpolicy(&atribute,SCHED_FIFO); //FIFO
	parameter.sched_priority=sched_get_priority_max(SCHED_FIFO);  //coloca a prioridade a maximo ( RMPO)
	if(pthread_attr_setschedparam(&atribute,&parameter)!=0) 
		printf("Erro em pthread_attr_setschedparam: %s\n",strerror(errno));
	// define as threads
	pthread_attr_setinheritsched(&atribute,PTHREAD_EXPLICIT_SCHED);
	if(pthread_create(&KEY[0],&atribute,(void*)Thread1,&thread_pack[0])!=0) //cria thread 1
		printf("Erro em pthread_create: %s\n",strerror(errno));


	parameter.sched_priority--; //diminui prioridade 
	pthread_attr_setschedparam(&atribute,&parameter);
	pthread_attr_setinheritsched(&atribute,PTHREAD_EXPLICIT_SCHED);
	if(pthread_create(&KEY[1],&atribute,(void*)Thread2,&thread_pack[1])!=0) //cria thread 2
		printf("Erro em pthread_create: %s\n",strerror(errno));
	
	parameter.sched_priority--;
	pthread_attr_setschedparam(&atribute,&parameter);
	pthread_attr_setinheritsched(&atribute,PTHREAD_EXPLICIT_SCHED);
	if(pthread_create(&KEY[2],&atribute,(void*)Thread3,&thread_pack[2])!=0) // cria thread 3
		printf("Erro em pthread_create: %s\n",strerror(errno));
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for(temp=0;temp<=2;temp++){
		if(pthread_join(KEY[temp],NULL)!=0)
			printf("Erro em pthread_join %d: %s\n",temp,strerror(errno));
	}	

	pthread_attr_destroy(&atribute);
	

//imprime a tabela
    printf("\n");
    printf("\n| Thread |  Max Tempo de Computacao  |  Min Tempo de Computacao  |   Time Jitter |    Deadline   |  Max Tempo de Resposta |");
    printf("\n|   1    |           %lf        |           %lf        |    %lf   |      0.1      |	  %lf	  |", maxt1, mint1, maxt1-mint1,  maxtempr1);
    printf("\n|   2    |           %lf        |           %lf        |    %lf   |      0.2      |	  %lf	  |", maxt2, mint2,maxt2-mint2, maxtempr2);
    printf("\n|   3    |           %lf        |           %lf        |    %lf   |      0.3      |	  %lf	  |\n", maxt3, mint3, maxt3-mint3, maxtempr3);
    printf("\n");


return 0;
}


double subtrai_soma(struct timespec *A,struct timespec *B,int operacao){
	if(operacao!=0 && operacao!=1){printf("Erro operacao subtrai_soma\n");
		return 0;}
	if(operacao==1){							//operacao=1 Subtrai o tempos (s/ns)
		double valor;
		valor=(double)(B->tv_nsec-A->tv_nsec)/NSECTSEC;
		valor+=(double)(B->tv_sec-A->tv_sec);
		return valor;
	}
	else{									//operacao=0 Soma o tempos (s/ns)
		A->tv_sec=A->tv_sec+B->tv_sec+(A->tv_nsec+B->tv_nsec)/NSECTSEC;
	A->tv_nsec=(A->tv_nsec+B->tv_nsec)%NSECTSEC;
	return 0;
	}
} 
void setperiodo(int x,struct timespec *Y){ //poe periodo na struct em nanosegundos 

	float z;
	z=(float)x/1000;	// 100/1000=0.1
	Y->tv_sec=0; 		// 0 segundos
	Y->tv_nsec=z*NSECTSEC; //0.1s em nanosegundos
}

void Thread1 (void *IN){
	struct timespec *start=(struct timespec*)IN;
	struct timespec tempo_inicial,tempo_final,espera,periodo,termina;
	espera.tv_sec=start->tv_sec;	
	espera.tv_nsec=start->tv_nsec;
	// Periodo em ns
	setperiodo(T1,&periodo);
	
	termina.tv_sec=(int)6;
	termina.tv_nsec=(float)0;	
	subtrai_soma(&termina,&espera,0); // Acaba aos 6 segundos
	clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&espera,NULL);

	while(subtrai_soma(&espera,&termina,1)>=0){
	clock_gettime(CLOCK_MONOTONIC,&tempo_inicial);	// temmpo em que começa
	
	f1(CLASS,GROUP);		
	
	clock_gettime(CLOCK_MONOTONIC,&tempo_final);	// tempo em que acaba
	
	 if (subtrai_soma(&tempo_inicial,&tempo_final,1) > maxt1)
			maxt1 = subtrai_soma(&tempo_inicial,&tempo_final,1);
        if (subtrai_soma(&tempo_inicial,&tempo_final,1) < mint1)
			mint1 = subtrai_soma(&tempo_inicial,&tempo_final,1);
	
	subtrai_soma(&espera,&periodo,0);	 // aumenta 1 periodo	
	printf("Thread 1:Tempo de ativacao: %f sec , inicio: %f  sec, fim: %f sec, demorou: %f sec, tempo de resposta: %f sec, deadline: %f sec -->",6-0.1-subtrai_soma(&espera,&termina,1),subtrai_soma(start,&tempo_inicial,1),subtrai_soma(start,&tempo_final,1),subtrai_soma(&tempo_inicial,&tempo_final,1),subtrai_soma(start,&tempo_final,1)-(6-0.1-subtrai_soma(&espera,&termina,1)),6-subtrai_soma(&espera,&termina,1) );
	

	if ((subtrai_soma(start,&tempo_final,1)-(6-0.1-subtrai_soma(&espera,&termina,1))) > maxtempr1){
			maxtempr1 = subtrai_soma(start,&tempo_final,1)-(6-0.1-subtrai_soma(&espera,&termina,1));
		}
			

		if(subtrai_soma(&tempo_final,&espera,1)<0){ //tempo final for maior que deadline
			printf("Falhou\n");
		}else{
			printf("Cumpriu\n");
		}
	
	clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&espera,NULL);	
	}

	pthread_exit(NULL);
}
void Thread2 (void *IN){
	struct timespec *start=(struct timespec*)IN;
	struct timespec tempo_inicial,tempo_final,espera,periodo,termina;
	espera.tv_sec=start->tv_sec;	
	espera.tv_nsec=start->tv_nsec;
	// Periodo em ns
	setperiodo(T2,&periodo);
	termina.tv_sec=(int)6;
	termina.tv_nsec=(float)0;	
	subtrai_soma(&termina,&espera,0); // Acaba aos 6 segundos
	clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&espera,NULL);

	while(subtrai_soma(&espera,&termina,1)>=0){
	clock_gettime(CLOCK_MONOTONIC,&tempo_inicial);	// temmpo em que começa
	
	f2(CLASS,GROUP);		
	
	clock_gettime(CLOCK_MONOTONIC,&tempo_final);	// tempo em que acaba
	
	 if (subtrai_soma(&tempo_inicial,&tempo_final,1) > maxt2)
			maxt2 = subtrai_soma(&tempo_inicial,&tempo_final,1);
        if (subtrai_soma(&tempo_inicial,&tempo_final,1) < mint2)
			mint2 = subtrai_soma(&tempo_inicial,&tempo_final,1);
	
	subtrai_soma(&espera,&periodo,0);	 // aumenta 1 periodo	
	printf("Thread 2:Tempo de ativacao: %f sec , inicio: %f  sec, fim: %f sec, demorou: %f sec, tempo de resposta: %f sec, deadline: %f sec -->",6-0.2-subtrai_soma(&espera,&termina,1),subtrai_soma(start,&tempo_inicial,1),subtrai_soma(start,&tempo_final,1),subtrai_soma(&tempo_inicial,&tempo_final,1),subtrai_soma(start,&tempo_final,1)-(6-0.2-subtrai_soma(&espera,&termina,1)),6-subtrai_soma(&espera,&termina,1) );
	

	if ((subtrai_soma(start,&tempo_final,1)-(6-0.2-subtrai_soma(&espera,&termina,1))) > maxtempr2){
			maxtempr2 = subtrai_soma(start,&tempo_final,1)-(6-0.2-subtrai_soma(&espera,&termina,1));
		}
			

		if(subtrai_soma(&tempo_final,&espera,1)<0){ //tempo final for maior que deadline
			printf("Falhou\n");
		}else{
			printf("Cumpriu\n");
		}
	
	clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&espera,NULL);	
	}

	pthread_exit(NULL);
}
void Thread3 (void *IN){
	struct timespec *start=(struct timespec*)IN;
	struct timespec tempo_inicial,tempo_final,espera,periodo,termina;
	espera.tv_sec=start->tv_sec;	
	espera.tv_nsec=start->tv_nsec;
	// Periodo em ns
	setperiodo(T3,&periodo);
	termina.tv_sec=(int)6;
	termina.tv_nsec=(float)0;	
	subtrai_soma(&termina,&espera,0); // Acaba aos 6 segundos
	clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&espera,NULL);

	while(subtrai_soma(&espera,&termina,1)>=0){
	clock_gettime(CLOCK_MONOTONIC,&tempo_inicial);	// temmpo em que começa
	
	f3(CLASS,GROUP);		

	clock_gettime(CLOCK_MONOTONIC,&tempo_final);	// tempo em que acaba
	
	 if (subtrai_soma(&tempo_inicial,&tempo_final,1) > maxt3)
			maxt3 = subtrai_soma(&tempo_inicial,&tempo_final,1);
        if (subtrai_soma(&tempo_inicial,&tempo_final,1) < mint3)
			mint3 = subtrai_soma(&tempo_inicial,&tempo_final,1);
	
	subtrai_soma(&espera,&periodo,0);	 // aumenta 1 periodo	
	printf("Thread 3:Tempo de ativacao: %f sec , inicio: %f  sec, fim: %f sec, demorou: %f sec, tempo de resposta: %f sec, deadline: %f sec -->",6-0.3-subtrai_soma(&espera,&termina,1),subtrai_soma(start,&tempo_inicial,1),subtrai_soma(start,&tempo_final,1),subtrai_soma(&tempo_inicial,&tempo_final,1),subtrai_soma(start,&tempo_final,1)-(6-0.3-subtrai_soma(&espera,&termina,1)),6-subtrai_soma(&espera,&termina,1) );
	

	if ((subtrai_soma(start,&tempo_final,1)-(6-0.3-subtrai_soma(&espera,&termina,1))) > maxtempr3){
			maxtempr3 = subtrai_soma(start,&tempo_final,1)-(6-0.3-subtrai_soma(&espera,&termina,1));
		}
			

		if(subtrai_soma(&tempo_final,&espera,1)<0){ //tempo final for maior que deadline
			printf("Falhou\n");
		}else{
			printf("Cumpriu\n");
		}
	
	clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&espera,NULL);	
	}

	pthread_exit(NULL);
}

